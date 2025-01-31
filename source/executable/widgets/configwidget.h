#ifndef CONFIGWIDGET_H
#define CONFIGWIDGET_H

#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>


class ConfigWidget : public QWidget {
    Q_OBJECT
public:
    explicit ConfigWidget(QWidget* parent = nullptr);

private:
    void initUi();
    void setupConnections();

private:
    QLineEdit* lengthUnitEdit;
    QLineEdit* massUnitEdit;
    QLineEdit* timeUnitEdit;
};

#endif // CONFIGWIDGET_H
