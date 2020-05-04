#pragma    comment(lib,"Pdh.lib")
#pragma    comment(lib, "user32.lib")
#include "monitor.h"
#include <PdhMsg.h>
#include "constants.h"
#include <QDebug>
#include <WinUser.h>

//wmic相关定义
#define k2g 1073741824 //byte to gb
#define _WIN32_DCOM
#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")


//宽字符转浮点数函数声明
float w2f(const wchar_t* pwstr);
//宽字符转QSTRING函数声明
QString w2s(const wchar_t* pwstr);

// 这两个结构体不用管，这是查询信息用的
typedef struct _PEB {
    BOOLEAN InheritedAddressSpace;
    BOOLEAN ReadImageFileExecOptions;
    BOOLEAN BeingDebugged;
    BOOLEAN BitField;
    HANDLE Mutant;
    PVOID ImageBaseAddress;
    PVOID Ldr;
    PVOID ProcessParameters;
    PVOID SubSystemData;
    PVOID ProcessHeap;
    PVOID FastPebLock;
    PVOID AtlThunkSListPtr;
    PVOID SparePtr2;
    ULONG EnvironmentUpdateCount;
    PVOID KernelCallbackTable;
    ULONG SystemReserved[1];
    ULONG SpareUlong;
    PVOID FreeList;
    ULONG TlsExpansionCounter;
    PVOID TlsBitmap;
    ULONG TlsBitmapBits[2];
    PVOID ReadOnlySharedMemoryBase;
    PVOID ReadOnlySharedMemoryHeap;
    PVOID *ReadOnlyStaticServerData;
    PVOID AnsiCodePageData;
    PVOID OemCodePageData;
    PVOID UnicodeCaseTableData;
    ULONG NumberOfProcessors;
    ULONG NtGlobalFlag;
    LARGE_INTEGER CriticalSectionTimeout;
    SIZE_T HeapSegmentReserve;
    SIZE_T HeapSegmentCommit;
    SIZE_T HeapDeCommitTotalFreeThreshold;
    SIZE_T HeapDeCommitFreeBlockThreshold;
    ULONG NumberOfHeaps;
    ULONG MaximumNumberOfHeaps;
    PVOID *ProcessHeaps;
    PVOID GdiSharedHandleTable;
    PVOID ProcessStarterHelper;
    ULONG GdiDCAttributeList;
    PVOID LoaderLock;
    ULONG OSMajorVersion;
    ULONG OSMinorVersion;
    USHORT OSBuildNumber;
    USHORT OSCSDVersion;
    ULONG OSPlatformId;
} PEB, *PPEB;

typedef struct _TEB {
    NT_TIB NtTib;
    PVOID EnvironmentPointer;
    struct {
        HANDLE UniqueProcess;
        HANDLE UniqueThread;
    } ClientId;
    PVOID ActiveRpcHandle;
    PVOID ThreadLocalStoragePointer;
    PEB *ProcessEnvironmentBlock;
} TEB, *PTEB;


/**
 * @brief Monitor::Monitor 无参构造函数，默认构建四个计数器
 */
Monitor::Monitor()
{
    PDH_STATUS status = PdhOpenQuery(nullptr, 0, &this->m_hQuery);
    this->m_lpBuffer = new MEMORYSTATUSEX;
    this->m_lpBuffer->dwLength = sizeof (MEMORYSTATUSEX);
    // 添加各种Counter
    status = PdhAddCounter(this->m_hQuery, TEXT("\\Processor Information(_Total)\\% Processor Utility"), NULL, &this->m_hTotalCPUCounter);
    status = PdhAddCounter(this->m_hQuery, TEXT("\\Process(_Total)\\IO Read Bytes/sec"), NULL, &this->m_hTotalDiskReadCounter);
    status = PdhAddCounter(this->m_hQuery, TEXT("\\Process(_Total)\\IO Write Bytes/sec"), NULL, &this->m_hTotalDiskWriteCounter);
    // 获取系统的版本
    this->QueryOSVersion(this->m_sOSVersion);
    qDebug() << this->m_sOSVersion << endl;
    //获取cpuid
    QueryCPUID(m_sCPUID);
    //获取硬盘信息
    this->SetDiskInf(this->diskCount,this->disk1,this->disk2);
    this->Update(); // 初始化
}


