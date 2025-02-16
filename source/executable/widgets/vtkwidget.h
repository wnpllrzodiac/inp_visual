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

public slots:
    void openAbaqusFile(const QString &file_path);
    void openGmshFile(const QString &file_path);

private slots:
    void onCellSelected(size_t cellId, int x, int y);
    void onPointSelected(size_t pointId, int x, int y);
    void switchInteractorStyle();

private:
    void loadInpFile(const QString &filename, const FileType &type);
    void initUi();
    void setupConnections();
    void initVtk();

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
};
