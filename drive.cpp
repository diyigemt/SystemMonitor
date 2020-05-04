#include "drive.h"

drive::drive()
{
  this->id=nullptr;
  this->partitionList=new QList<partition>();
}

drive::drive(QString id)
{
  this->id=id;
  this->partitionList=new QList<partition>();
}

void drive::addPartition(QString name, float free, float size)
{
    partition tempPartition;
    tempPartition.name=name;
    tempPartition.free=free;
    tempPartition.size=size;
    partitionList->append(tempPartition);
}

QString drive::getDriveID()
{
    QString tempID=this->id;
    return tempID;
}

void drive::getPartitonList(QList<partition> *tempPartitonList)
{
   for(int i=0;i<=(*partitionList).length()-1;i++)
   tempPartitonList->append((*partitionList)[i]);
}
