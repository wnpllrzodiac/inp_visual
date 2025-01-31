#include "configwidget.h"

ConfigWidget::ConfigWidget(QWidget* parent)
    : QWidget(parent)
{
    initUi();
    setupConnections();
}

void ConfigWidget::initUi()
{
    auto layout = new QVBoxLayout(this);

    // 长度单位
    auto lengthLabel = new QLabel("长度单位", this);
    lengthUnitEdit = new QLineEdit(this);
    lengthUnitEdit->setText("m");

    // 质量单位
    auto massLabel = new QLabel("质量单位", this);
    massUnitEdit = new QLineEdit(this);
    massUnitEdit->setText("kg");

    // 时间单位
    auto timeLabel = new QLabel("时间单位", this);
    timeUnitEdit = new QLineEdit(this);
    timeUnitEdit->setText("s");

    layout->addWidget(lengthLabel);
    layout->addWidget(lengthUnitEdit);
    layout->addWidget(massLabel);
    layout->addWidget(massUnitEdit);
    layout->addWidget(timeLabel);
    layout->addWidget(timeUnitEdit);
    layout->addStretch();

    setLayout(layout);
}

void ConfigWidget::setupConnections()
{
}
