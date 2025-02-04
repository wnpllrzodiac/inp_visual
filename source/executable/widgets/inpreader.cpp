#include "inpreader.h"
#include "utils/logging.h"
#include <QFile>
#include <QRegularExpression>
#include <QStringList>
#include <QTextStream>

#include <vtkAppendFilter.h>
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkTransform.h>
#include <vtkTransformFilter.h>
#include <vtkTriangle.h>

vtkSmartPointer<vtkUnstructuredGrid> InpReader::parseGmsh(const QString& file_name)
{
    // 创建返回的网格
    auto grid = vtkSmartPointer<vtkUnstructuredGrid>::New();
    // 节点映射表，用于 processNodes 和 processElements 的关联
    QHash<int, vtkIdType> node_map;
    // 创建用于存放节点的点集合
    auto points = vtkSmartPointer<vtkPoints>::New();

    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // 如果无法打开文件，返回空网格
        return grid;
    }

    QTextStream stream(&file);
    QString line;
    // 对于节点坐标没有偏移，所以默认为0
    double offset[3] = { 0.0, 0.0, 0.0 };

    while (!stream.atEnd()) {
        line = stream.readLine().trimmed();
        if (line.isEmpty())
            continue;

        if (line.startsWith("*Heading", Qt::CaseInsensitive)) {
            // 解析 Heading 区域，获取模型信息
            processHeading(stream);
        } else if (line.startsWith("*NODE", Qt::CaseInsensitive)) {
            // 解析节点数据
            processNodesBulk(stream, grid, node_map, offset);
            grid->SetPoints(points);
        } else if (line.startsWith("*ELEMENT", Qt::CaseInsensitive)) {
            // 解析单元数据（目前仅假设为三角形单元）
            processElementsBulk(stream, grid, node_map);
        }
        // 其他关键字（例如 *ELSET、*NSET 等）暂不处理
    }

    return grid;
}

vtkSmartPointer<vtkUnstructuredGrid> InpReader::parseAbaqus(const QString& file_name)

{
    auto main_grid = vtkSmartPointer<vtkUnstructuredGrid>::New();
    parts_map_.clear();
    instances_map_.clear();

    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return main_grid;
    }
    QTextStream stream(&file);

    double transform[3] = { 0, 0, 0 };
    while (!stream.atEnd()) {
        // 读一行
        QString line = stream.readLine().trimmed();
        if (line.startsWith("*Heading", Qt::CaseInsensitive)) {
            processHeading(stream);
        } else if (line.startsWith("*Part,", Qt::CaseInsensitive)) {
            // 解析部件名
            QString part_name = line.section("name=", 1).section(',', 0, 0).trimmed();
            auto part_grid = vtkSmartPointer<vtkUnstructuredGrid>::New();
            QHash<int, vtkIdType> part_node_map;

            // 进入 part 内部
            while (!stream.atEnd()) {
                qint64 pos_before = stream.pos();
                QString sub_line = stream.readLine().trimmed();
                if (sub_line.startsWith("*End Part", Qt::CaseInsensitive)) {
                    break;
                } else if (sub_line.startsWith("*Node", Qt::CaseInsensitive)) {
                    // 回退，让 process_nodes_bulk 自己从下一行开始读
                    stream.seek(pos_before);
                    processNodesBulk(stream, part_grid, part_node_map, transform);
                } else if (sub_line.startsWith("*Element", Qt::CaseInsensitive)) {
                    // 回退
                    stream.seek(pos_before);
                    processElementsBulk(stream, part_grid, part_node_map);
                }
            }

            parts_map_[part_name] = part_grid;
        } else if (line.startsWith("*Assembly", Qt::CaseInsensitive)) {
            processAssembly(stream);
        } else if (line.startsWith("*Instance", Qt::CaseInsensitive)) {
            // 如果有写在外面，简单解析 transform
            parseTransform(line, transform);
        }
    }

    // 合并所有 instances 到 main_grid
    for (auto it = instances_map_.cbegin(); it != instances_map_.cend(); ++it) {
        const QString& instance_name = it.key();
        const instance_info_t& instance_info = it.value();
        if (parts_map_.contains(instance_info.part_name)) {
            mergeGrids(main_grid, parts_map_.value(instance_info.part_name), instance_info.transform);
        }
    }
    return main_grid;
}

