#pragma once

#include <QWidget>

class QTableWidget;
class QPushButton;
class InputValidationDelegate;
class ColorDelegate;
class ComboBoxDelegate;

class ProjectModel;

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
    explicit BoundaryConditionsWidget(QWidget *parent = nullptr, ProjectModel *project_model = nullptr);

private:
    void initUi();
    void setupConnections();
    void setupTableHeaders();
    void setupDelegates();
    void updateNodeSetItems();

private slots:
    void addNewNode();
    void removeSelectedNode();
    void onCellChanged(int row, int column);
    void onNodeSetsChanged();

private:
    BoundaryConditionsWidgetUi ui_;
    InputValidationDelegate *validate_delegate_;
    ColorDelegate *color_delegate_;
    ComboBoxDelegate *combobox_delegate_;

    ProjectModel *project_model_;
};