/**
 * @brief Monitor::~Monitor 析构函数，在这里移除counter，关闭查询
 */
Monitor::~Monitor()
{
   PdhRemoveCounter(this->m_hTotalCPUCounter);
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
 * @brief Monitor::GetCPUID
 * @return QString 本机的CPUID字符串
 */

QString Monitor::GetCPUID()
{
    return this->m_sCPUID;
}

QString Monitor::GetOSVersion()
{
    return this->m_sOSVersion;
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

    //更新cpuid
    QueryCPUID(m_sCPUID);

    //清楚全部数据重新读取
    SetDiskInf(diskCount,disk1,disk2);

    return result;
}

/**
 * @brief Monitor::QueryCPUID
 * @return QString 通过wmci指令查询到的CPUID
 */
bool Monitor::QueryCPUID(QString &CPUID)
{
    IWbemLocator* pLoc=NULL;
    IWbemServices* pSvc=NULL;

    HRESULT hres;

    //玄学
    CoUninitialize();
    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres))
    {
        qDebug() << "Failed to initialize COM library. Error code = 0x"
            << hex << hres << endl;
    }

    hres = CoInitializeSecurity(
        NULL,
        -1,
        NULL,
        NULL,
        RPC_C_AUTHN_LEVEL_DEFAULT,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE,
        NULL
    );

    if (FAILED(hres))
    {
        qDebug() << "Failed to initialize security. Error code = 0x"
            << hex << hres << endl;
        CoUninitialize();
    }

    hres = CoCreateInstance(
        CLSID_WbemLocator,
        0,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator, (LPVOID*)&pLoc);

    if (FAILED(hres))
    {
        qDebug()<< "Failed to create IWbemLocator object."
            << " Err code = 0x"
            << hex << hres << endl;
        CoUninitialize();
    }

    hres = pLoc->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"),
        NULL,
        NULL,
        0,
        NULL,
        0,
        0,
        &pSvc
    );

    if (FAILED(hres))
    {
        qDebug()<< "Could not connect. Error code = 0x"
            << hex << hres << endl;
        pLoc->Release();
        CoUninitialize();
    }

    hres = CoSetProxyBlanket(
        pSvc,
        RPC_C_AUTHN_WINNT,
        RPC_C_AUTHZ_NONE,
        NULL,
        RPC_C_AUTHN_LEVEL_CALL,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE);

    if (FAILED(hres))
    {
        qDebug()<< "Could not set proxy blanket. Error code = 0x"
            << hex << hres << endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
    }

    IEnumWbemClassObject* pEnumerator = NULL;

    hres = pSvc->ExecQuery(
        bstr_t("WQL"),
        bstr_t("SELECT * FROM Win32_Processor"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator);

    if (FAILED(hres)) {
        qDebug()<< "Query for processes failed. "
            << "Error code = 0x"
            << hex << hres << endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return FALSE;
    }
    else {
        IWbemClassObject* pclsObj;
        ULONG uReturn = 0;
        while (pEnumerator) {
            hres = pEnumerator->Next(WBEM_INFINITE, 1,
                &pclsObj, &uReturn);
            if (0 == uReturn) break;

            VARIANT vtProp3;
            hres = pclsObj->Get(_bstr_t(L"DeviceID"), 0, &vtProp3, 0, 0);

            std::wstring tmp = vtProp3.bstrVal;
            tmp = tmp.substr(4);

            VARIANT vtProp3_id;
            pclsObj->Get(L"ProcessorId", 0, &vtProp3_id, 0, 0);

            if (!(vtProp3.vt == VT_EMPTY || vtProp3.vt == VT_I4 ||
                vtProp3.vt == VT_DISPATCH))
            {
                CPUID = w2s(vtProp3_id.bstrVal);
                //test
                qDebug()<<"origin:"<< w2s(vtProp3_id.bstrVal);
                qDebug()<<"cpuid:"<< CPUID;
            }
        }
    }
    pEnumerator->Release();

    pSvc->Release();
    pLoc->Release();
    CoUninitialize();

    return TRUE;
}

