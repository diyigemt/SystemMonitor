#pragma    comment(lib,"Pdh.lib")
#include "monitor.h"

#include <QString.h>
#include <Pdh.h>
#include <pdhmsg.h>
#include <Windows.h>

Monitor::Monitor()
{

}
Monitor::~Monitor()
{

}
QString Monitor::getCPUUsage()
{
    HQUERY query;
    PDH_STATUS status = PdhOpenQuery(NULL, NULL, &query);
    PDH_STATUS AvilableMemorystatus = PdhOpenQuery(NULL, NULL, &query);

    if (status != ERROR_SUCCESS)
        return "Open Query Error";

    HCOUNTER counter;

    //counter = (HCOUNTER*)GlobalAlloc(GPTR, sizeof(HCOUNTER));
    HCOUNTER cntProcessCPU, cntAvilableMemory;
    status = PdhAddCounter(query, TEXT("\\Processor Information(_Total)\\% Processor Utility"), NULL, &counter);

    PdhCollectQueryData(query);
    Sleep(1000);
    PdhCollectQueryData(query);

    PDH_FMT_COUNTERVALUE pdhValue;
    DWORD dwValue;

    status = PdhGetFormattedCounterValue(counter, PDH_FMT_DOUBLE, &dwValue, &pdhValue);
    if (status != ERROR_SUCCESS)
        return  "Get Value Error";
    QString s = "processCPU:" + QString("%1").arg(pdhValue.doubleValue);
    return s;
}
