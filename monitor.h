#ifndef MONITOR_H
#define MONITOR_H

#include <Windows.h>
#include <Pdh.h>
#include <QString>
#include <QList>
#include <disk.h>
#include <drive.h>

class Monitor
{
public:
    Monitor();
    ~Monitor();
    double GetCPUUsage();
    double GetMemoryUsage();
    double GetDiskReadSpeed();
    double GetDiskWriteSpeed();
    QString GetCPUID();
    QString GetOSVersion();
    double GetSystemMemory();
    int Update();
    int GetdiskCount();
    QString GetDriveID(int driveIndex);
    void getPartitonList(int driveIndex,QList<partition> *tempPartitonList);

private:
    bool QueryCPUID(QString &CPUID);
    bool QueryOSVersion(QString &OSVersion);
    bool SetDiskInf();

private:
    int querytime;
    int diskCount;
    QList<drive> *driveList;
    double m_dbTotalCPUUsage;
    double m_dbTotalMemoryUsage;
    double m_dbTotalDiskReadSpeed;
    double m_dbTotalDiskWriteSpeed;
    double m_dbTotalMemoryCapc;
    double m_dbMemoryAvailbable;
    QString m_sOSVersion; // 存放操作系统的版本
    QString m_sCPUID;  // 存放CPU的ID
    LPMEMORYSTATUSEX m_lpBuffer;
    HCOUNTER m_hTotalCPUCounter;
    HCOUNTER m_hTotalDiskReadCounter;
    HCOUNTER m_hTotalDiskWriteCounter;
    DWORD m_dwValue;
    PDH_FMT_COUNTERVALUE m_pdhCounterValue;
    HQUERY m_hQuery;
};

#endif // MONITOR_H