bool Monitor::QueryOSVersion(QString &OSVersion)
{
    OSVersion = "";  // 参数置空

    // 使用systeminfo来获取CPU架构
    SYSTEM_INFO systeminfo;
    GetSystemInfo(&systeminfo);

    // 使用os获取操作系统的类型和版本
    DWORD dwMajorVersion, dwMinorVersion;
    TEB *lpTeb = NtCurrentTeb();
    if (lpTeb != nullptr)
    {
        PEB *lpPeb = lpTeb->ProcessEnvironmentBlock;
        if (lpPeb != nullptr)
        {
            dwMajorVersion = lpPeb->OSMajorVersion;
            dwMinorVersion = lpPeb->OSMinorVersion;
        }
    }
    OSVERSIONINFOEX os;
    os.dwOSVersionInfoSize = sizeof (OSVERSIONINFOEX);
    bool unknown = false;
    if (GetVersionEx((OSVERSIONINFO *)&os))
    {
        // 判断主版本号
        switch (dwMajorVersion)
        {
        case 10:
            // 判断次版本号
            switch (dwMinorVersion)
            {
            case 0:
                if (os.wProductType == VER_NT_WORKSTATION)
                {
                    OSVersion += "Windows 10";
                }
                else
                {
                    OSVersion += "Windows Server 2016";
                }
                break;
            default:
                unknown = true;
                break;
            }
            break;
        case 6:
            switch (dwMinorVersion)
            {
            case 3:
                if (os.wProductType == VER_NT_WORKSTATION)
                {
                    OSVersion += "Windows 8.1";
                }
                else
                {
                    OSVersion += "Windows Server 2012 R2";
                }
                break;
            case 2:
                if (os.wProductType == VER_NT_WORKSTATION)
                {
                    OSVersion += "Windows 8";
                }
                else
                {
                    OSVersion += "Windows Server 2012";
                }
                break;
            case 1:
                if (os.wProductType == VER_NT_WORKSTATION)
                {
                    OSVersion += "Windows 7";
                }
                else
                {
                    OSVersion += "Windows Server 2008 R2";
                }
                break;
            case 0:
                if (os.wProductType == VER_NT_WORKSTATION)
                {
                    OSVersion += "Windows Vista";
                }
                else
                {
                    OSVersion += "Windows Server 2008";
                }
                break;
            default:
                unknown = true;
                break;
            }
            break;
        case 5:
            switch (dwMinorVersion)
            {
            case 2:
                if (os.wSuiteMask & VER_SUITE_WH_SERVER)
                {
                    OSVersion += "Windows Home Server";
                }
                else if (GetSystemMetrics(SM_SERVERR2) == 0)
                {
                    OSVersion += "Windows Server 2003";
                }
                else if((os.wProductType == VER_NT_WORKSTATION) && (systeminfo.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64))
                {
                    OSVersion += "Windows XP Professional x64 Edition";
                }
                else
                {
                    unknown = true;
                }
                break;
            case 1:
                OSVersion += "Windows XP";
                break;
            case 0:
                OSVersion += "Windows 2000";
                break;
            default:
                unknown = true;
                break;
            }
            break;
        default:
            unknown = true;
            break;
        }
    }
    if (unknown)
    {
        OSVersion = "UNKNOWN";
        return false;
    }
    return true;
}

/**
 * @brief Monitor::SetDiskInf
 * @return QString 通过wmci指令添加硬盘和分区信息
 */
