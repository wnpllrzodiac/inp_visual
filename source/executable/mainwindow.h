#pragma once

#include <QMainWindow>

class VtkWidget;
class ConfigWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

public slots:
    void openAbaqusFile();
    void openGmshFile();
    void openVtuFile();
    void openVtkFile();
    void saveInpFile();

private:
    void initUI();
    void setupConnections();
    void retranslateUI();

    QAction* open_abaqus_action_;
    QAction* open_gmsh_action_;
    QAction* open_vtu_action_;
    QAction* open_vtk_action_;
    QAction* save_action_;
    ConfigWidget* configWidget_;
    VtkWidget* vtkWidget_;
};
