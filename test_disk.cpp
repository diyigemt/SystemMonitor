#include <disk.h>
#include <qstringlist.h>

#define _WIN32_DCOM
#include <iostream>
#include <comdef.h>
#include <Wbemidl.h>

#pragma comment(lib, "wbemuuid.lib")

#define k2g 1073741824

using namespace std;

QStringList wmi_getDriveId();
BOOL wmi_getDriveInform();
BOOL wmi_getCpuInform();
BOOL wmi_run();
BOOL wmi_close();
FLOAT w2f(const wchar_t* pwstr);

IWbemLocator* pLoc = NULL;
IWbemServices* pSvc = NULL;

QStringList wmi_getDriveId()
{
    QStringList DiskIdList;

    wmi_run();

    HRESULT hres;
    IEnumWbemClassObject* pEnumerator = NULL;

    hres = pSvc->ExecQuery(
        bstr_t("WQL"),
        bstr_t("SELECT * FROM Win32_DiskDrive"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator);

    if (FAILED(hres)) {
        cout << "Query for processes failed. "
            << "Error code = 0x"
            << hex << hres << endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return DiskIdList;
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

            if (!(vtProp.vt == VT_EMPTY || vtProp.vt == VT_I4 ||
                vtProp.vt == VT_DISPATCH))
                DiskIdList.append((QString::fromStdWString)(vtProp.bstrVal));
        }
    }
    pEnumerator->Release();
    wmi_close();

    return DiskIdList;
}

