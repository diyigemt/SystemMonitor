#ifndef DISK_H
#define DISK_H
#include <QGridLayout>
#include <QLabel>
#include <QString>
#include <QMap>

typedef struct DiskLabelInfo{
    QLabel* usageLabel;
    QLabel* percentLabel;
} diskLabelInfo;

class Disk :public QGridLayout
{
    Q_OBJECT
public:
    Disk();
    Disk(QString id);
    ~Disk();
    void setDiskId(QString id);
    void addPartition(QString name, float usage, float total);
    void setPartition(QString name, float usage, float total);
    QString getDiskId();
private:
    int partitionCount;
    QLabel* diskId;
    QMap<QString, diskLabelInfo*>* labelMap;
};

#endif // DISK_H
