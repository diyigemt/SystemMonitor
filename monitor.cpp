#pragma    comment(lib,"Pdh.lib")
#include "monitor.h"
#include <PdhMsg.h>
#include "constants.h"
#include <QDebug>


/**
 * @brief Monitor::Monitor 无参构造函数，默认构建四个计数器
 */
Monitor::Monitor()
{
    PDH_STATUS status = PdhOpenQuery(nullptr, 0, &this->m_hQuery);
    this->m_lpBuffer = new MEMORYSTATUSEX;
    this->m_lpBuffer->dwLength = sizeof (MEMORYSTATUSEX);
//    if (status != ERROR_SUCCESS)
//    {
//        MessageBox(NULL, TEXT("初始化查询失败!"), TEXT(""), MB_OK);
//    }
    // 添加各种Counter
    status = PdhAddCounter(this->m_hQuery, TEXT("\\Processor Information(_Total)\\% Processor Utility"), NULL, &this->m_hTotalCPUCounter);
    status = PdhAddCounter(this->m_hQuery, TEXT("\\Process(_Total)\\Working Set - Private"), NULL, &this->m_hTotalMemoryCounter);
    status = PdhAddCounter(this->m_hQuery, TEXT("\\Process(_Total)\\IO Read Bytes/sec"), NULL, &this->m_hTotalDiskReadCounter);
    status = PdhAddCounter(this->m_hQuery, TEXT("\\Process(_Total)\\IO Write Bytes/sec"), NULL, &this->m_hTotalDiskWriteCounter);
    this->Update(); // 初始化
}


/**
 * @brief Monitor::~Monitor 析构函数，在这里移除counter，关闭查询
 */
Monitor::~Monitor()
{
   PdhRemoveCounter(this->m_hTotalCPUCounter);
   PdhRemoveCounter(this->m_hTotalMemoryCounter);
   PdhRemoveCounter(this->m_hTotalDiskReadCounter);
   PdhRemoveCounter(this->m_hTotalDiskWriteCounter);
   PdhCloseQuery(this->m_hQuery);
   delete this->m_lpBuffer;
}

/**
 * @brief Monitor::GetMemoryUsage 获得所有进程的内存使用率
 * @return double 总体内存使用率
 */
double Monitor::GetMemoryUsage()
{

    return this->m_dbTotalMemoryUsage;
}

/**
 * @brief Monitor::GetCPUUsage 获得所有进程的CPU使用率
 * @return double 总体CPU使用率
 */
double Monitor::GetCPUUsage()
{
    return this->m_dbTotalCPUUsage;
}


/**
 * @brief Monitor::GetDiskReadSpeed 获得总体的磁盘读取速度
 * @return double 总体的磁盘读取速度
 */
double Monitor::GetDiskReadSpeed()
{
    return this->m_dbTotalDiskReadSpeed;
}


/**
 * @brief Monitor::GetDiskWriteSpeed 获得总体的磁盘写入速度
 * @return double 总体的磁盘写入速度
 */
double Monitor::GetDiskWriteSpeed()
{
    return this->m_dbTotalDiskWriteSpeed;
}

/**
 * @brief Monitor::GetSystemMemory 这是一个公有函数，用来获取内存总量信息。
 * @return double 返回内存总量
 */
double Monitor::GetSystemMemory()
{
    return this->m_dbTotalMemoryCapc;
}

/**
 * @brief Monitor::Update 这是一个私有函数，用来更新Monitor的各种counter的数据，可以作为一个线程定时更新，也可以实时调用。
 * @return int 返回更新状态
 */
int Monitor::Update()
{
    int result = 0;
    PDH_STATUS status;
    status = PdhCollectQueryData(this->m_hQuery);
//    Sleep(1000);  // 睡眠一秒钟再次收集一次数据
//    status = PdhCollectQueryData(this->m_hQuery);
    if (ERROR_SUCCESS != status)
    {
        result |= GET_DATA_FAILURE;
    }

    // 格式化数据
    status = PdhGetFormattedCounterValue(this->m_hTotalCPUCounter, PDH_FMT_DOUBLE, &this->m_dwValue, &this->m_pdhCounterValue);
    if (ERROR_SUCCESS != status)
    {
        result |= GET_CPUUSAGE_FAILURE;
    }
    else
    {
        this->m_dbTotalCPUUsage = this->m_pdhCounterValue.doubleValue;
    }

    bool retVal = GlobalMemoryStatusEx(this->m_lpBuffer);
    if (retVal == false)
    {
        result |= GET_MEMORY_FAILURE;
    }
    else
    {
        this->m_dbTotalMemoryCapc = this->m_lpBuffer->ullTotalPhys;
        this->m_dbMemoryAvailbable = this->m_lpBuffer->ullAvailPhys;
        this->m_dbTotalMemoryUsage = this->m_lpBuffer->dwMemoryLoad;
    }

    status = PdhGetFormattedCounterValue(this->m_hTotalDiskReadCounter, PDH_FMT_DOUBLE, &this->m_dwValue, &this->m_pdhCounterValue);
    if (ERROR_SUCCESS != status)
    {
        result |= GET_DISK_R_FAILURE;
    }
    else
    {
        this->m_dbTotalDiskReadSpeed = this->m_pdhCounterValue.doubleValue;
    }

    status = PdhGetFormattedCounterValue(this->m_hTotalDiskWriteCounter, PDH_FMT_DOUBLE, &this->m_dwValue, &this->m_pdhCounterValue);
    if (ERROR_SUCCESS != status)
    {
        result |= GET_DISK_W_FAILURE;
    }
    else
    {
        this->m_dbTotalDiskWriteSpeed = this->m_pdhCounterValue.doubleValue;
    }
    return result;
}
