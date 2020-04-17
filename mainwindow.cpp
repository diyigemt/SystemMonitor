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
    ui->statusbar->setStyleSheet("color: rgb(255,0,0)");
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
    ui->statusbar->clearMessage(); //删除提示信息
    if(this->myMonitor->Update() == -1) // 使用之前先更新
    {
        ui->statusbar->showMessage("get data fialed!");
    } else
    {
        updateCPUUsage();
        updateMemoryUsage();
        updateDiskReadAndWriteSpeed();
    }
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
    if(CPUUsage == ui->CPUUsageprogressBar->value())
    {
        ui->statusbar->showMessage("get CPU Usage failed!");
    } else {
        ui->CPUUsageprogressBar->setValue(CPUUsage);
    }
}

void MainWindow::updateMemoryUsage()
{
    int memoryUsage = this->myMonitor->GetMemoryUsage();
    qDebug()<<memoryUsage<<endl;
    if(memoryUsage == ui->MemoryUsageprogressBar->value())
    {
        ui->statusbar->showMessage("get Memory Usage failed!");
    } else {
        ui->MemoryUsageprogressBar->setValue(memoryUsage);
    }
}

void MainWindow::updateDiskReadAndWriteSpeed()
{
    //bit to KB
    double readSpeed = this->myMonitor->GetDiskReadSpeed() / 1024;
    double writeSpeed = this->myMonitor->GetDiskWriteSpeed() / 1024;
    QString s;

    if(readSpeed > 1024)
    {
        readSpeed = readSpeed / 1024;
        s = "Read: " + QString::number(readSpeed, 'f', 2) + "MB/s";
    } else
    {
        s = "Read: " + QString::number(readSpeed, 'f', 2) + "KB/s";
    }
    if(s == ui->DiskReadLabel->text())
    {
        ui->statusbar->showMessage("get disk read speed failed!");
    } else {
        ui->DiskReadLabel->setText(s);
    }

    if(writeSpeed > 1024)
    {
        writeSpeed = writeSpeed / 1024;
        s = "Write: " + QString::number(writeSpeed, 'f', 2) + "MB/s";
    } else
    {
        s = "Write: " + QString::number(writeSpeed, 'f', 2) + "KB/s";
    }
    if(s == ui->DiskWriteLabel->text())
    {
        ui->statusbar->showMessage("get disk write speed failed!");
    } else {
        ui->DiskWriteLabel->setText(s);
    }
}
