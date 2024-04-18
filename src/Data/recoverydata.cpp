#include "recoverydata.h"

RecoveryData::RecoveryData(QString nameRoom, QString description, QDate date1, QDate date2)
{
    this->roomName = nameRoom;
    this->description = description;
    this->lastDate = date2;
    this->startDate = date1;
}

QString RecoveryData::getRoomName() const
{
    return roomName;
}

QString RecoveryData::getDescription() const
{
    return description;
}

QDate RecoveryData::getStartDate() const
{
    return startDate;
}

QDate RecoveryData::getLastDate() const
{
    return lastDate;
}
