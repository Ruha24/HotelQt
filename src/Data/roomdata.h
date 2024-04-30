#ifndef ROOMDATA_H
#define ROOMDATA_H

#include <QFile>
#include <QImage>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QString>

class Roomdata
{
public:
    Roomdata(int id, QString type, int price, int count, QString description, QString image);

    Roomdata();

    int getId() const;

    QString getTypeRoom() const;

    int getStartPrice() const;

    int getCountRoom() const;

    QString getDescription() const;

    void getRooms(int countPlace, std::function<void(bool)> callback);

    QList<Roomdata> getListRooms() const;

    QString getImage() const;

private:
    int id;
    QString typeRoom;
    int startPrice;
    int countRoom;
    QString description;
    QString image;

    QList<Roomdata> listRooms;
};

#endif // ROOMDATA_H
