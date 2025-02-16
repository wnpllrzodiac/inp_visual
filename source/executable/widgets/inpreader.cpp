#include "inpreader.h"
#include "utils/logging.h"
#include <QCoreApplication>
#include <QFile>
#include <QProcess>
#include <QRegularExpression>
#include <QStringList>
#include <QTextStream>

#include <vtkAppendFilter.h>
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkTransform.h>
#include <vtkTransformFilter.h>
#include <vtkTriangle.h>
#include <vtkUnstructuredGridReader.h>

bool InpReader::parseGmsh(const QString& file_name)
{

    {
        QFile file(file_name);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return false;
        }

        QTextStream stream(&file);
        QString line;

        while (!stream.atEnd()) {
            // auto pos_before = stream.pos();
            line = stream.readLine();
            if (line.isEmpty())
                continue;

            if (line.startsWith("*NSET", Qt::CaseInsensitive)) {
                // 处理节点集
                processNodeSet(line, stream);

            } else if (line.startsWith("*ELSET", Qt::CaseInsensitive)) {
                // 处理单元集
                processElementSet(line, stream);
            }
        }
    }
    return praseModel(file_name);
}

bool InpReader::parseAbaqus(const QString& file_name)

{
    main_grid_ = vtkSmartPointer<vtkUnstructuredGrid>::New();
    return true;
}

vtkSmartPointer<vtkUnstructuredGrid> InpReader::getMainGrid() const
{
    return main_grid_;
}

QMap<QString, std::vector<int>> InpReader::getNodeSets() const
{
    return node_sets_;
}

QMap<QString, std::vector<int>> InpReader::getElementSets() const
{
    return element_sets_;
}

//------------------------------------------------------
// 处理节点集 (*NSET)
//------------------------------------------------------
void InpReader::processNodeSet(const QString& line_header, QTextStream& stream)

{
    // 解析NSET名称
    QString set_name;
    if (line_header.contains("NSET=", Qt::CaseInsensitive)) {
        set_name = line_header.section("NSET=", 1).section(',', 0, 0).trimmed();
    } else {
        return; // 没有找到NSET名称,退出
    }

    std::vector<int> node_ids;

    while (!stream.atEnd()) {
        QString line = stream.readLine().trimmed();
        if (line.startsWith("*"))
            break;

        // 解析这一行的节点ID
        QStringList ids = line.split(',', Qt::SkipEmptyParts);
        for (const QString& id_str : ids) {
            bool ok;
            int id = id_str.trimmed().toInt(&ok);
            if (ok) {
                node_ids.push_back(id);
            }
        }
    }

    // 存储节点集
    node_sets_[set_name] = node_ids;
}

//------------------------------------------------------
// 处理单元集 (*ELSET)
//------------------------------------------------------
void InpReader::processElementSet(const QString& line_header, QTextStream& stream)
{
    // 解析ELSET名称
    QString set_name;
    if (line_header.contains("ELSET=", Qt::CaseInsensitive)) {
        set_name = line_header.section("ELSET=", 1).section(',', 0, 0).trimmed();
    } else {
        return; // 没有找到ELSET名称,退出
    }

    std::vector<int> element_ids;

    while (!stream.atEnd()) {
        QString line = stream.readLine().trimmed();
        if (line.startsWith("*"))
            break;

        // 解析这一行的单元ID
        QStringList ids = line.split(',', Qt::SkipEmptyParts);
        for (const QString& id_str : ids) {
            bool ok;
            int id = id_str.trimmed().toInt(&ok);
            if (ok) {
                element_ids.push_back(id);
            }
        }
    }

    // 存储单元集
    element_sets_[set_name] = element_ids;
}

bool InpReader::praseModel(const QString& file_name)
{
    // 创建临时文件
    QString temp_file = file_name + ".vtk";
    QFile::copy(file_name, temp_file);

    auto meshio_path = QCoreApplication::applicationDirPath() + "/meshio.exe";
    if (!QFile::exists(meshio_path)) {
        return false;
    }

    // 使用meshio转换inp文件
    QStringList arguments;
    arguments << "convert" << file_name << temp_file;
    int exit_code = QProcess::execute(meshio_path, arguments);

    // 检查进程是否成功执行
    if (exit_code != 0) {
        // 转换失败，清理临时文件
        QFile::remove(temp_file);
        return false;
    }

    if (!QFile::exists(temp_file)) {
        return false;
    }

    return readConvertedVTK(temp_file);
}

bool InpReader::readConvertedVTK(const QString& file_name)

{
    // 读取转换后的vtk文件
    auto reader = vtkSmartPointer<vtkUnstructuredGridReader>::New();
    reader->SetFileName(file_name.toStdString().c_str());
    reader->Update();

    main_grid_ = reader->GetOutput();
    QFile::remove(file_name);
    return true;
}
