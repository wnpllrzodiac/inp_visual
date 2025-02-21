#pragma once

#include <QWidget>

class QComboBox;
class QLabel;

class UnitWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UnitWidget(QWidget *parent = nullptr);

private:
    void initUi();
    void setupConnections();

    struct
    {
        QComboBox *length_combo;
        QComboBox *mass_combo;
        QComboBox *time_combo;
    } ui_;
};
