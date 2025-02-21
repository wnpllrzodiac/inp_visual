#include "dfnwidget.h"
#include <QFile>
#include <QVBoxLayout>

DFNWidget::DFNWidget(QWidget *parent)
: QWidget(parent)
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