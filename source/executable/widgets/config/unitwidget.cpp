#include "unitwidget.h"
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

UnitWidget::UnitWidget(QWidget *parent)
: QWidget(parent)
{
    initUi();
    setupConnections();
}

void UnitWidget::initUi()
{
    auto mainLayout = new QVBoxLayout(this);

    // Length unit
    auto length_layout = new QHBoxLayout();
    auto length_label = new QLabel(tr("Length Unit:"), this);
    ui_.length_combo = new QComboBox(this);
    ui_.length_combo->addItems({tr("Millimeter (mm)"), tr("Centimeter (cm)"), tr("Meter (m)")});
    length_layout->addWidget(length_label);
    length_layout->addWidget(ui_.length_combo);

    // Mass unit
    auto mass_layout = new QHBoxLayout();
    auto mass_label = new QLabel(tr("Mass Unit:"), this);
    ui_.mass_combo = new QComboBox(this);
    ui_.mass_combo->addItems({tr("Gram (g)"), tr("Kilogram (kg)"), tr("Ton (t)")});
    mass_layout->addWidget(mass_label);
    mass_layout->addWidget(ui_.mass_combo);

    // Time unit
    auto time_layout = new QHBoxLayout();
    auto time_label = new QLabel(tr("Time Unit:"), this);
    ui_.time_combo = new QComboBox(this);
    ui_.time_combo->addItems({tr("Microsecond (μs)"), tr("Millisecond (ms)"), tr("Second (s)")});
    time_layout->addWidget(time_label);
    time_layout->addWidget(ui_.time_combo);

    mainLayout->addLayout(length_layout);
    mainLayout->addLayout(mass_layout);
    mainLayout->addLayout(time_layout);
    mainLayout->addStretch();

    setLayout(mainLayout);
}

void UnitWidget::setupConnections()
{
    // 在这里添加信号槽连接
    // 例如当用户选择不同单位时触发相应的处理函数
}