//------------------------------------------------------
// 批量解析节点 (Node)
// 不再一行行地往 points 里 insert_next_point
// 而是先把所有节点行存进 vector，一次性设置 Points
//------------------------------------------------------
void InpReader::processNodesBulk(
    QTextStream& stream, vtkSmartPointer<vtkUnstructuredGrid>& grid, QHash<int, vtkIdType>& node_map, const double offset[3])
{
    // 临时存储所有节点行
    std::vector<QString> node_lines;
    // 跳过 "*Node" 这一行
    QString line = stream.readLine().trimmed();
    // 继续往下读，直到遇到下一个 * 关键字 或 文件结束
    while (!stream.atEnd()) {
        qint64 pos_before = stream.pos();
        QString l = stream.readLine();
        QString trimmed = l.trimmed();
        if (trimmed.startsWith("*", Qt::CaseInsensitive)) {
            // 回退
            stream.seek(pos_before);
            break;
        }
        if (!trimmed.isEmpty()) {
            node_lines.push_back(trimmed);
        }
    }

    if (node_lines.empty()) {
        return;
    }

    // 先创建一个容器，用于存储解析后的 (id, x, y, z)
    // 注意如果 node id 很大且不连续，你还是需要用 hash
    // 此处演示仅存放坐标和 ID 的 vector，后续再做映射
    struct node_record {
        int id;
        double x, y, z;
    };
    std::vector<node_record> nodes;
    nodes.reserve(node_lines.size());

    // 逐行解析
    for (auto& ln : node_lines) {
        // 用 indexOf(',', ...) + mid() 或者 QString::splitRef() 可以略微快一点
        // 这里演示就简单写 split(',')
        QStringList parts = ln.split(',', Qt::SkipEmptyParts);
        if (parts.size() < 3) {
            continue;
        }
        bool ok = false;
        int node_id = parts[0].toInt(&ok);
        if (!ok) {
            continue;
        }
        double xx = parts[1].toDouble(&ok);
        if (!ok)
            xx = 0.0;
        double yy = parts[2].toDouble(&ok);
        if (!ok)
            yy = 0.0;
        double zz = 0.0;
        if (parts.size() > 3) {
            zz = parts[3].toDouble(&ok);
            if (!ok)
                zz = 0.0;
        }
        nodes.push_back({ node_id, xx + offset[0], yy + offset[1], zz + offset[2] });
    }

    // 现在我们有了所有节点数据，可以一次性创建 vtkPoints
    auto points = vtkSmartPointer<vtkPoints>::New();
    points->SetNumberOfPoints(static_cast<vtkIdType>(nodes.size()));

    // 把坐标填进去
    for (vtkIdType i = 0; i < static_cast<vtkIdType>(nodes.size()); ++i) {
        points->SetPoint(i, nodes[i].x, nodes[i].y, nodes[i].z);
    }

    // 把 grid 的 points 设置好
    grid->SetPoints(points);

    // 维护 id -> vtkId 的映射
    // 这里假设 nodes 各不重复
    // 如果 inp 文件里有重复id，需要额外去重判断
    for (vtkIdType i = 0; i < static_cast<vtkIdType>(nodes.size()); ++i) {
        node_map[nodes[i].id] = i;
    }
}

//------------------------------------------------------
// 批量解析单元 (Elements)
// 假设三角形为主，若有其他类型需自定义
//------------------------------------------------------
void InpReader::processElementsBulk(QTextStream& stream, vtkSmartPointer<vtkUnstructuredGrid>& grid, const QHash<int, vtkIdType>& node_map)
{
    // 跳过 "*Element" 这一行
    QString line = stream.readLine().trimmed();

    // 临时存储所有单元行
    std::vector<QString> elem_lines;

    while (!stream.atEnd()) {
        qint64 pos_before = stream.pos();
        QString l = stream.readLine();
        QString trimmed = l.trimmed();
        if (trimmed.startsWith("*", Qt::CaseInsensitive)) {
            stream.seek(pos_before);
            break;
        }
        if (!trimmed.isEmpty()) {
            elem_lines.push_back(trimmed);
        }
    }
    if (elem_lines.empty()) {
        return;
    }

    // 本例只针对三角单元 (element_id, n1, n2, n3)
    // 先分配一个 cellarray
    auto cells = vtkSmartPointer<vtkCellArray>::New();
    cells->AllocateEstimate(static_cast<vtkIdType>(elem_lines.size()), 3);

    for (auto& ln : elem_lines) {
        // 把逗号后面的节点拿到
        // ln 形如: "  101, 10, 11, 12" => element_id=101, 节点=10,11,12
        QStringList parts = ln.split(',', Qt::SkipEmptyParts);
        if (parts.size() < 4) {
            continue;
        }
        // parts[0] 是单元ID, 1..3是节点
        // 这里只简单取前三个节点构三角形
        int n1 = parts[1].toInt();
        int n2 = parts[2].toInt();
        int n3 = parts[3].toInt();
        // 查找映射
        auto it1 = node_map.find(n1);
        auto it2 = node_map.find(n2);
        auto it3 = node_map.find(n3);
        if (it1 == node_map.end() || it2 == node_map.end() || it3 == node_map.end()) {
            continue;
        }

        vtkIdType pts[3] = { it1.value(), it2.value(), it3.value() };
        cells->InsertNextCell(3, pts);
    }

    // 设置到 grid
    grid->SetCells(VTK_TRIANGLE, cells);
}

