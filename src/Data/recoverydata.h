#ifndef RECOVERYDATA_H
#define RECOVERYDATA_H

#include <QDate>
#include <QList>

class RecoveryData
{
public:
    RecoveryData(int recId, QString nameRoom, QString description, QDate date1, QDate date2);

    QString getRoomName() const;
    QString getDescription() const;
    QDate getStartDate() const;
    QDate getLastDate() const;

    int getId() const;

private:
    int id;
    QString roomName;
    QDate startDate;
    QDate lastDate;
    QString description;
};

#endif // RECOVERYDATA_H
