#pragma once
#include <QWidget>

class JointPropertiesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit JointPropertiesWidget(QWidget *parent = nullptr);

private:
    void initUi();
    void setupConnections();
};