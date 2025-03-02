#pragma once

#include "inputdelegate.h"
#include <QWidget>

class QTableWidget;
class QPushButton;

class ProjectModel;

class ElementPropertiesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ElementPropertiesWidget(QWidget *parent = nullptr, ProjectModel *project_model = nullptr);

private slots:
    void onCellChanged(int row, int column);
    void onElementSetsChanged();

protected:
private:
    void initUi();
    void setupConnections();
    void setupTableHeaders();
    void setupDelegates();
    void updateElementSetItems();
    void addNewElement();
    void removeSelectedElement();
    void showColorDialog(int row, int column);

    struct
    {
        QTableWidget *properties_table;
        QPushButton *add_button;
        QPushButton *remove_button;
    } ui_;

    InputValidationDelegate *validate_delegate_;
    ColorDelegate *color_delegate_;
    ComboBoxDelegate *combobox_delegate_;

    ProjectModel *project_model_;
};
