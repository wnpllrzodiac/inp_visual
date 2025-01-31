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
    void openInpFile();
    void saveInpFile();

private:
    void initUI();
    void setupConnections();
    void retranslateUI();

    QAction* open_action_;
    QAction* save_action_;
    ConfigWidget* configWidget_;
    VtkWidget* vtkWidget_;
};
