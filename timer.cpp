#include "timer.h"
#include "mainwindow.h"
#include "monitor.h"
#include <QTimerEvent>
#include <QDebug>
Timer::Timer(QObject *parent, MainWindow *ui)
    : QTimer(parent)
{
    myUi = ui;
    myMonitor = new Monitor();
//    connect(this, SIGNAL(timeout()), this, SLOT(update()));
//    this->start(1000);
}
Timer::~Timer()
{

}
void Timer::update()
{
    myUi->onChange(myMonitor->getCPUUsage());
}
