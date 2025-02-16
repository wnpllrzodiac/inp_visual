#pragma once

#include <QObject>

class SignalEmitter : public QObject
{
    Q_OBJECT
public:
    explicit SignalEmitter(QObject *parent = nullptr)
    : QObject(parent)
    {
    }

signals:
    void cellSelected(size_t cellId, int x, int y);
    void pointSelected(size_t pointId, int x, int y);
};