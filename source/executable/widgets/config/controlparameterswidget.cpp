#include "controlparameterswidget.h"
#include <QDoubleValidator>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

ControlParametersWidget::ControlParametersWidget(QWidget *parent)
: QWidget(parent)
{
    initUi();
    setupConnections();
}

void ControlParametersWidget::initUi()
{
    auto main_layout = new QVBoxLayout(this);

    const int label_width = 150; // 固定标签宽度
    const int edit_width = 100;  // 固定输入框宽度

    // Time steps
    auto time_steps_layout = new QHBoxLayout();
    auto time_steps_label = new QLabel(tr("Time Steps:"), this);
    time_steps_label->setFixedWidth(label_width);
    ui_.time_steps = new QLineEdit(this);
    ui_.time_steps->setFixedWidth(edit_width);
    ui_.time_steps->setText("1e+05");
    auto time_steps_validator = new QDoubleValidator(this);
    time_steps_validator->setNotation(QDoubleValidator::ScientificNotation);
    ui_.time_steps->setValidator(time_steps_validator);
    auto time_steps_unit = new QLabel(tr("steps"), this);
    time_steps_layout->addWidget(time_steps_label);
    time_steps_layout->addWidget(ui_.time_steps);
    time_steps_layout->addWidget(time_steps_unit);
    time_steps_layout->addStretch();

    // Output frequency
    auto output_freq_layout = new QHBoxLayout();
    auto output_freq_label = new QLabel(tr("Output Frequency:"), this);
    output_freq_label->setFixedWidth(label_width);
    ui_.output_frequency = new QLineEdit(this);
    ui_.output_frequency->setFixedWidth(edit_width);
    ui_.output_frequency->setText("1000");
    auto output_freq_validator = new QDoubleValidator(this);
    output_freq_validator->setNotation(QDoubleValidator::ScientificNotation);
    ui_.output_frequency->setValidator(output_freq_validator);
    auto output_freq_unit = new QLabel(tr("steps"), this);
    output_freq_layout->addWidget(output_freq_label);
    output_freq_layout->addWidget(ui_.output_frequency);
    output_freq_layout->addWidget(output_freq_unit);
    output_freq_layout->addStretch();

    // Minimum element size
    auto min_size_layout = new QHBoxLayout();
    auto min_size_label = new QLabel(tr("Minimum Element Size:"), this);
    min_size_label->setFixedWidth(label_width);
    ui_.min_element_size = new QLineEdit(this);
    ui_.min_element_size->setFixedWidth(edit_width);
    ui_.min_element_size->setText("1");
    auto min_size_validator = new QIntValidator(this);
    ui_.min_element_size->setValidator(min_size_validator);
    auto min_size_unit = new QLabel(tr("..."), this);
    min_size_layout->addWidget(min_size_label);
    min_size_layout->addWidget(ui_.min_element_size);
    min_size_layout->addWidget(min_size_unit);
    min_size_layout->addStretch();

    // Time step
    auto time_step_layout = new QHBoxLayout();
    auto time_step_label = new QLabel(tr("Time Step:"), this);
    time_step_label->setFixedWidth(label_width);
    ui_.time_step = new QLineEdit(this);
    ui_.time_step->setFixedWidth(edit_width);
    ui_.time_step->setText("5e-06");
    auto time_step_validator = new QDoubleValidator(this);
    time_step_validator->setNotation(QDoubleValidator::ScientificNotation);
    ui_.time_step->setValidator(time_step_validator);
    auto time_step_unit = new QLabel(tr("ms"), this);
    time_step_layout->addWidget(time_step_label);
    time_step_layout->addWidget(ui_.time_step);
    time_step_layout->addWidget(time_step_unit);
    time_step_layout->addStretch();

    // Gravity
    auto gravity_layout = new QHBoxLayout();
    auto gravity_label = new QLabel(tr("Gravity:"), this);
    gravity_label->setFixedWidth(label_width);
    ui_.gravity = new QLineEdit(this);
    ui_.gravity->setFixedWidth(edit_width);
    ui_.gravity->setText("1000");
    auto gravity_validator = new QDoubleValidator(this);
    gravity_validator->setNotation(QDoubleValidator::ScientificNotation);
    ui_.gravity->setValidator(gravity_validator);
    auto gravity_unit = new QLabel(tr("mm/ms^2"), this);
    gravity_layout->addWidget(gravity_label);
    gravity_layout->addWidget(ui_.gravity);
    gravity_layout->addWidget(gravity_unit);
    gravity_layout->addStretch();

    main_layout->addLayout(time_steps_layout);
    main_layout->addLayout(output_freq_layout);
    main_layout->addLayout(min_size_layout);
    main_layout->addLayout(time_step_layout);
    main_layout->addLayout(gravity_layout);
    main_layout->addStretch();

    setLayout(main_layout);
}

void ControlParametersWidget::setupConnections()
{
    // 在这里添加信号槽连接
}