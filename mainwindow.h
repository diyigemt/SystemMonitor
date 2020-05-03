#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QWidget>
#include <disk.h>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow;}
QT_END_NAMESPACE
class Monitor;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void startMonitor();
    void stopMonitor();
    void update();
private:
    void resetAll();
    void updateCPUUsage();
    void updateMemoryUsage();
    void updateDiskReadAndWriteSpeed();
    void addDisk(QString id);
    Ui::MainWindow *ui;
    Monitor *myMonitor;
    QTimer *myTimer;
    QMap<QString, QWidget*> *myMap;
    int diskCount;

};
#endif // MAINWINDOW_H
