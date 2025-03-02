#include "unitwidget.h"
#include "projectmodel.h"
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

UnitWidget::UnitWidget(QWidget *parent, ProjectModel *project_model)
: QWidget(parent)
, project_model_(project_model)
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
    ui_.length_combo->addItems({tr("mm (millimeter)"), tr("m (meter)"), tr("km (kilometer)")});
    ui_.length_combo->setCurrentIndex(static_cast<int>(project_model_->lengthUnit()));
    length_layout->addWidget(length_label);
    length_layout->addWidget(ui_.length_combo);

    // Mass unit
    auto mass_layout = new QHBoxLayout();
    auto mass_label = new QLabel(tr("Mass Unit:"), this);
    ui_.mass_combo = new QComboBox(this);
    ui_.mass_combo->addItems({tr("g (gram)"), tr("kg (kilogram)"), tr("t (ton)")});
    ui_.mass_combo->setCurrentIndex(static_cast<int>(project_model_->massUnit()));
    mass_layout->addWidget(mass_label);
    mass_layout->addWidget(ui_.mass_combo);

    // Time unit
    auto time_layout = new QHBoxLayout();
    auto time_label = new QLabel(tr("Time Unit:"), this);
    ui_.time_combo = new QComboBox(this);
    ui_.time_combo->addItems({tr("μs (microsecond)"), tr("ms (millisecond)"), tr("s (second)")});
    ui_.time_combo->setCurrentIndex(static_cast<int>(project_model_->timeUnit()));
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
    connect(
        ui_.length_combo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &UnitWidget::onLengthUnitChanged);
    connect(ui_.mass_combo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &UnitWidget::onMassUnitChanged);
    connect(ui_.time_combo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &UnitWidget::onTimeUnitChanged);
}

void UnitWidget::onLengthUnitChanged(int index) { project_model_->setLengthUnit(static_cast<LengthUnit>(index)); }

void UnitWidget::onMassUnitChanged(int index) { project_model_->setMassUnit(static_cast<MassUnit>(index)); }

void UnitWidget::onTimeUnitChanged(int index) { project_model_->setTimeUnit(static_cast<TimeUnit>(index)); }
