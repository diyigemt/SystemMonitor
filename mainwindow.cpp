#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include "monitor.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    myMonitor = new Monitor();
    myTimer = new QTimer();
    QObject::connect(myTimer, &QTimer::timeout, this, &MainWindow::update);
    myTimer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update()
{
    updateCPUUsage();
}
void MainWindow::updateCPUUsage()
{
    ui->label->setText(myMonitor->getCPUUsage());
}
