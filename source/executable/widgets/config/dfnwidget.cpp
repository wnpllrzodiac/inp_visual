#include "dfnwidget.h"
#include "projectmodel.h"
#include <QFile>
#include <QVBoxLayout>

DFNWidget::DFNWidget(QWidget *parent, ProjectModel *project_model)
: QWidget(parent)
, project_model_(project_model)
{
    initUi();
    setupConnections();
}

void DFNWidget::initUi()
{
    auto layout = new QVBoxLayout(this);
    setLayout(layout);
}

void DFNWidget::setupConnections() { }