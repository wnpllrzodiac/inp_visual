#include "jointpropertieswidget.h"
#include "projectmodel.h"
#include <QVBoxLayout>

JointPropertiesWidget::JointPropertiesWidget(QWidget *parent, ProjectModel *project_model)
: QWidget(parent)
, project_model_(project_model)
{
    initUi();
    setupConnections();
}

void JointPropertiesWidget::initUi()
{
    auto layout = new QVBoxLayout(this);
    setLayout(layout);
}

void JointPropertiesWidget::setupConnections() { }