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
    diskMap = new QMap<QString, Disk*>();
    diskCount = 0;
    QObject::connect(myTimer, &QTimer::timeout, this, &MainWindow::update);
    startMonitor();

    ui->CPUTypeLabel->setText("CPU Type:" + myMonitor->GetCPUID());
    ui->OSTypeLable->setText("OS:" + myMonitor->GetOSVersion());

    for(int i = 0; i < 4; i++)
    {
        QLabel* label = new QLabel();
        ui->DiskLayout->addWidget(label, i / 2, i % 2, 1, 1);
    }

    int count=myMonitor->GetdiskCount();
    for (int i = 0; i < count; i++)
    {
        addDisk(myMonitor->GetDriveID(i));
        QList<partition> *partitionList=new QList<partition>();
        myMonitor->getPartitonList(i,partitionList);
        for(int j=0;j<=(*partitionList).length()-1;j++)
        {
            addPartition(myMonitor->GetDriveID(i),(*partitionList)[j].name,(*partitionList)[j].free,(*partitionList)[j].size);
        }
        delete partitionList;
    }
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

    //更新分卷
//    int diskcount=this->myMonitor->GetdiskCount();
//    for (int i = 0; i < diskcount; i++)
//    {
//        QList<partition> *partitionList=new QList<partition>();
//        this->myMonitor->getPartitonList(i,partitionList);
//        for(int j=0;j<=(*partitionList).length()-1;j++)
//        {
//            setPartition(this->myMonitor->GetDriveID(i),(*partitionList)[j].name,(*partitionList)[j].free,(*partitionList)[j].size);
//        }
//        delete partitionList;
//    }
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
    if(CPUUsage != ui->CPUUsageprogressBar->value())
    {
        ui->CPUUsageprogressBar->setValue(CPUUsage);
    }
}

void MainWindow::updateMemoryUsage()
{
    int memoryUsage = this->myMonitor->GetMemoryUsage();
//    qDebug()<<memoryUsage<<endl;
    ui->MemoryUsageprogressBar->setValue(memoryUsage);
}

void MainWindow::updateSystemMemory()
{
    int SystemMemory = (int)(this->myMonitor->GetSystemMemory()/1024/1024);
    ui->TotalMemoryLabel->setText("System Memory:" + QString::number(SystemMemory) + "MB");
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

/**
 * @brief MainWindow::addDisk 添加一个物理磁盘信息
 * @param id 物理磁盘ID,用于更新和显示使用率，务必保持一致
 */
void MainWindow::addDisk(QString id)
{
    Disk* disk = new Disk(id);
    diskMap->insert(id, disk);
    ui->DiskLayout->addLayout(disk, diskCount / 2, diskCount % 2, 1, 1);
    diskCount++;
}

/**
 * @brief MainWindow::addPartition 给物理磁盘添加分卷
 * @param diskId 磁盘ID,!!必须与创建磁盘时的ID保持一致!!
 * @param name 分卷名,用于标识分卷及更新分卷信息
 * @param usage 分卷当前使用空间
 * @param total 分卷总可用空间
 */
void MainWindow::addPartition(QString diskId, QString name, float usage, float total)
{
    Disk* disk = diskMap->find(diskId).value();
    if(disk != nullptr)
    {
        disk->addPartition(name, usage, total);
    }
}

/**
 * @brief MainWindow::setPartition 设置/更新分卷信息
 * @param diskId 磁盘ID,!!必须与创建磁盘时的ID保持一致!!
 * @param name 分卷名,!!必须与创建分卷时的名保持一致!!
 * @param usage 分卷当前使用空间
 * @param total 分卷总可用空间
 */
void MainWindow::setPartition(QString diskId, QString name, float usage, float total)
{
    Disk* disk = diskMap->find(diskId).value();
    if(disk != nullptr)
    {
        disk->setPartition(name, usage, total);
    }
}
