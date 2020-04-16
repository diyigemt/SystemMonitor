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
    ui->CPUUsageprogressBar->setValue(this->myMonitor->GetCPUUsage());
}