BOOL wmi_getDriveInform()
{
    wmi_run();

    HRESULT hres;
    IEnumWbemClassObject* pEnumerator = NULL;

    hres = pSvc->ExecQuery(
        bstr_t("WQL"),
        bstr_t("SELECT * FROM Win32_DiskDrive"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator);

    if (FAILED(hres)) {
        cout << "Query for processes failed. "
            << "Error code = 0x"
            << hex << hres << endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return FALSE;               // Program has failed.
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

            // adjust string
            wstring tmp = vtProp.bstrVal;
            tmp = tmp.substr(4);

            // Size属性可以获取硬盘的大小
            // 如果需要使用的话，注意先判断vtProp的类型，目前虽然VARIANT结构体内置多种类型，
            // 但目前只支持四种类型：VT_I4、VT_DISPATCH、VT_BSTR、VT_EMPTY
            // 对应获取值得名称：lVal、pdispVal、bstrVal、none
            // 例如：vtProp类型为VT_BSTR，获取值为vtProp.bstrValue
            // 最后注意：获取的值为宽字符，可以通过WideCharToMultiByte转化为多字符
            VARIANT vtProp_size;
            pclsObj->Get(L"Size", 0, &vtProp_size, 0, 0);
            VARIANT vtProp_id;
            pclsObj->Get(L"SerialNumber", 0, &vtProp_id, 0, 0);

            // 打印硬盘型号
            pclsObj->Get(L"Caption", 0, &vtProp, 0, 0);

            if (!(vtProp.vt == VT_EMPTY || vtProp.vt == VT_I4 ||
                vtProp.vt == VT_DISPATCH))
                printf("硬盘型号:%ls\t硬盘序列号:%ls\t硬盘容量:%.2lf\n", vtProp.bstrVal, vtProp_id.bstrVal,(w2f(vtProp_size.bstrVal))/k2g);

            wstring wstrQuery = L"Associators of {Win32_DiskDrive.DeviceID='\\\\.\\";
            wstrQuery += tmp;
            wstrQuery += L"'} where AssocClass=Win32_DiskDriveToDiskPartition";

            // reference drive to partition
            IEnumWbemClassObject* pEnumerator1 = NULL;
            hres = pSvc->ExecQuery(
                bstr_t("WQL"),
                bstr_t(wstrQuery.c_str()),
                WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
                NULL,
                &pEnumerator1);

            if (FAILED(hres)) {
                cout << "Query for processes failed. "
                    << "Error code = 0x"
                    << hex << hres << endl;
                pSvc->Release();
                pLoc->Release();
                CoUninitialize();
                return FALSE;               // Program has failed.
            }
            else {

                IWbemClassObject* pclsObj1;
                ULONG uReturn1 = 0;
                while (pEnumerator1) {
                    hres = pEnumerator1->Next(WBEM_INFINITE, 1,
                        &pclsObj1, &uReturn1);
                    if (0 == uReturn1) break;

                    // 此处获取的都是Win32_DiskPartition磁盘分区信息

                    // reference logical drive to partition
                    VARIANT vtProp1;
                    hres = pclsObj1->Get(_bstr_t(L"DeviceID"), 0, &vtProp1, 0, 0);
                    //printf("DeviceID:%ls\n",vtProp1.bstrVal);

                    wstring wstrQuery = L"Associators of {Win32_DiskPartition.DeviceID='";
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
                        cout << "Query for processes failed. "
                            << "Error code = 0x"
                            << hex << hres << endl;
                        pSvc->Release();
                        pLoc->Release();
                        CoUninitialize();
                        return FALSE;               // Program has failed.
                    }
                    else {

                        // get driveletter
                        IWbemClassObject* pclsObj2;
                        ULONG uReturn2 = 0;
                        while (pEnumerator2) {
                            hres = pEnumerator2->Next(WBEM_INFINITE, 1,
                                &pclsObj2, &uReturn2);
                            if (0 == uReturn2) break;

                            // 获取的是Win32_LogicalDisk的信息
                            // 例如：磁盘C,磁盘D都是同一硬盘的逻辑磁盘
                            // 可以在单独获取磁盘C、磁盘D的剩余空间，总空间信息
                            // 下面vtProp.bstrVal为上面获取的硬盘的型号
                            // vtProp2.bstrVal为磁盘的盘符C或D
                            // 分别获取磁盘的总大小和剩余大小。注意事项和硬盘部分描述一样

                            VARIANT vtProp2;
                            VARIANT vtProp2_size;
                            VARIANT vtProp2_free;

                            pclsObj2->Get(L"Size", 0, &vtProp2_size, 0, 0);
                            pclsObj2->Get(L"FreeSpace", 0, &vtProp2_free, 0, 0);

                            hres = pclsObj2->Get(_bstr_t(L"DeviceID"), 0, &vtProp2, 0, 0);
                            // print result
                            printf("硬盘型号:%ls\t磁盘盘符:%ls\t盘符容量:%.2lf\t盘符可用:%.2lf\n", vtProp.bstrVal, vtProp2.bstrVal,
                                (w2f(vtProp2_size.bstrVal)) / k2g, (w2f(vtProp2_free.bstrVal)) / k2g);
                            VariantClear(&vtProp2);
                        }
                        pclsObj1->Release();
                    }
                    VariantClear(&vtProp1);
                    pEnumerator2->Release();
                }
                pclsObj->Release();
            }
            VariantClear(&vtProp);
            pEnumerator1->Release();
        }
    }
    pEnumerator->Release();
    return TRUE;
}

BOOL wmi_getCpuInform()
{
    // Use the IWbemServices pointer to make requests of WMI.
    // Make requests here:
    HRESULT hres;
    IEnumWbemClassObject* pEnumerator = NULL;

    hres = pSvc->ExecQuery(
        bstr_t("WQL"),
        bstr_t("SELECT * FROM Win32_Processor"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator);

    if (FAILED(hres)) {
        cout << "Query for processes failed. "
            << "Error code = 0x"
            << hex << hres << endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return FALSE;               // Program has failed.
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

            // adjust string
            wstring tmp = vtProp3.bstrVal;
            tmp = tmp.substr(4);

            VARIANT vtProp3_id;
            pclsObj->Get(L"ProcessorId", 0, &vtProp3_id, 0, 0);

            if (!(vtProp3.vt == VT_EMPTY || vtProp3.vt == VT_I4 ||
                vtProp3.vt == VT_DISPATCH))
                printf("CPU序列号:%ls\n", vtProp3_id.bstrVal);
        }
    }
    pEnumerator->Release();
    return TRUE;
}

