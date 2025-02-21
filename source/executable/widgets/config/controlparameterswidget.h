#pragma once

#include <QWidget>

class QLabel;
class QLineEdit;

class ControlParametersWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ControlParametersWidget(QWidget *parent = nullptr);

private:
    void initUi();
    void setupConnections();

    struct
    {
        QLineEdit *time_steps;
        QLineEdit *output_frequency;
        QLineEdit *min_element_size;
        QLineEdit *time_step;
        QLineEdit *gravity;
    } ui_;
};
