#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "timer.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    myTimer = new Timer(nullptr, this);
    QObject::connect(myTimer, &QTimer::timeout, myTimer, &Timer::update);
    myTimer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onChange(QString s)
{
    ui->label->setText(s);
}
