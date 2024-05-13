#include "roomdata.h"

Roomdata::Roomdata(int id, QString type, int price, int count, QString description, QString image)
{
    this->id = id;
    this->typeRoom = type;
    this->startPrice = price;
    this->countRoom = count;
    this->description = description;
    this->image = image;
}

Roomdata::Roomdata() {}

int Roomdata::getId() const
{
    return id;
}

QString Roomdata::getTypeRoom() const
{
    return typeRoom;
}

int Roomdata::getStartPrice() const
{
    return startPrice;
}

int Roomdata::getCountRoom() const
{
    return countRoom;
}

QString Roomdata::getDescription() const
{
    return description;
}

void Roomdata::getRoomsSearch(int countPlace, std::function<void(bool)> callback)
{
    QNetworkAccessManager *networkManager = new QNetworkAccessManager();

    QJsonObject json;

    json["count"] = countPlace;

    QJsonDocument jsonDoc(json);

    QNetworkRequest request(QUrl("http://localhost:555/getSearchRooms"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = networkManager->post(request, jsonDoc.toJson());

    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);

            QJsonObject response = responseDoc.object();
            QJsonArray jsonArray;

            if (response.contains("data")) {
                listRooms.clear();
                jsonArray = response["data"].toArray();

                for (const QJsonValue &room : qAsConst(jsonArray)) {
                    QJsonObject roomObject = room.toObject();

                    int idRoom = roomObject["idRoom"].toInt();
                    QString typeRoom = roomObject["typeRoom"].toString();
                    int price = roomObject["price"].toInt();
                    int count = roomObject["count"].toInt();
                    QString description = roomObject["description"].toString();

                    QByteArray imageData = QByteArray::fromBase64(
                        roomObject["imageData"].toString().toUtf8());

                    QString imagePath = "src/" + QString::number(idRoom) + ".jpg";
                    QFile imageFile(imagePath);
                    if (imageFile.open(QIODevice::WriteOnly)) {
                        imageFile.write(imageData);
                        imageFile.close();
                    } else {
                        qDebug() << "Failed to save image file: " << imagePath;
                        continue;
                    }
                    Roomdata roomD = Roomdata(idRoom, typeRoom, price, count, description, imagePath);

                    listRooms.append(roomD);
                }

                callback(true);
            }
        } else {
            callback(false);
        }

        reply->deleteLater();
        networkManager->deleteLater();
    });
}

void Roomdata::getRooms(std::function<void(bool)> callback)
{
    QNetworkAccessManager *networkManager = new QNetworkAccessManager();

    QNetworkRequest request(QUrl("http://localhost:555/getRooms"));

    QNetworkReply *reply = networkManager->get(request);

    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);

            QJsonObject response = responseDoc.object();
            QJsonArray jsonArray;

            if (response.contains("data")) {
                listRooms.clear();
                jsonArray = response["data"].toArray();

                for (const QJsonValue &room : qAsConst(jsonArray)) {
                    QJsonObject roomObject = room.toObject();

                    int idRoom = roomObject["idRoom"].toInt();

                    Roomdata roomD = Roomdata();
                    roomD.setId(idRoom);

                    listRooms.append(roomD);
                }

                callback(true);
            }

        } else {
            callback(false);
        }

        reply->deleteLater();
        networkManager->deleteLater();
    });
}

void Roomdata::deleteRoom(int idRoom, std::function<void(bool)> callback)
{
    QNetworkAccessManager *networkManager = new QNetworkAccessManager();

    QJsonObject json;

    json["idRoom"] = idRoom;

    QJsonDocument jsonDoc(json);

    QNetworkRequest request(QUrl("http://localhost:555/deleteRoom"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = networkManager->post(request, jsonDoc.toJson());

    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            if (response.contains("success")) {
                callback(true);
            }
        } else {
            callback(false);
        }
        reply->deleteLater();
        networkManager->deleteLater();
    });
}

QList<Roomdata> Roomdata::getListRooms() const
{
    return listRooms;
}

QString Roomdata::getImage() const
{
    return image;
}

void Roomdata::setId(int newId)
{
    id = newId;
}
