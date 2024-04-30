#include "recoverydata.h"

RecoveryData::RecoveryData(
    int recId, QString nameRoom, QString description, QDate date1, QDate date2, QString image)
{
    this->id = recId;
    this->roomName = nameRoom;
    this->description = description;
    this->lastDate = date2;
    this->startDate = date1;
    this->image = image;
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

int RecoveryData::getId() const
{
    return id;
}

QString RecoveryData::getImage() const
{
    return image;
}
