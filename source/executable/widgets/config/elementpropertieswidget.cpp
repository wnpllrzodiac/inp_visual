#include "elementpropertieswidget.h"
#include "inputdelegate.h"
#include <QColorDialog>
#include <QComboBox>
#include <QDoubleValidator>
#include <QEvent>
#include <QFile>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

ElementPropertiesWidget::ElementPropertiesWidget(QWidget *parent)
: QWidget(parent)
{
    initUi();
    setupConnections();

    // 创建并设置验证delegate
    validate_delegate_ = new InputValidationDelegate(this);
    color_delegate_ = new ColorDelegate(this);
    combobox_delegate_ = new ComboBoxDelegate(this);

    // 为不同行设置delegate
    ui_.properties_table->setItemDelegateForRow(0, combobox_delegate_);
    ui_.properties_table->setItemDelegateForRow(9, color_delegate_);
    // 为其他行设置数值验证delegate
    for (int i = 1; i < 9; ++i)
    {
        ui_.properties_table->setItemDelegateForRow(i, validate_delegate_);
    }
}

void ElementPropertiesWidget::initUi()
{
    auto main_layout = new QVBoxLayout(this);

    // 创建表格
    ui_.properties_table = new QTableWidget(this);
    ui_.properties_table->setRowCount(10);   // 10个属性
    ui_.properties_table->setColumnCount(1); // 初始只有一个单元

    // 设置滚动条策略
    ui_.properties_table->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui_.properties_table->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // 固定表格大小，启用滚动
    ui_.properties_table->setFixedWidth(400); // 设置一个合适的固定宽度

    setupTableHeaders();

    // 按钮布局
    auto button_layout = new QHBoxLayout();
    ui_.add_button = new QPushButton(tr("Add Element"), this);
    ui_.remove_button = new QPushButton(tr("Remove Element"), this);
    button_layout->addWidget(ui_.add_button);
    button_layout->addWidget(ui_.remove_button);
    button_layout->addStretch();

    main_layout->addWidget(ui_.properties_table);
    main_layout->addLayout(button_layout);

    setLayout(main_layout);
}

void ElementPropertiesWidget::setupTableHeaders()
{
    QStringList headers;
    headers << tr("Element 1");
    ui_.properties_table->setHorizontalHeaderLabels(headers);

    // 设置垂直表头（属性名称和单位分两行）
    QStringList row_headers;
    row_headers << tr("Element ID")                        // 无单位
                << tr("Element Type")                      // 无单位
                << tr("Young's Modulus\n(ugr/(mm·m·ms²))") // 换行显示单位
                << tr("Poisson's Ratio\n(-)")              // 无量纲，显示(-)
                << tr("Friction Coefficient\n(-)")         // 无量纲，显示(-)
                << tr("Density\n(ugr/mm³)") << tr("Viscous Damping\n(ugr/(mm·ms))")
                << tr("Normal Stiffness\n(ugr/(ms²))") << tr("Tangential Stiffness\n(ugr/(mm·m·ms²))")
                << tr("Color"); // 无单位

    ui_.properties_table->setVerticalHeaderLabels(row_headers);

    // 设置表头和单元格大小
    ui_.properties_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui_.properties_table->horizontalHeader()->setDefaultSectionSize(120);
    ui_.properties_table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui_.properties_table->verticalHeader()->setDefaultSectionSize(45); // 增加行高以适应两行文本
    ui_.properties_table->verticalHeader()->setMinimumWidth(100);

    // 设置文本对齐和换行
    for (int i = 0; i < ui_.properties_table->rowCount(); ++i)
    {
        auto header = ui_.properties_table->verticalHeaderItem(i);
        if (header)
        {
            header->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        }
    }
}

void ElementPropertiesWidget::setupConnections()
{
    connect(ui_.add_button, &QPushButton::clicked, this, &ElementPropertiesWidget::addNewElement);
    connect(ui_.remove_button, &QPushButton::clicked, this, &ElementPropertiesWidget::removeSelectedElement);
}

void ElementPropertiesWidget::addNewElement()
{
    const int col = ui_.properties_table->columnCount();
    ui_.properties_table->setColumnCount(col + 1);

    // 设置新列的表头
    ui_.properties_table->setHorizontalHeaderItem(col, new QTableWidgetItem(tr("Element %1").arg(col + 1)));

    // Element ID (使用ComboBox)
    auto id_item = new QTableWidgetItem(QString::number(col));
    ui_.properties_table->setItem(0, col, id_item);

    // 属性类型 (下拉框)
    auto type_combo = new QComboBox();
    type_combo->addItem(tr("5 - Plane Stress"));
    ui_.properties_table->setCellWidget(1, col, type_combo);

    // 杨氏模量
    auto youngs_item = new QTableWidgetItem("3.53e+10");
    ui_.properties_table->setItem(2, col, youngs_item);

    // 泊松比
    auto poisson_item = new QTableWidgetItem("0.24");
    ui_.properties_table->setItem(3, col, poisson_item);

    // 摩擦系数
    auto friction_item = new QTableWidgetItem("1.27");
    ui_.properties_table->setItem(4, col, friction_item);

    // 密度
    auto density_item = new QTableWidgetItem("2537");
    ui_.properties_table->setItem(5, col, density_item);

    // 粘滞阻尼
    auto damping_item = new QTableWidgetItem("2.37e+07");
    ui_.properties_table->setItem(6, col, damping_item);

    // 接触刚度
    auto normal_stiffness_item = new QTableWidgetItem("3.53e+11");
    ui_.properties_table->setItem(7, col, normal_stiffness_item);

    // 切向刚度
    auto tangential_stiffness_item = new QTableWidgetItem("3.53e+11");
    ui_.properties_table->setItem(8, col, tangential_stiffness_item);

    // 颜色
    auto color_item = new QTableWidgetItem();
    color_item->setData(Qt::BackgroundRole, QColor(Qt::blue));
    ui_.properties_table->setItem(9, col, color_item);
}

void ElementPropertiesWidget::removeSelectedElement()
{
    const auto selected_cols = ui_.properties_table->selectionModel()->selectedColumns();
    if (!selected_cols.isEmpty())
    {
        ui_.properties_table->removeColumn(selected_cols.first().column());

        // 更新剩余列的表头和ID
        for (int col = 0; col < ui_.properties_table->columnCount(); ++col)
        {
            ui_.properties_table->setHorizontalHeaderItem(col, new QTableWidgetItem(tr("Element %1").arg(col + 1)));

            // 更新ID
            auto id_item = new QTableWidgetItem(QString::number(col));
            id_item->setFlags(id_item->flags() & ~Qt::ItemIsEditable);
            ui_.properties_table->setItem(0, col, id_item);
        }
    }
}