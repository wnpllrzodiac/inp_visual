#include "boundaryconditionswidget.h"
#include "inputdelegate.h"
#include "projectmodel.h"
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFile>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSignalBlocker>
#include <QTableWidget>
#include <QVBoxLayout>

BoundaryConditionsWidget::BoundaryConditionsWidget(QWidget *parent, ProjectModel *project_model)
: QWidget(parent)
, project_model_(project_model)
{
    initUi();
    setupConnections();
    setupDelegates();
}

void BoundaryConditionsWidget::initUi()
{
    auto main_layout = new QVBoxLayout(this);

    // 创建表格
    ui_.properties_table = new QTableWidget(this);
    ui_.properties_table->setRowCount(8); // 8个属性

    // 设置滚动条策略
    ui_.properties_table->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui_.properties_table->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // 固定表格大小
    // ui_.properties_table->setFixedWidth(400);

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
    connect(ui_.properties_table, &QTableWidget::cellChanged, this, &BoundaryConditionsWidget::onCellChanged);
    connect(project_model_, &ProjectModel::nodeSetsChanged, this, &BoundaryConditionsWidget::onNodeSetsChanged);
}

void BoundaryConditionsWidget::setupDelegates()
{
    // 创建并设置验证delegate
    validate_delegate_ = new InputValidationDelegate(this);
    color_delegate_ = new ColorDelegate(this);
    combobox_delegate_ = new ComboBoxDelegate(this);

    // 更新节点集列表
    updateNodeSetItems();

    // 为不同行设置delegate
    // ui_.properties_table->setItemDelegateForRow(0, combobox_delegate_);
    ui_.properties_table->setItemDelegateForRow(1, combobox_delegate_);
    ui_.properties_table->setItemDelegateForRow(3, combobox_delegate_);
    ui_.properties_table->setItemDelegateForRow(7, color_delegate_);

    // 为数值输入行设置验证delegate
    for (int row : {2, 4, 5, 6})
    {
        ui_.properties_table->setItemDelegateForRow(row, validate_delegate_);
    }
}

void BoundaryConditionsWidget::updateNodeSetItems()
{
    // QStringList items;
    // for (const auto &set : project_model_->nodeSets().keys())
    // {
    //     items << set;
    // }
    // combobox_delegate_->setItems(items);
}

void BoundaryConditionsWidget::addNewNode()
{
    // 检查是否有可用的节点集
    if (project_model_->nodeSets().isEmpty())
    {
        QMessageBox::warning(this, tr("警告"), tr("没有可用的节点集。请先创建节点集。"));
        return;
    }

    // 创建对话框
    QDialog dialog(this);
    dialog.setWindowTitle(tr("添加新节点"));
    dialog.setMinimumWidth(300);

    // 创建布局
    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // 添加标签
    QLabel *label = new QLabel(tr("选择节点集:"), &dialog);
    layout->addWidget(label);

    // 创建下拉框
    QComboBox *comboBox = new QComboBox(&dialog);
    for (const auto &set : project_model_->nodeSets().keys())
    {
        comboBox->addItem(set);
    }
    layout->addWidget(comboBox);

    // 添加按钮
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout->addWidget(buttonBox);

    // 显示对话框
    if (dialog.exec() != QDialog::Accepted)
    {
        return; // 用户取消了操作
    }

    // 获取选择的节点集
    QString selectedNodeSet = comboBox->currentText();
    if (selectedNodeSet.isEmpty())
    {
        return;
    }

    const QSignalBlocker blocker(ui_.properties_table);
    const int col = ui_.properties_table->columnCount();

    if (col >= project_model_->nodeSets().size())
    {
        return;
    }

    ui_.properties_table->setColumnCount(col + 1);
    ui_.properties_table->setHorizontalHeaderItem(col, new QTableWidgetItem(tr("Node %1").arg(col + 1)));

    // 创建新的边界条件
    BoundaryCondition newCondition;
    newCondition.id = selectedNodeSet; // 设置为选择的节点集ID

    // 节点属性
    auto id_item = new QTableWidgetItem();
    id_item->setData(Qt::EditRole, selectedNodeSet);
    id_item->setFlags(id_item->flags() & ~Qt::ItemIsEditable); // 设置为只读
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
    color_item->setData(Qt::BackgroundRole, QColor(Qt::black));
    ui_.properties_table->setItem(7, col, color_item);
    newCondition.color = QColor(Qt::black);

    // 添加到模型
    project_model_->addBoundaryCondition(newCondition);
}

void BoundaryConditionsWidget::removeSelectedNode()
{
    const auto selected_cols = ui_.properties_table->selectionModel()->selectedColumns();
    if (!selected_cols.isEmpty())
    {
        int col = selected_cols.first().column();

        // 获取要删除的边界条件ID
        QTableWidgetItem *idItem = ui_.properties_table->item(0, col);
        if (idItem)
        {
            QString boundary_id = idItem->text();
            project_model_->removeBoundaryCondition(boundary_id);
        }

        ui_.properties_table->removeColumn(col);

        // 更新剩余列的表头
        for (int i = 0; i < ui_.properties_table->columnCount(); ++i)
        {
            ui_.properties_table->setHorizontalHeaderItem(i, new QTableWidgetItem(tr("Node %1").arg(i + 1)));
        }
    }
}

void BoundaryConditionsWidget::onCellChanged(int row, int column)
{
    if (row < 0 || column < 0 || column >= ui_.properties_table->columnCount())
        return;

    // 获取当前节点的ID
    QTableWidgetItem *idItem = ui_.properties_table->item(0, column);
    if (!idItem)
        return;

    QString boundary_id = idItem->text();

    // 确保索引有效
    if (column >= project_model_->boundaryConditions().size())
        return;

    // 获取当前边界条件
    BoundaryCondition condition = project_model_->boundaryConditions()[column];

    QTableWidgetItem *item = ui_.properties_table->item(row, column);
    if (!item)
        return;

    QString value = item->text();

    // 使用switch更新属性
    switch (row)
    {
    case 0:
        condition.id = value;
        break;
    case 1:
        condition.x_direction_type = value.toInt();
        break;
    case 2:
        condition.x_direction_size = value.toDouble();
        break;
    case 3:
        condition.y_direction_type = value.toInt();
        break;
    case 4:
        condition.y_direction_size = value.toDouble();
        break;
    case 5:
        condition.unit_surface_force = value.toDouble();
        break;
    case 6:
        condition.unit_shear_force = value.toDouble();
        break;
    case 7:
        {
            auto color = item->data(Qt::BackgroundRole).value<QColor>();
            condition.color = color;
            break;
        }
    default:
        return;
    }

    // 更新项目模型中的边界条件
    project_model_->updateBoundaryCondition(boundary_id, condition);
}

void BoundaryConditionsWidget::onNodeSetsChanged()
{
    // 更新节点集列表
    updateNodeSetItems();
}