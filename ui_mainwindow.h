/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *CPULayout;
    QLabel *CUPUsageLabe;
    QProgressBar *CPUUsageprogressBar;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *ControlLayout;
    QPushButton *pushButton_2;
    QPushButton *stopBtn;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayoutWidget = new QWidget(centralwidget);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 10, 391, 61));
        CPULayout = new QHBoxLayout(horizontalLayoutWidget);
        CPULayout->setObjectName(QString::fromUtf8("CPULayout"));
        CPULayout->setContentsMargins(0, 0, 0, 0);
        CUPUsageLabe = new QLabel(horizontalLayoutWidget);
        CUPUsageLabe->setObjectName(QString::fromUtf8("CUPUsageLabe"));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(11);
        CUPUsageLabe->setFont(font);

        CPULayout->addWidget(CUPUsageLabe);

        CPUUsageprogressBar = new QProgressBar(horizontalLayoutWidget);
        CPUUsageprogressBar->setObjectName(QString::fromUtf8("CPUUsageprogressBar"));
        CPUUsageprogressBar->setValue(0);

        CPULayout->addWidget(CPUUsageprogressBar);

        horizontalLayoutWidget_2 = new QWidget(centralwidget);
        horizontalLayoutWidget_2->setObjectName(QString::fromUtf8("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(410, 10, 194, 61));
        ControlLayout = new QHBoxLayout(horizontalLayoutWidget_2);
        ControlLayout->setObjectName(QString::fromUtf8("ControlLayout"));
        ControlLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_2 = new QPushButton(horizontalLayoutWidget_2);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        ControlLayout->addWidget(pushButton_2);

        stopBtn = new QPushButton(horizontalLayoutWidget_2);
        stopBtn->setObjectName(QString::fromUtf8("stopBtn"));

        ControlLayout->addWidget(stopBtn);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);
        QObject::connect(pushButton_2, SIGNAL(clicked()), MainWindow, SLOT(startMonitor()));
        QObject::connect(stopBtn, SIGNAL(clicked()), MainWindow, SLOT(stopMonitor()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        CUPUsageLabe->setText(QCoreApplication::translate("MainWindow", "CPU Usage:", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "\345\274\200\345\247\213\347\233\221\346\216\247", nullptr));
        stopBtn->setText(QCoreApplication::translate("MainWindow", "\345\201\234\346\255\242\347\233\221\346\216\247", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
