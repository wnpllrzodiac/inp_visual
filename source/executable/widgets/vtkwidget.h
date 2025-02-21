#pragma once

#include <QAction>
#include <QLabel>
#include <QToolBar>
#include <QVTKOpenGLNativeWidget.h>
#include <QWidget>
#include <vtkCellPicker.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>

#include "utils/vtksignal.h"

class VtkWidget : public QWidget
{
    Q_OBJECT
public:
    enum class FileType
    {
        Abaqus,
        Gmsh
    };

    enum class InteractorStyle
    {
        PickPoint,
        PickCell
    };

    explicit VtkWidget(QWidget *parent = nullptr);

    vtkRenderer *renderer() const;
    void resetCamera();

    // 添加新的颜色修改方法
    void setCellColor(size_t cellId, const QColor &color);
    void setPointColor(size_t pointId, const QColor &color);
    void setCellSetColor(const QString &setName, const QColor &color);
    void setNodeSetColor(const QString &setName, const QColor &color);

public slots:
    void openAbaqusFile(const QString &file_path);
    void openGmshFile(const QString &file_path);

private slots:
    void onCellSelected(size_t cellId, int x, int y);
    void onPointSelected(size_t pointId, int x, int y);
    void onChangeCellColor();
    void onChangePointColor();
    void switchInteractorStyle();

private:
    void loadInpFile(const QString &filename, const FileType &type);
    void initUi();
    void setupConnections();
    void initVtk();
    vtkSmartPointer<vtkIntArray> generateCellColors(const QMap<QString, std::vector<int>> &elset_sets);
    vtkSmartPointer<vtkIntArray> generatePointColors(const QMap<QString, std::vector<int>> &nset_sets);

private:
    QVTKOpenGLNativeWidget *vtk_widget_{};
    vtkSmartPointer<vtkRenderer> renderer_{};
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> render_window_{};
    vtkSmartPointer<vtkUnstructuredGrid> grid_;
    vtkSmartPointer<vtkCellPicker> picker_;

    SignalEmitter *emitter_;
    struct
    {
        QToolBar *toolbar;
        QAction *switch_style;
        QLabel *tooltip_widget;
    } ui_;

    InteractorStyle current_style_; // 当前交互样式

    // 添加新的成员变量来存储颜色映射
    vtkSmartPointer<vtkLookupTable> cell_lut_;
    vtkSmartPointer<vtkLookupTable> point_lut_;
    QMap<QString, size_t> elset_index_map_;
    QMap<QString, size_t> nset_index_map_;
};
