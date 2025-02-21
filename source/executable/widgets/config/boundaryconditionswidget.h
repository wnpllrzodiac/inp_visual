#pragma once

#include <QWidget>

class QTableWidget;
class QPushButton;
class InputValidationDelegate;
class ColorDelegate;
class ComboBoxDelegate;

struct BoundaryConditionsWidgetUi
{
    QTableWidget *properties_table;
    QPushButton *add_button;
    QPushButton *remove_button;
};

class BoundaryConditionsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BoundaryConditionsWidget(QWidget *parent = nullptr);

private:
    void initUi();
    void setupConnections();
    void setupTableHeaders();

private slots:
    void addNewNode();
    void removeSelectedNode();

private:
    BoundaryConditionsWidgetUi ui_;
    InputValidationDelegate *validate_delegate_;
    ColorDelegate *color_delegate_;
    ComboBoxDelegate *combobox_delegate_;
};