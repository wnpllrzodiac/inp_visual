#pragma once
#include <QWidget>

class ProjectModel;

class JointPropertiesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit JointPropertiesWidget(QWidget *parent = nullptr, ProjectModel *project_model = nullptr);

private:
    void initUi();
    void setupConnections();

private:
    ProjectModel *project_model_;
};