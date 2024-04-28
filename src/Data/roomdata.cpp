#include "roomdata.h"

Roomdata::Roomdata(int id, QString type, int price, int count, QString description)
{
    this->id = id;
    this->typeRoom = type;
    this->startPrice = price;
    this->countRoom = count;
    this->description = description;
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

void Roomdata::getRooms(int countPlace, std::function<void(bool)> callback)
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
                    Roomdata roomD = Roomdata(room["idRoom"].toInt(),
                                              room["typeRoom"].toString(),
                                              room["price"].toInt(),
                                              room["count"].toInt(),
                                              room["description"].toString());

                    listRooms.append(roomD);
                }

                callback(true);
            }
        } else {
            callback(false);
        }
    });
}

QList<Roomdata> Roomdata::getListRooms() const
{
    return listRooms;
}
