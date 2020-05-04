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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *ControlLayout;
    QPushButton *startBtn;
    QPushButton *stopBtn;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *CPUverticalLayout;
    QLabel *CPUTitleLabel;
    QLabel *CPUTypeLabel;
    QHBoxLayout *CPUUsageLayout;
    QLabel *CUPUsageLabel;
    QProgressBar *CPUUsageprogressBar;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *StorageverticalLayout;
    QLabel *StorageTitleLabel;
    QHBoxLayout *ReadAndWriteLayout;
    QLabel *DiskReadLabel;
    QLabel *DiskWriteLabel;
    QWidget *verticalLayoutWidget_3;
    QVBoxLayout *MemoryverticalLayout;
    QLabel *MemoryTitleLabel;
    QLabel *TotalMemoryLabel;
    QHBoxLayout *MemoryUsageLayout;
    QLabel *MemoryUsageLabe;
    QProgressBar *MemoryUsageprogressBar;
    QLabel *OSTypeLable;
    QWidget *gridLayoutWidget_3;
    QGridLayout *DiskLayout;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayoutWidget_2 = new QWidget(centralwidget);
        horizontalLayoutWidget_2->setObjectName(QString::fromUtf8("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(600, 480, 195, 61));
        ControlLayout = new QHBoxLayout(horizontalLayoutWidget_2);
        ControlLayout->setObjectName(QString::fromUtf8("ControlLayout"));
        ControlLayout->setContentsMargins(0, 0, 0, 0);
        startBtn = new QPushButton(horizontalLayoutWidget_2);
        startBtn->setObjectName(QString::fromUtf8("startBtn"));

        ControlLayout->addWidget(startBtn);

        stopBtn = new QPushButton(horizontalLayoutWidget_2);
        stopBtn->setObjectName(QString::fromUtf8("stopBtn"));

        ControlLayout->addWidget(stopBtn);

        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 40, 371, 111));
        CPUverticalLayout = new QVBoxLayout(verticalLayoutWidget);
        CPUverticalLayout->setObjectName(QString::fromUtf8("CPUverticalLayout"));
        CPUverticalLayout->setContentsMargins(0, 0, 0, 0);
        CPUTitleLabel = new QLabel(verticalLayoutWidget);
        CPUTitleLabel->setObjectName(QString::fromUtf8("CPUTitleLabel"));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(11);
        CPUTitleLabel->setFont(font);

        CPUverticalLayout->addWidget(CPUTitleLabel);

        CPUTypeLabel = new QLabel(verticalLayoutWidget);
        CPUTypeLabel->setObjectName(QString::fromUtf8("CPUTypeLabel"));
        CPUTypeLabel->setFont(font);

        CPUverticalLayout->addWidget(CPUTypeLabel);

        CPUUsageLayout = new QHBoxLayout();
        CPUUsageLayout->setObjectName(QString::fromUtf8("CPUUsageLayout"));
        CUPUsageLabel = new QLabel(verticalLayoutWidget);
        CUPUsageLabel->setObjectName(QString::fromUtf8("CUPUsageLabel"));
        CUPUsageLabel->setFont(font);

        CPUUsageLayout->addWidget(CUPUsageLabel);

        CPUUsageprogressBar = new QProgressBar(verticalLayoutWidget);
        CPUUsageprogressBar->setObjectName(QString::fromUtf8("CPUUsageprogressBar"));
        CPUUsageprogressBar->setValue(0);

        CPUUsageLayout->addWidget(CPUUsageprogressBar);


        CPUverticalLayout->addLayout(CPUUsageLayout);

        verticalLayoutWidget_2 = new QWidget(centralwidget);
        verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(10, 170, 781, 81));
        StorageverticalLayout = new QVBoxLayout(verticalLayoutWidget_2);
        StorageverticalLayout->setObjectName(QString::fromUtf8("StorageverticalLayout"));
        StorageverticalLayout->setContentsMargins(0, 0, 0, 0);
        StorageTitleLabel = new QLabel(verticalLayoutWidget_2);
        StorageTitleLabel->setObjectName(QString::fromUtf8("StorageTitleLabel"));
        StorageTitleLabel->setFont(font);

        StorageverticalLayout->addWidget(StorageTitleLabel);

        ReadAndWriteLayout = new QHBoxLayout();
        ReadAndWriteLayout->setObjectName(QString::fromUtf8("ReadAndWriteLayout"));
        DiskReadLabel = new QLabel(verticalLayoutWidget_2);
        DiskReadLabel->setObjectName(QString::fromUtf8("DiskReadLabel"));
        DiskReadLabel->setFont(font);

        ReadAndWriteLayout->addWidget(DiskReadLabel);

        DiskWriteLabel = new QLabel(verticalLayoutWidget_2);
        DiskWriteLabel->setObjectName(QString::fromUtf8("DiskWriteLabel"));
        DiskWriteLabel->setFont(font);

        ReadAndWriteLayout->addWidget(DiskWriteLabel);


        StorageverticalLayout->addLayout(ReadAndWriteLayout);

        verticalLayoutWidget_3 = new QWidget(centralwidget);
        verticalLayoutWidget_3->setObjectName(QString::fromUtf8("verticalLayoutWidget_3"));
        verticalLayoutWidget_3->setGeometry(QRect(410, 40, 381, 111));
        MemoryverticalLayout = new QVBoxLayout(verticalLayoutWidget_3);
        MemoryverticalLayout->setObjectName(QString::fromUtf8("MemoryverticalLayout"));
        MemoryverticalLayout->setContentsMargins(0, 0, 0, 0);
        MemoryTitleLabel = new QLabel(verticalLayoutWidget_3);
        MemoryTitleLabel->setObjectName(QString::fromUtf8("MemoryTitleLabel"));
        MemoryTitleLabel->setFont(font);

        MemoryverticalLayout->addWidget(MemoryTitleLabel);

        TotalMemoryLabel = new QLabel(verticalLayoutWidget_3);
        TotalMemoryLabel->setObjectName(QString::fromUtf8("TotalMemoryLabel"));
        TotalMemoryLabel->setFont(font);

        MemoryverticalLayout->addWidget(TotalMemoryLabel);

        MemoryUsageLayout = new QHBoxLayout();
        MemoryUsageLayout->setObjectName(QString::fromUtf8("MemoryUsageLayout"));
        MemoryUsageLabe = new QLabel(verticalLayoutWidget_3);
        MemoryUsageLabe->setObjectName(QString::fromUtf8("MemoryUsageLabe"));
        MemoryUsageLabe->setFont(font);

        MemoryUsageLayout->addWidget(MemoryUsageLabe);

        MemoryUsageprogressBar = new QProgressBar(verticalLayoutWidget_3);
        MemoryUsageprogressBar->setObjectName(QString::fromUtf8("MemoryUsageprogressBar"));
        MemoryUsageprogressBar->setValue(0);

        MemoryUsageLayout->addWidget(MemoryUsageprogressBar);


        MemoryverticalLayout->addLayout(MemoryUsageLayout);

        OSTypeLable = new QLabel(centralwidget);
        OSTypeLable->setObjectName(QString::fromUtf8("OSTypeLable"));
        OSTypeLable->setGeometry(QRect(10, 10, 781, 16));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(OSTypeLable->sizePolicy().hasHeightForWidth());
        OSTypeLable->setSizePolicy(sizePolicy);
        OSTypeLable->setFont(font);
        gridLayoutWidget_3 = new QWidget(centralwidget);
        gridLayoutWidget_3->setObjectName(QString::fromUtf8("gridLayoutWidget_3"));
        gridLayoutWidget_3->setGeometry(QRect(10, 260, 781, 211));
        DiskLayout = new QGridLayout(gridLayoutWidget_3);
        DiskLayout->setObjectName(QString::fromUtf8("DiskLayout"));
        DiskLayout->setContentsMargins(0, 0, 0, 0);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);
        QObject::connect(startBtn, SIGNAL(clicked()), MainWindow, SLOT(startMonitor()));
        QObject::connect(stopBtn, SIGNAL(clicked()), MainWindow, SLOT(stopMonitor()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        startBtn->setText(QCoreApplication::translate("MainWindow", "\345\274\200\345\247\213\347\233\221\346\216\247", nullptr));
        stopBtn->setText(QCoreApplication::translate("MainWindow", "\345\201\234\346\255\242\347\233\221\346\216\247", nullptr));
        CPUTitleLabel->setText(QCoreApplication::translate("MainWindow", "CPU Information:", nullptr));
        CPUTypeLabel->setText(QCoreApplication::translate("MainWindow", "CPU Type:", nullptr));
        CUPUsageLabel->setText(QCoreApplication::translate("MainWindow", "CPU Usage:", nullptr));
        StorageTitleLabel->setText(QCoreApplication::translate("MainWindow", "Storage:", nullptr));
        DiskReadLabel->setText(QCoreApplication::translate("MainWindow", "Read:", nullptr));
        DiskWriteLabel->setText(QCoreApplication::translate("MainWindow", "Write:", nullptr));
        MemoryTitleLabel->setText(QCoreApplication::translate("MainWindow", "Memory Information:", nullptr));
        TotalMemoryLabel->setText(QCoreApplication::translate("MainWindow", "System Memory:", nullptr));
        MemoryUsageLabe->setText(QCoreApplication::translate("MainWindow", "Memory Usage:", nullptr));
        OSTypeLable->setText(QCoreApplication::translate("MainWindow", "OS:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
