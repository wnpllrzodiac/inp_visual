#pragma once

#include <QWidget>

class QComboBox;
class QLabel;

class ProjectModel;

class UnitWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UnitWidget(QWidget *parent = nullptr, ProjectModel *project_model = nullptr);

private slots:
    void onLengthUnitChanged(int index);
    void onMassUnitChanged(int index);
    void onTimeUnitChanged(int index);

private:
    void initUi();
    void setupConnections();

    struct
    {
        QComboBox *length_combo;
        QComboBox *mass_combo;
        QComboBox *time_combo;
    } ui_;

    ProjectModel *project_model_;
};
