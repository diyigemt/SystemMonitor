#ifndef DRIVE_H
#define DRIVE_H

#include <QString>
#include <QList>

typedef struct Partition{
    QString name;
    float free;
    float size;
} partition;

class drive
{
public:
    drive();
    drive(QString id);
    void addPartition(QString name,float free,float size);
    QString getDriveID();
    void getPartitonList(QList<partition> *tempPartitonList);

private:
    QString id=nullptr;
    QList<partition> *partitionList=nullptr;
};

#endif // DRIVE_H
