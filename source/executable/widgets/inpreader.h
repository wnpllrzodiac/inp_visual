#pragma once

#include <QDateTime>
#include <QHash>
#include <QMap>
#include <QString>
#include <QTextStream>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>

struct ModelInfo {
    QString job_name;
    QString model_name;
    QString generated_by;
    QDateTime created_time;
    QMap<QString, QString> preprint_settings;
};

struct instance_info_t {
    QString part_name;
    double transform[3];
};

class InpReader {
public:
    InpReader() = default;
    ~InpReader() = default;

    // 解析inp文件，返回合并后的 vtkUnstructuredGrid
    vtkSmartPointer<vtkUnstructuredGrid> parseAbaqus(const QString& file_name);

    // 解析inp文件，返回合并后的 vtkUnstructuredGrid
    vtkSmartPointer<vtkUnstructuredGrid> parseGmsh(const QString& file_name);

private:
    // 核心解析函数
    void processHeading(QTextStream& stream);
    void processAssembly(QTextStream& stream);

    // 批量解析 Node
    void processNodesBulk(
        QTextStream& stream, vtkSmartPointer<vtkUnstructuredGrid>& grid, QHash<int, vtkIdType>& node_map, const double offset[3]);

    // 批量解析 Element
    void processElementsBulk(QTextStream& stream, vtkSmartPointer<vtkUnstructuredGrid>& grid, const QHash<int, vtkIdType>& node_map);

    // 解析简单平移
    void parseTransform(const QString& line, double transform[3]);

    // 合并网格
    void mergeGrids(vtkUnstructuredGrid* main_grid, vtkUnstructuredGrid* part_grid, const double transform[3]);

private:
    ModelInfo model_info_;
    QHash<QString, vtkSmartPointer<vtkUnstructuredGrid>> parts_map_;
    QHash<QString, instance_info_t> instances_map_;
};
