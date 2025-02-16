#pragma once

#include <QDateTime>
#include <QHash>
#include <QMap>
#include <QString>
#include <QTextStream>
#include <vector>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>

struct ModelInfo
{
    QString job_name;
    QString model_name;
    QString generated_by;
    QDateTime created_time;
    QMap<QString, QString> preprint_settings;
};

struct instance_info_t
{
    QString part_name;
    double transform[3];
};

class InpReader
{
public:
    // 获取单例实例
    static InpReader &getInstance()
    {
        static InpReader instance;
        return instance;
    }

    // 删除拷贝构造函数和赋值运算符
    InpReader(const InpReader &) = delete;
    InpReader &operator=(const InpReader &) = delete;

    // 解析inp文件
    bool parseAbaqus(const QString &file_name);

    // 解析inp文件
    bool parseGmsh(const QString &file_name);

    vtkSmartPointer<vtkUnstructuredGrid> getMainGrid() const;

    QMap<QString, std::vector<int>> getNodeSets() const;

    QMap<QString, std::vector<int>> getElementSets() const;

private:
    InpReader() = default;
    ~InpReader() = default;

    // 处理节点集 (*NSET)
    void processNodeSet(const QString &line_header, QTextStream &stream);

    // 处理单元集 (*ELSET)
    void processElementSet(const QString &line_header, QTextStream &stream);

    // 用meshio转换inp文件
    bool parseModel(const QString &file_name);

    // 读取转换后的vtk文件
    bool readConvertedVTK(const QString &file_name);

private:
    ModelInfo model_info_{};
    vtkSmartPointer<vtkUnstructuredGrid> main_grid_ = nullptr;
    QMap<QString, std::vector<int>> node_sets_;    // 存储节点集
    QMap<QString, std::vector<int>> element_sets_; // 存储单元集
};
