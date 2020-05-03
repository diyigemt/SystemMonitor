#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <QDebug>
#include <QWidget>
#include <QMap>
#include "monitor.h"
#include "constants.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusbar->setStyleSheet("color: rgb(255,0,0)");
    myMonitor = new Monitor();
    myTimer = new QTimer();
    myMap = new QMap<QString, QWidget*>();
    diskCount = 0;
    QObject::connect(myTimer, &QTimer::timeout, this, &MainWindow::update);
    startMonitor();
    addDisk("123123123");
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
    resetAll();
}

void MainWindow::update()
{
    ui->statusbar->clearMessage(); //删除提示信息
    unsigned int status = this->myMonitor->Update();
    if(status == GET_DATA_FAILURE)
    {
        ui->statusbar->showMessage("get data failed!");
        return;
    }
    if(status == GET_CPUUSAGE_FAILURE)
    {
        ui->statusbar->showMessage("get CPU Usage failed!");
    } else if(status == GET_MEMORY_FAILURE)
    {
        ui->statusbar->showMessage("get Memory Usage failed!");
    } else if(status == GET_DISK_R_FAILURE)
    {
        ui->statusbar->showMessage("get disk read speed failed!");
    } else if(status == GET_DISK_W_FAILURE)
    {
        ui->statusbar->showMessage("get disk write speed failed!");
    }
    updateCPUUsage();
    updateMemoryUsage();
    updateDiskReadAndWriteSpeed();
    updateSystemMemory();
}

void MainWindow::resetAll()
{
    ui->CPUUsageprogressBar->setValue(0);
    ui->MemoryUsageprogressBar->setValue(0);
    ui->DiskReadLabel->setText("Read: 0.00KB/s");
    ui->DiskWriteLabel->setText("Write: 0.00KB/s");
}

void MainWindow::updateCPUUsage()
{
    int CPUUsage = this->myMonitor->GetCPUUsage();
    if(CPUUsage == ui->CPUUsageprogressBar->value())
    {

    } else {
        ui->CPUUsageprogressBar->setValue(CPUUsage);
    }
}

void MainWindow::updateMemoryUsage()
{
    int memoryUsage = this->myMonitor->GetMemoryUsage();
    qDebug()<<memoryUsage<<endl;
    ui->MemoryUsageprogressBar->setValue(memoryUsage);
}

void MainWindow::updateSystemMemory()
{
    int SystemMemory = (int)(this->myMonitor->GetSystemMemory()/1024/1024);
    ui->TotalMemoryLabel->setText("System Memory:" + QString::number(SystemMemory) + "M");
}
void MainWindow::updateDiskReadAndWriteSpeed()
{
    //Byte to KB
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
    ui->DiskReadLabel->setText(s);

    if(writeSpeed > 1024)
    {
        writeSpeed = writeSpeed / 1024;
        s = "Write: " + QString::number(writeSpeed, 'f', 2) + "MB/s";
    } else
    {
        s = "Write: " + QString::number(writeSpeed, 'f', 2) + "KB/s";
    }
    ui->DiskWriteLabel->setText(s);
}

void MainWindow::addDisk(QString id)
{
    Disk* disk = new Disk(id);
    disk->addPartition("C:", 100.1f, 200.2f);
    int row = diskCount / 2;
    int column = diskCount - row;
    ui->DiskLayout->addLayout(disk, row, column, 1, 1);
    diskCount++;
}
