#pragma once

#include <QDateTime>
#include <QHash>
#include <QMap>
#include <QString>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>

// 用于存储一些模型的元信息（作业名、模型名、生成者、时间等）
struct model_info_t {
    QString job_name;
    QString model_name;
    QString generated_by;
    QDateTime created_time;
    QMap<QString, QString> preprint_settings; // 存储 *Preprint 中的自定义设置
};

// 用于存储实例信息
struct instance_info_t {
    QString part_name; // 实例对应的部件名
    double transform[3]; // 简单的平移变换（x, y, z）
};

class InpReader {
public:
    InpReader() = default;
    ~InpReader() = default;

    // 解析inp文件，返回合并后的 vtkUnstructuredGrid
    vtkSmartPointer<vtkUnstructuredGrid> parse(const QString& file_name);

private:
    // 解析 *Heading 区域
    void processHeading(QTextStream& stream);

    // 解析节点信息
    void processNodes(QTextStream& stream, vtkPoints* points, QHash<int, vtkIdType>& node_map, const double offset[3]);

    // 解析单元（此处假设三角形单元为主）
    void processElements(QTextStream& stream, vtkUnstructuredGrid* grid, const QHash<int, vtkIdType>& node_map);

    // 解析变换参数，这里仅简易示例，假设只有简单的 x,y,z 平移
    void parseTransform(const QString& line, double transform[3]);

    // 解析 *Assembly 区域中的 *Instance
    void processAssembly(QTextStream& stream);

    // 合并网格到主网格里
    void mergeGrids(vtkUnstructuredGrid* main_grid, vtkUnstructuredGrid* part_grid, const double transform[3]);

private:
    // 模型信息
    model_info_t model_info_;

    // 存储各个 part 的网格
    QHash<QString, vtkSmartPointer<vtkUnstructuredGrid>> parts_map_;

    // 存储所有 instance 信息
    QHash<QString, instance_info_t> instances_map_;
};
