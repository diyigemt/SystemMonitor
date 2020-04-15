#ifndef MONITOR_H
#define MONITOR_H

#include <QString>
class Monitor
{
public:
    Monitor();
    ~Monitor();
    QString getCPUUsage();
};

#endif // MONITOR_H
