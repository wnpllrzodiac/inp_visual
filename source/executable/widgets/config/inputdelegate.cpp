#include "inputdelegate.h"
#include <QColorDialog>
#include <QComboBox>
#include <QDoubleValidator>
#include <QLineEdit>
#include <QMouseEvent>
#include <QPainter>

InputValidationDelegate::InputValidationDelegate(QObject *parent)
: QStyledItemDelegate(parent)
{
}

QWidget *InputValidationDelegate::createEditor(QWidget *parent,
                                               const QStyleOptionViewItem &option,
                                               const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    QLineEdit *editor = new QLineEdit(parent);

    // 使用QDoubleValidator进行浮点数验证
    // 设置范围为-999999.0到999999.0，小数点后2位
    QDoubleValidator *validator = new QDoubleValidator(-999999.0, 999999.0, 2, editor);
    validator->setNotation(QDoubleValidator::StandardNotation);
    editor->setValidator(validator);

    return editor;
}

void InputValidationDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();
    QLineEdit *lineEdit = static_cast<QLineEdit *>(editor);
    lineEdit->setText(value);
}

void InputValidationDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *lineEdit = static_cast<QLineEdit *>(editor);
    QString value = lineEdit->text();
    model->setData(index, value, Qt::EditRole);
}

void InputValidationDelegate::updateEditorGeometry(QWidget *editor,
                                                   const QStyleOptionViewItem &option,
                                                   const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

ColorDelegate::ColorDelegate(QObject *parent)
: QStyledItemDelegate(parent)
{
}

void ColorDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QColor color = index.data(Qt::BackgroundRole).value<QColor>();

    painter->save();

    // 绘制选中状态的背景
    if (option.state & QStyle::State_Selected)
    {
        painter->fillRect(option.rect, option.palette.highlight());
    }

    // 绘制颜色方块
    QRect colorRect = option.rect.adjusted(4, 4, -4, -4);
    painter->fillRect(colorRect, color);
    painter->setPen(Qt::black);
    painter->drawRect(colorRect);

    painter->restore();
}

QWidget *
ColorDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(parent);
    Q_UNUSED(option);
    Q_UNUSED(index);
    return nullptr; // 不创建编辑器，使用双击事件处理
}

void ColorDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    Q_UNUSED(editor);
    Q_UNUSED(index);
}

void ColorDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    Q_UNUSED(editor);
    Q_UNUSED(model);
    Q_UNUSED(index);
}

bool ColorDelegate::editorEvent(QEvent *event,
                                QAbstractItemModel *model,
                                const QStyleOptionViewItem &option,
                                const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonDblClick)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->button() == Qt::LeftButton)
        {
            QColor currentColor = index.data(Qt::BackgroundRole).value<QColor>();
            QColor newColor =
                QColorDialog::getColor(currentColor, nullptr, tr("Select Color"), QColorDialog::ShowAlphaChannel);

            if (newColor.isValid())
            {
                model->setData(index, newColor, Qt::BackgroundRole);
                return true;
            }
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

ComboBoxDelegate::ComboBoxDelegate(QObject *parent)
: QStyledItemDelegate(parent)
{
}

QWidget *
ComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    QComboBox *editor = new QComboBox(parent);
    // 添加0到9的选项
    for (int i = 0; i < 10; ++i)
    {
        editor->addItem(QString::number(i));
    }
    return editor;
}

void ComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();
    QComboBox *comboBox = static_cast<QComboBox *>(editor);
    comboBox->setCurrentText(value);
}

void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox *>(editor);
    model->setData(index, comboBox->currentText(), Qt::EditRole);
}

void ComboBoxDelegate::updateEditorGeometry(QWidget *editor,
                                            const QStyleOptionViewItem &option,
                                            const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}