BOOL wmi_run()
{
    HRESULT hres;

    // Step 1: --------------------------------------------------
    // Initialize COM. ------------------------------------------

    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres))
    {
        cout << "Failed to initialize COM library. Error code = 0x"
            << hex << hres << endl;
        return 1;                  // Program has failed.
    }

    // Step 2: --------------------------------------------------
    // Set general COM security levels --------------------------
    // Note: If you are using Windows 2000, you need to specify -
    // the default authentication credentials for a user by using
    // a SOLE_AUTHENTICATION_LIST structure in the pAuthList ----
    // parameter of CoInitializeSecurity ------------------------

    hres = CoInitializeSecurity(
        NULL,
        -1,                          // COM authentication
        NULL,                        // Authentication services
        NULL,                        // Reserved
        RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
        RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation
        NULL,                        // Authentication info
        EOAC_NONE,                   // Additional capabilities
        NULL                         // Reserved
    );



    if (FAILED(hres))
    {
        cout << "Failed to initialize security. Error code = 0x"
            << hex << hres << endl;
        CoUninitialize();
        return 1;                    // Program has failed.
    }

    // Step 3: ---------------------------------------------------
    // Obtain the initial locator to WMI -------------------------

    //IWbemLocator *pLoc = NULL;

    hres = CoCreateInstance(
        CLSID_WbemLocator,
        0,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator, (LPVOID*)&pLoc);

    if (FAILED(hres))
    {
        cout << "Failed to create IWbemLocator object."
            << " Err code = 0x"
            << hex << hres << endl;
        CoUninitialize();
        return 1;                 // Program has failed.
    }

    // Step 4: -----------------------------------------------------
    // Connect to WMI through the IWbemLocator::ConnectServer method

    //IWbemServices *pSvc = NULL;

    // Connect to the root\cimv2 namespace with
    // the current user and obtain pointer pSvc
    // to make IWbemServices calls.
    hres = pLoc->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
        NULL,                    // User name. NULL = current user
        NULL,                    // User password. NULL = current
        0,                       // Locale. NULL indicates current
        NULL,                    // Security flags.
        0,                       // Authority (e.g. Kerberos)
        0,                       // Context object
        &pSvc                    // pointer to IWbemServices proxy
    );

    if (FAILED(hres))
    {
        cout << "Could not connect. Error code = 0x"
            << hex << hres << endl;
        pLoc->Release();
        CoUninitialize();
        return 1;                // Program has failed.
    }

    cout << "Connected to ROOT\\CIMV2 WMI namespace" << endl;

    // Step 5: --------------------------------------------------
    // Set security levels on the proxy -------------------------

    hres = CoSetProxyBlanket(
        pSvc,                        // Indicates the proxy to set
        RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
        RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
        NULL,                        // Server principal name
        RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx
        RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
        NULL,                        // client identity
        EOAC_NONE                    // proxy capabilities
    );

    if (FAILED(hres))
    {
        cout << "Could not set proxy blanket. Error code = 0x"
            << hex << hres << endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return 1;               // Program has failed.
    }
    return 0;
}

BOOL wmi_close()
{
    // Cleanup
    // ========

    pSvc->Release();
    pLoc->Release();
    CoUninitialize();

    return 0;   // Program successfully completed.
}

FLOAT w2f(const wchar_t* pwstr)
{
    int nlength = wcslen(pwstr);
    //获取转换后的长度
    int nbytes = WideCharToMultiByte(0, 0, pwstr, nlength, NULL, 0, NULL, NULL);
    char* pcstr = new char[nbytes + 1];
    // 通过以上得到的结果，转换unicode 字符为ascii 字符
    WideCharToMultiByte(0, 0, pwstr, nlength, pcstr, nbytes, NULL, NULL);
    pcstr[nbytes] = '\0';

    double number;
    sscanf_s(pcstr, "%lf", &number);

    return number;
}
