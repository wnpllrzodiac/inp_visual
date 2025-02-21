#include "jointpropertieswidget.h"
#include <QVBoxLayout>

JointPropertiesWidget::JointPropertiesWidget(QWidget *parent)
: QWidget(parent)
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