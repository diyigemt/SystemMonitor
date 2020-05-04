#ifndef COM_H
#define COM_H
#include <QtCore>

//wmic相关定义
#define _WIN32_DCOM
#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")

class Com:public QThread
{
    Q_OBJECT
public:
    Com();
    void closeCom();
    IWbemLocator* pLoc;
    IWbemServices* pSvc;
protected:
    void run();

};

#endif // COM_H
