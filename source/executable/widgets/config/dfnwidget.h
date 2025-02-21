#pragma once

#include <QWidget>

class DFNWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DFNWidget(QWidget *parent = nullptr);

private:
    void initUi();
    void setupConnections();
};