bool Monitor::SetDiskInf(int &diskCount,Disk &disk1,Disk &disk2)
{
    //diskcount=0
    diskCount=0;

    IWbemLocator* pLoc=NULL;
    IWbemServices* pSvc=NULL;

    HRESULT hres;

    //玄学
    CoUninitialize();
    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres))
    {
        qDebug() << "Failed to initialize COM library. Error code = 0x"
            << hex << hres << endl;
    }

    hres = CoInitializeSecurity(
        NULL,
        -1,
        NULL,
        NULL,
        RPC_C_AUTHN_LEVEL_DEFAULT,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE,
        NULL
    );

    if (FAILED(hres))
    {
        qDebug() << "Failed to initialize security. Error code = 0x"
            << hex << hres << endl;
        CoUninitialize();
    }

    hres = CoCreateInstance(
        CLSID_WbemLocator,
        0,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator, (LPVOID*)&pLoc);

    if (FAILED(hres))
    {
        qDebug()<< "Failed to create IWbemLocator object."
            << " Err code = 0x"
            << hex << hres << endl;
        CoUninitialize();
    }

    hres = pLoc->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"),
        NULL,
        NULL,
        0,
        NULL,
        0,
        0,
        &pSvc
    );

    if (FAILED(hres))
    {
        qDebug()<< "Could not connect. Error code = 0x"
            << hex << hres << endl;
        pLoc->Release();
        CoUninitialize();
    }

    hres = CoSetProxyBlanket(
        pSvc,
        RPC_C_AUTHN_WINNT,
        RPC_C_AUTHZ_NONE,
        NULL,
        RPC_C_AUTHN_LEVEL_CALL,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE);

    if (FAILED(hres))
    {
        qDebug()<< "Could not set proxy blanket. Error code = 0x"
            << hex << hres << endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
    }

    IEnumWbemClassObject* pEnumerator = NULL;

    hres = pSvc->ExecQuery(
        bstr_t("WQL"),
        bstr_t("SELECT * FROM Win32_DiskDrive"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator);

    if (FAILED(hres)) {
        qDebug()<< "Query for processes failed. "
            << "Error code = 0x"
            << hex << hres << endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return FALSE;
    }
    else {
        IWbemClassObject* pclsObj;
        ULONG uReturn = 0;
        while (pEnumerator) {

            hres = pEnumerator->Next(WBEM_INFINITE, 1,
                &pclsObj, &uReturn);
            if (0 == uReturn) break;

            VARIANT vtProp;
            hres = pclsObj->Get(_bstr_t(L"DeviceID"), 0, &vtProp, 0, 0);

            std::wstring tmp = vtProp.bstrVal;
            tmp = tmp.substr(4);

            // 获取硬盘信息
            pclsObj->Get(L"Caption", 0, &vtProp, 0, 0);
            VARIANT vtProp_id;
            pclsObj->Get(L"SerialNumber", 0, &vtProp_id, 0, 0);

            //添加硬盘信息
            if (!(vtProp.vt == VT_EMPTY || vtProp.vt == VT_I4 ||
                vtProp.vt == VT_DISPATCH)){             
                diskCount++;
                if(diskCount==1) disk1.setDiskId(w2s(vtProp_id.bstrVal));
                if(diskCount==2) disk2.setDiskId(w2s(vtProp_id.bstrVal));

                  //test
                  qDebug()<<"origin:"<<w2s(vtProp_id.bstrVal);
                  qDebug()<<"diskcount:"<<diskCount;
                };

            //确定硬盘对应分区
            std::wstring wstrQuery = L"Associators of {Win32_DiskDrive.DeviceID='\\\\.\\";
            wstrQuery += tmp;
            wstrQuery += L"'} where AssocClass=Win32_DiskDriveToDiskPartition";

            IEnumWbemClassObject* pEnumerator1 = NULL;
            hres = pSvc->ExecQuery(
                bstr_t("WQL"),
                bstr_t(wstrQuery.c_str()),
                WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
                NULL,
                &pEnumerator1);

            if (FAILED(hres)) {
                qDebug()<< "Query for processes failed. "
                    << "Error code = 0x"
                    << hex << hres << endl;
                pSvc->Release();
                pLoc->Release();
                CoUninitialize();
                return FALSE;
            }
            else {
                IWbemClassObject* pclsObj1;
                ULONG uReturn1 = 0;
                while (pEnumerator1) {
                    hres = pEnumerator1->Next(WBEM_INFINITE, 1,
                        &pclsObj1, &uReturn1);
                    if (0 == uReturn1) break;

                    //获取硬盘索引
                    VARIANT vtProp1;
                    hres = pclsObj1->Get(_bstr_t(L"DeviceID"), 0, &vtProp1, 0, 0);

                    std::wstring wstrQuery = L"Associators of {Win32_DiskPartition.DeviceID='";
                    wstrQuery += vtProp1.bstrVal;
                    wstrQuery += L"'} where AssocClass=Win32_LogicalDiskToPartition";

                    IEnumWbemClassObject* pEnumerator2 = NULL;
                    hres = pSvc->ExecQuery(
                        bstr_t("WQL"),
                        bstr_t(wstrQuery.c_str()),
                        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
                        NULL,
                        &pEnumerator2);

                    if (FAILED(hres)) {
                        qDebug()<< "Query for processes failed. "
                            << "Error code = 0x"
                            << hex << hres << endl;
                       pSvc->Release();
                        pLoc->Release();
                        CoUninitialize();
                        return FALSE;
                    }
                    else {
                        IWbemClassObject* pclsObj2;
                        ULONG uReturn2 = 0;
                        while (pEnumerator2) {
                            hres = pEnumerator2->Next(WBEM_INFINITE, 1,
                                &pclsObj2, &uReturn2);
                            if (0 == uReturn2) break;

                            // 获取分区信息
                            VARIANT vtProp2;
                            VARIANT vtProp2_size;
                            VARIANT vtProp2_free;
                            pclsObj2->Get(L"Size", 0, &vtProp2_size, 0, 0);
                            pclsObj2->Get(L"FreeSpace", 0, &vtProp2_free, 0, 0);

                            hres = pclsObj2->Get(_bstr_t(L"DeviceID"), 0, &vtProp2, 0, 0);

                            // 添加分区信息                       
                            if(diskCount==1) disk1.addPartition(w2s(vtProp2.bstrVal),(w2f(vtProp2_free.bstrVal))/k2g,w2f(vtProp2_size.bstrVal)/k2g);
                            if(diskCount==2) disk2.addPartition(w2s(vtProp2.bstrVal),(w2f(vtProp2_free.bstrVal))/k2g,w2f(vtProp2_size.bstrVal)/k2g);

                              //test
                              qDebug()<<"diskcount:"<<diskCount;
                              qDebug()<<"origin1:"<<w2s(vtProp2.bstrVal);
                              qDebug()<<"origin2:"<<(w2f(vtProp2_free.bstrVal))/k2g;
                              qDebug()<<"origin3:"<<(w2f(vtProp2_size.bstrVal))/k2g;

                            VariantClear(&vtProp2);
                            VariantClear(&vtProp2_size);
                            VariantClear(&vtProp2_free);
                        }
                        pclsObj1->Release();
                    }
                    VariantClear(&vtProp1);
                    pEnumerator2->Release();
                }
                pclsObj->Release();
            }
            VariantClear(&vtProp);
            VariantClear(&vtProp_id);
            pEnumerator1->Release();
        }
    }
    pEnumerator->Release();

    pSvc->Release();
    pLoc->Release();
    CoUninitialize();

    return TRUE;
}

float w2f(const wchar_t* pwstr)
{
    int nlength = wcslen(pwstr);
    //获取转换后的长度
    int nbytes = WideCharToMultiByte(0, 0, pwstr, nlength, NULL, 0, NULL, NULL);
    char* pcstr = new char[nbytes + 1];
    // 通过以上得到的结果，转换unicode 字符为ascii 字符
    WideCharToMultiByte(0, 0, pwstr, nlength, pcstr, nbytes, NULL, NULL);
    pcstr[nbytes] = '\0';
    //ascii字符转浮点数
    double number;
    sscanf_s(pcstr, "%lf", &number);

    return number;
}

QString w2s(const wchar_t* pwstr)
{
    QString q_str((QChar*)pwstr,wcslen(pwstr));
    return QString::fromUtf16(q_str.utf16());
}
