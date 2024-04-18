#ifndef RECOVERYDATA_H
#define RECOVERYDATA_H

#include <QDate>
#include <QList>

class RecoveryData
{
public:
    RecoveryData(QString nameRoom, QString description, QDate date1, QDate date2);

    QString getRoomName() const;
    QString getDescription() const;
    QDate getStartDate() const;
    QDate getLastDate() const;

private:
    QString roomName;
    QDate startDate;
    QDate lastDate;
    QString description;
};

#endif // RECOVERYDATA_H
