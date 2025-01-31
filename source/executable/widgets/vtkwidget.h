#pragma once

#include <QVTKOpenGLNativeWidget.h>
#include <QWidget>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>

class VtkWidget : public QWidget {
    Q_OBJECT
public:
    explicit VtkWidget(QWidget* parent = nullptr);

    vtkRenderer* renderer() const;
    void resetCamera();

public slots:
    void openInpFile(const QString& file_path);
    void loadInpFile(const QString& filename);

private:
    void initUi();
    void setupConnections();
    void initVtk();

private:
    QVTKOpenGLNativeWidget* vtk_widget_ {};
    vtkSmartPointer<vtkRenderer> renderer_ {};
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> render_window_ {};
};