//------------------------------------------------------
// 解析 *Heading 区域
// 若量不大，也无所谓性能
//------------------------------------------------------
void InpReader::processHeading(QTextStream& stream)
{
    while (!stream.atEnd()) {
        qint64 pos_before = stream.pos();
        QString line = stream.readLine().trimmed();
        if (line.startsWith("*Part", Qt::CaseInsensitive) || line.startsWith("*Assembly", Qt::CaseInsensitive)
            || line.startsWith("*Instance", Qt::CaseInsensitive) || line.startsWith("*Node", Qt::CaseInsensitive)
            || line.startsWith("*Element", Qt::CaseInsensitive)) {
            stream.seek(pos_before);
            break;
        }
        // 尝试手动寻找 job / model / generated 等关键字
        if (line.contains("Job name:", Qt::CaseInsensitive)) {
            // 这里就随便写个例子，别再用QRegularExpression匹配整行了
            int idx = line.indexOf("Job name:", 0, Qt::CaseInsensitive);
            if (idx >= 0) {
                QString rest = line.mid(idx + 9).trimmed();
                model_info_.job_name = rest.split(' ', Qt::SkipEmptyParts).value(0);
            }
        }
        // ... 同理对 Model name / Generated by 等做简单解析
        if (line.startsWith("*Preprint", Qt::CaseInsensitive)) {
            // 这里可以不做高性能要求，因为preprint一般就一两行
            QStringList settings_list = line.section(',', 1).split(',');
            for (const QString& setting : settings_list) {
                QStringList kv = setting.split('=');
                if (kv.size() == 2) {
                    model_info_.preprint_settings[kv[0].trimmed()] = kv[1].trimmed();
                }
            }
        }
    }
}

//------------------------------------------------------
// 解析 *Assembly, 主要提取 *Instance
//------------------------------------------------------
void InpReader::processAssembly(QTextStream& stream)
{
    while (!stream.atEnd()) {
        QString line = stream.readLine().trimmed();

        if (line.startsWith("*End Assembly", Qt::CaseInsensitive)) {
            break;
        }
        if (line.startsWith("*Instance", Qt::CaseInsensitive)) {
            instance_info_t inst;
            inst.transform[0] = inst.transform[1] = inst.transform[2] = 0.0;

            // 解析 name=xxx, part=xxx
            QString instance_name = line.section("name=", 1).section(',', 0, 0).trimmed();
            inst.part_name = line.section("part=", 1).section(',', 0, 0).trimmed();

            // 看下一行是否有平移数字
            qint64 pos_before2 = stream.pos();
            if (!stream.atEnd()) {
                QString transform_line = stream.readLine().trimmed();
                if (transform_line.startsWith("*", Qt::CaseInsensitive)) {
                    // 没有平移
                    stream.seek(pos_before2);
                } else {
                    parseTransform(transform_line, inst.transform);
                }
            }
            instances_map_[instance_name] = inst;
        }
        // 其他情况，就继续
    }
}

//------------------------------------------------------
// 简易解析平移
//------------------------------------------------------
void InpReader::parseTransform(const QString& line, double transform[3])
{
    // 全置 0
    transform[0] = transform[1] = transform[2] = 0.0;

    // 把可能的 "name=..., part=..." 去掉
    QString cleaned = line;
    cleaned.remove(QRegularExpression("name=[^,]+", QRegularExpression::CaseInsensitiveOption));
    cleaned.remove(QRegularExpression("part=[^,]+", QRegularExpression::CaseInsensitiveOption));
    cleaned.remove("*Instance", Qt::CaseInsensitive);

    QStringList tokens = cleaned.split(',', Qt::SkipEmptyParts);
    int idx = 0;
    for (auto& tk : tokens) {
        tk = tk.trimmed();
        if (tk.isEmpty())
            continue;
        bool ok = false;
        double val = tk.toDouble(&ok);
        if (ok) {
            transform[idx++] = val;
            if (idx >= 3)
                break;
        }
    }
}

//------------------------------------------------------
// 合并网格：对 part_grid 做平移后 append 到 main_grid
//------------------------------------------------------
void InpReader::mergeGrids(vtkUnstructuredGrid* main_grid, vtkUnstructuredGrid* part_grid, const double transform[3])
{
    vtkNew<vtkTransform> translation;
    translation->Translate(transform[0], transform[1], transform[2]);

    vtkNew<vtkTransformFilter> transform_filter;
    transform_filter->SetInputData(part_grid);
    transform_filter->SetTransform(translation);
    transform_filter->Update();

    vtkNew<vtkAppendFilter> append_filter;
    append_filter->MergePointsOn();
    append_filter->AddInputData(main_grid);
    append_filter->AddInputData(transform_filter->GetOutput());
    append_filter->Update();

    main_grid->DeepCopy(append_filter->GetOutput());
}
