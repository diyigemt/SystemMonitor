#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <QDebug>
#include "monitor.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    myMonitor = new Monitor();
    myTimer = new QTimer();
    QObject::connect(myTimer, &QTimer::timeout, this, &MainWindow::update);
    startMonitor();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startMonitor()
{
    update();
    this->myTimer->start(1000);
}

void MainWindow::stopMonitor()
{
    this->myTimer->stop();
    ui->CPUUsageprogressBar->setValue(0);
}

void MainWindow::update()
{
    this->myMonitor->Update(); // 使用之前先更新
    updateCPUUsage();
}

void MainWindow::resetAll()
{
    ui->CPUUsageprogressBar->setValue(0);
}

void MainWindow::updateCPUUsage()
{
    int CPUUsage = this->myMonitor->GetCPUUsage();
    if(CPUUsage == -1)
    {
        ui->statusbar->setStatusTip("获取CPU利用率失败!");
    } else {
        ui->CPUUsageprogressBar->setValue(CPUUsage);
    }
}
