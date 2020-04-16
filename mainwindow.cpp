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
    updateMemoryUsage();
    updateDiskReadAndWriteSpeed();
}

void MainWindow::resetAll()
{
    ui->CPUUsageprogressBar->setValue(0);
    ui->MemoryUsageprogressBar->setValue(0);
    ui->DiskReadLabel->setText("Read:");
    ui->DiskWriteLabel->setText("Write:");
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

void MainWindow::updateMemoryUsage()
{
    int memoryUsage = this->myMonitor->GetMemoryUsage();
    if(memoryUsage == -1)
    {
        ui->statusbar->setStatusTip("获取内存占用率失败!");
    } else {
        ui->MemoryUsageprogressBar->setValue(memoryUsage);
    }
}

void MainWindow::updateDiskReadAndWriteSpeed()
{
    int readSpeed = this->myMonitor->GetDiskReadSpeed();
    int writeSpeed = this->myMonitor->GetDiskWriteSpeed();
    if(readSpeed == -1)
    {
        ui->statusbar->setStatusTip("获取磁盘读取速率失败!");
    } else {
        ui->DiskReadLabel->setText("Read:" + QString::number(readSpeed) + "kb/s");
    }
    if(writeSpeed == -1)
    {
        ui->statusbar->setStatusTip("获取磁盘写入速率失败!");
    } else {
        ui->DiskWriteLabel->setText("Write:" + QString::number(writeSpeed) + "kb/s");
    }
}
