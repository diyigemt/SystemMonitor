#ifndef MONITOR_H
#define MONITOR_H

#include <QString>
#include <Windows.h>
#include <Pdh.h>

class Monitor
{
public:
    Monitor();
    ~Monitor();
    double GetCPUUsage();
    double GetMemoryUsage();
    double GetDiskReadSpeed();
    double GetDiskWriteSpeed();
private:
    double m_dbTotalCPUUsage;
    double m_dbTotalMemoryUsage;
    double m_dbTotalDiskReadSpeed;
    double m_dbTotalDiskWriteSpeed;

private:
    void Update();

private:
    HCOUNTER m_hTotalCPUCounter;
    HCOUNTER m_hTotalMemoryCounter;
    HCOUNTER m_hTotalDiskReadCounter;
    HCOUNTER m_hTotalDiskWriteCounter;
    DWORD m_dwValue;
    PDH_FMT_COUNTERVALUE m_pdhCounterValue;
    HQUERY m_hQuery;
};

#endif // MONITOR_H
