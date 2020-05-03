#include "disk.h"
Disk::Disk()
{
    diskId = new QLabel();
    diskId->setText("null");
    labelMap = new QMap<QString, diskLabelInfo*>();
    this->addWidget((QWidget*)diskId, 0, 0, 1, 3);
    this->partitionCount = 0;
}

Disk::Disk(QString id)
{
    diskId = new QLabel("Disk Id:" + id);
    labelMap = new QMap<QString, diskLabelInfo*>();
    this->addWidget((QWidget*)diskId, 0, 0, 1, 3);
    this->partitionCount = 0;
}

Disk::~Disk()
{

}

void Disk::setDiskId(QString id)
{
    this->diskId->setText("Disk Id:" + id);
}

/**
 * @brief Disk::addPartition 为一个磁盘添加分卷
 * @param name 分卷名，如"C:"
 * @param usage 分卷已使用容量，单位GB;要求保留1-2为小数
 * @param total 分卷总容量，单位GB;要求保留1-2为小数
 */
void Disk::addPartition(QString name, float usage, float total)
{
    partitionCount = partitionCount + 1;
    QString usageInfo = QString("%1").arg(usage) + "GB";
    QString totalInfo = QString("%1").arg(total) + "GB";
    QLabel* label = new QLabel(name);
    QLabel* usageLabel = new QLabel(usageInfo + "/" + totalInfo);
    QLabel* percentLabel = new QLabel(QString::number((int)((usage / total) * 100)) + "%");
    DiskLabelInfo* labelInfo = (DiskLabelInfo*)malloc(sizeof (DiskLabelInfo));
    labelInfo->usageLabel = usageLabel;
    labelInfo->percentLabel = percentLabel;
    this->labelMap->insert(name, labelInfo);
    this->addWidget((QWidget*)label, partitionCount, 0, 1, 1);
    this->addWidget((QWidget*)usageLabel, partitionCount, 1, 1, 1);
    this->addWidget((QWidget*)percentLabel, partitionCount, 2, 1, 1);
}

/**
 * @brief Disk::setPartition 设置/更新分卷利用率
 * @param name 分卷名，如"C:",!!必须与创建分卷时设置的一致!!
 * @param usage 分卷已使用容量，单位GB;要求保留1-2为小数
 * @param total 分卷总容量，单位GB;要求保留1-2为小数
 */
void Disk::setPartition(QString name, float usage, float total)
{
    QString usageInfo = QString("%1").arg(usage) + "GB";
    QString totalInfo = QString("%1").arg(total) + "GB";
    DiskLabelInfo* labelInfo = this->labelMap->find(name).value();
    if(labelInfo != nullptr)
    {
        QLabel* usageLabel = labelInfo->usageLabel;
        QLabel* percentLabel = labelInfo->percentLabel;
        usageLabel->setText(usageInfo + "/" + totalInfo);
        percentLabel->setText(QString::number((int)((usage / total) * 100)) + "%");
    }
}

/**
 * @brief Disk::getDiskId 获取磁盘id
 * @return 返回磁盘id:QString
 */
QString Disk::getDiskId()
{
    return this->diskId->text().remove("Disk Id:");
}
