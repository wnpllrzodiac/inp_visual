#ifndef CONFIGWIDGET_H
#define CONFIGWIDGET_H

#include <QLabel>
#include <QLineEdit>
#include <QTabWidget>
#include <QVBoxLayout>

class ProjectModel;

class ConfigWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit ConfigWidget(QWidget *parent = nullptr, ProjectModel *project_model = nullptr);

private:
    void initUi();
    void setupConnections();

private:
    ProjectModel *project_model_;
};

#endif // CONFIGWIDGET_H
