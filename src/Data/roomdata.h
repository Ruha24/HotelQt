#ifndef ROOMDATA_H
#define ROOMDATA_H

#include <QFile>
#include <QFileInfo>
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

    void getRoomsSearch(int countPlace, std::function<void(bool)> callback);

    void getRooms(std::function<void(bool)> callback);

    void deleteRoom(int idRoom, std::function<void(bool)> callback);

    void addRoom(QString roomName,
                 int price,
                 QString description,
                 QString imagePath,
                 std::function<void(bool)> callback);

    QList<Roomdata> getListRooms() const;

    QString getImage() const;

    void setId(int newId);

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
