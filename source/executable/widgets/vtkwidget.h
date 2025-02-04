#pragma once

#include <QVTKOpenGLNativeWidget.h>
#include <QWidget>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>

class VtkWidget : public QWidget {
    Q_OBJECT
public:
    enum class FileType {
        Abaqus,
        Gmsh
    };
    explicit VtkWidget(QWidget* parent = nullptr);

    vtkRenderer* renderer() const;
    void resetCamera();

public slots:
    void openAbaqusFile(const QString& file_path);
    void openGmshFile(const QString& file_path);
    void openVtkFile(const QString& file_path);
    void openVtuFile(const QString& file_path);

private:
    void saveUnstructuredGrid(const QString& file_path, vtkUnstructuredGrid* grid);
    void loadInpFile(const QString& filename, const FileType& type);
    void initUi();
    void setupConnections();
    void initVtk();

private:
    QVTKOpenGLNativeWidget* vtk_widget_ {};
    vtkSmartPointer<vtkRenderer> renderer_ {};
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> render_window_ {};
};
