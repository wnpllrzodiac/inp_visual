#include "boundaryconditionswidget.h"
#include "inputdelegate.h"
#include <QComboBox>
#include <QFile>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

BoundaryConditionsWidget::BoundaryConditionsWidget(QWidget *parent)
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
    ui_.properties_table->setItemDelegateForRow(1, combobox_delegate_);
    ui_.properties_table->setItemDelegateForRow(3, combobox_delegate_);
    ui_.properties_table->setItemDelegateForRow(7, color_delegate_);

    // 为数值输入行设置验证delegate
    for (int row : {2, 4, 5, 6})
    {
        ui_.properties_table->setItemDelegateForRow(row, validate_delegate_);
    }
}

void BoundaryConditionsWidget::initUi()
{
    auto main_layout = new QVBoxLayout(this);

    // 创建表格
    ui_.properties_table = new QTableWidget(this);
    ui_.properties_table->setRowCount(8);    // 8个属性
    ui_.properties_table->setColumnCount(1); // 初始只有一个节点

    // 设置滚动条策略
    ui_.properties_table->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui_.properties_table->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // 固定表格大小
    ui_.properties_table->setFixedWidth(400);

    setupTableHeaders();

    // 按钮布局
    auto button_layout = new QHBoxLayout();
    ui_.add_button = new QPushButton(tr("添加节点"), this);
    ui_.remove_button = new QPushButton(tr("删除节点"), this);
    button_layout->addWidget(ui_.add_button);
    button_layout->addWidget(ui_.remove_button);
    button_layout->addStretch();

    main_layout->addWidget(ui_.properties_table);
    main_layout->addLayout(button_layout);
}

void BoundaryConditionsWidget::setupTableHeaders()
{
    QStringList headers;
    headers << tr("Node 1");
    ui_.properties_table->setHorizontalHeaderLabels(headers);

    QStringList row_headers;
    row_headers << tr("Node Attribute") << tr("X Direction Boundary Condition Type")
                << tr("X Direction Boundary Condition Size\n(ugr.mm/ms²)") << tr("Y Direction Boundary Condition Type")
                << tr("Y Direction Boundary Condition Size\n(ugr.mm/ms²)") << tr("Unit Surface Force\n(ugr/(mm.ms²))")
                << tr("Unit Shear Force\n(ugr/(mm.ms²))") << tr("Color");

    ui_.properties_table->setVerticalHeaderLabels(row_headers);

    // 设置表头和单元格大小
    ui_.properties_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui_.properties_table->horizontalHeader()->setDefaultSectionSize(120);
    ui_.properties_table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui_.properties_table->verticalHeader()->setDefaultSectionSize(45);
    ui_.properties_table->verticalHeader()->setMinimumWidth(150);

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

void BoundaryConditionsWidget::setupConnections()
{
    connect(ui_.add_button, &QPushButton::clicked, this, &BoundaryConditionsWidget::addNewNode);
    connect(ui_.remove_button, &QPushButton::clicked, this, &BoundaryConditionsWidget::removeSelectedNode);
}

void BoundaryConditionsWidget::addNewNode()
{
    const int col = ui_.properties_table->columnCount();
    ui_.properties_table->setColumnCount(col + 1);

    // 设置新列的表头
    ui_.properties_table->setHorizontalHeaderItem(col, new QTableWidgetItem(tr("Node %1").arg(col + 1)));

    // 节点属性
    auto id_item = new QTableWidgetItem("1");
    ui_.properties_table->setItem(0, col, id_item);

    // X方向边界条件类型
    auto x_type_item = new QTableWidgetItem(tr("3. Velocity"));
    ui_.properties_table->setItem(1, col, x_type_item);

    // X方向边界条件大小
    auto x_value_item = new QTableWidgetItem("0");
    ui_.properties_table->setItem(2, col, x_value_item);

    // Y方向边界条件类型
    auto y_type_item = new QTableWidgetItem(tr("3. Velocity"));
    ui_.properties_table->setItem(3, col, y_type_item);

    // Y方向边界条件大小
    auto y_value_item = new QTableWidgetItem("0");
    ui_.properties_table->setItem(4, col, y_value_item);

    // 单元面力
    auto stress_item = new QTableWidgetItem("0");
    ui_.properties_table->setItem(5, col, stress_item);

    // 单元剪引力
    auto shear_item = new QTableWidgetItem("0");
    ui_.properties_table->setItem(6, col, shear_item);

    // 颜色
    auto color_item = new QTableWidgetItem();
    color_item->setData(Qt::BackgroundRole, QColor(Qt::yellow));
    ui_.properties_table->setItem(7, col, color_item);
}

void BoundaryConditionsWidget::removeSelectedNode()
{
    const auto selected_cols = ui_.properties_table->selectionModel()->selectedColumns();
    if (!selected_cols.isEmpty())
    {
        ui_.properties_table->removeColumn(selected_cols.first().column());

        // 更新剩余列的表头
        for (int col = 0; col < ui_.properties_table->columnCount(); ++col)
        {
            ui_.properties_table->setHorizontalHeaderItem(col, new QTableWidgetItem(tr("Node %1").arg(col + 1)));
        }
    }
}