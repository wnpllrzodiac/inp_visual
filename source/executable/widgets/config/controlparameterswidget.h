#pragma once

#include <QWidget>

class QLabel;
class QLineEdit;

class ProjectModel;

class ControlParametersWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ControlParametersWidget(QWidget *parent = nullptr, ProjectModel *project_model = nullptr);

private slots:
    void onTimeStepsChanged(const QString &text);
    void onOutputFrequencyChanged(const QString &text);
    void onMinElementSizeChanged(const QString &text);
    void onTimeStepChanged(const QString &text);
    void onGravityChanged(const QString &text);

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

    ProjectModel *project_model_;
};
