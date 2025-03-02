#pragma once

#include <QWidget>

class ProjectModel;

class DFNWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DFNWidget(QWidget *parent = nullptr, ProjectModel *project_model = nullptr);

private:
    void initUi();
    void setupConnections();

private:
    ProjectModel *project_model_;
};