#ifndef TIMER_H
#define TIMER_H

#include <QTimer>
#include "monitor.h"
#include "mainwindow.h"
class Timer : public QTimer
{
    Q_OBJECT
public:
    Timer(QObject *parent = nullptr, MainWindow *ui = nullptr);
    ~Timer();
public slots:
    void update();
private:
    MainWindow *myUi;
    Monitor *myMonitor;
};

#endif // TIMER_H
