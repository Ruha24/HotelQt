#include "userdata.h"

#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

UserData::UserData() {}

void UserData::getIdUser(QString userName, std::function<void(bool)> callback)
{
    QNetworkAccessManager *networkManager = new QNetworkAccessManager();

    QJsonObject json;
    json["name"] = userName;
    QJsonDocument jsonDoc(json);

    QNetworkRequest request(QUrl("http://localhost:555/get_userId"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = networkManager->post(request, jsonDoc.toJson());

    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();

            QStringList parts = QString(responseData).split(' ');

            if (parts.size() == 2 && parts[0] == "200") {
                bool ok;
                int userId = parts[1].toInt(&ok);

                if (ok) {
                    setUserId(userId);
                    callback(true);
                } else {
                    callback(false);
                }
            } else {
                callback(false);
            }
        } else {
            callback(false);
            qDebug() << "Error:" << reply->errorString();
        }

        reply->deleteLater();
        networkManager->deleteLater();
    });
}

void UserData::checkUser(QString name, QString password, std::function<void(bool)> callback)
{
    QNetworkAccessManager *networkManager = new QNetworkAccessManager();

    QJsonObject json;
    json["name"] = name;
    json["password"] = password;
    QJsonDocument jsonDoc(json);

    QNetworkRequest request(QUrl("http://localhost:555/checkUser"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = networkManager->post(request, jsonDoc.toJson());

    bool isValidUser = false;
    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();

            if (responseData.startsWith("200") && responseData.contains("true"))
                callback(true);
            else
                callback(false);
        } else {
            qDebug() << "Error:" << reply->errorString();
        }

        reply->deleteLater();
        networkManager->deleteLater();
    });
}

void UserData::addUser(QString name, QString password, QString email)
{
    QNetworkAccessManager *networkManager = new QNetworkAccessManager();

    QJsonObject json;
    json["name"] = name;
    json["password"] = password;
    json["email"] = email;
    QJsonDocument jsonDoc(json);

    QNetworkRequest request(QUrl("http://localhost:555/addUser"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = networkManager->post(request, jsonDoc.toJson());

    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();

            if (response.contains("User")) {
                setUserName(name);
                setEmail(email);
            }
        }

        reply->deleteLater();
        networkManager->deleteLater();
    });
}

void UserData::updateStats(
    QString name, QString lastName, QString date, QString email, std::function<void(bool)> callback)
{
    QNetworkAccessManager *networkManager = new QNetworkAccessManager();

    QJsonObject json;

    setName(userName);
    setLastName(lastName);
    setBdate(date);
    setEmail(email);

    json["name"] = userName;
    json["firstName"] = name;
    json["lastName"] = lastName;
    json["date"] = date;
    json["email"] = email;

    QJsonDocument jsonDoc(json);

    QNetworkRequest request(QUrl("http://localhost:555/updateStats"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = networkManager->post(request, jsonDoc.toJson());

    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();

            if (response.contains("update")) {
                callback(true);
            } else {
                callback(false);
            }
        } else {
            callback(false);
        }

        reply->deleteLater();
        networkManager->deleteLater();
    });
}

void UserData::updatePassword(QString pass, std::function<void(bool)> callback)
{
    QNetworkAccessManager *networkManager = new QNetworkAccessManager();

    QJsonObject json;

    json["password"] = pass;
    json["name"] = userName;

    QJsonDocument jsonDoc(json);

    QNetworkRequest request(QUrl("http://localhost:555/updatePass"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = networkManager->post(request, jsonDoc.toJson());

    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();

            if (response.contains("update")) {
                callback(true);
            } else {
                callback(false);
            }

        } else {
            callback(false);
        }

        reply->deleteLater();
        networkManager->deleteLater();
    });
}

void UserData::updatePasswordonEmail(QString pass, std::function<void(bool)> callback)
{
    QNetworkAccessManager *networkManager = new QNetworkAccessManager();

    QJsonObject json;

    json["password"] = pass;
    json["email"] = email;

    QJsonDocument jsonDoc(json);

    QNetworkRequest request(QUrl("http://localhost:555/updatePassOnEmail"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = networkManager->post(request, jsonDoc.toJson());

    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();

            if (response.contains("success")) {
                callback(true);
            } else {
                callback(false);
            }

        } else {
            callback(false);
        }

        reply->deleteLater();
        networkManager->deleteLater();
    });
}

void UserData::checkEmail(std::function<void(bool)> callback)
{
    QNetworkAccessManager *networkManager = new QNetworkAccessManager();

    QJsonObject json;

    json["email"] = email;

    QJsonDocument jsonDoc(json);

    QNetworkRequest request(QUrl("http://localhost:555/checkEmail"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = networkManager->post(request, jsonDoc.toJson());

    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();

            if (response.contains("success")) {
                callback(true);
            } else {
                callback(true);
            }
        } else {
            callback(true);
        }
    });
}

void UserData::getUserRecovery(std::function<void(bool)> callback)
{
    QNetworkAccessManager *networkManager = new QNetworkAccessManager();

    QJsonObject json;

    json["userId"] = idUser;

    QJsonDocument jsonDoc(json);

    QNetworkRequest request(QUrl("http://localhost:555/getRecovery"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = networkManager->post(request, jsonDoc.toJson());

    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);

            QJsonObject response = responseDoc.object();
            QJsonArray jsonArray;

            if (response.contains("data")) {
                listRecovery.clear();
                jsonArray = response["data"].toArray();

                for (const QJsonValue &roomValue : jsonArray) {
                    QString startDateString = roomValue["startDate"].toString();
                    QDate startDate = QDate::fromString(startDateString, "yyyy-MM-dd");

                    QString lastDateString = roomValue["lastDate"].toString();
                    QDate lastDate = QDate::fromString(startDateString, "yyyy-MM-dd");

                    RecoveryData recovery = RecoveryData(roomValue["recoveryId"].toInt(),
                                                         roomValue["roomName"].toString(),
                                                         roomValue["description"].toString(),
                                                         startDate,
                                                         lastDate);

                    listRecovery.append(recovery);
                }

                callback(true);
            }
        } else
            callback(false);
    });
}

void UserData::getUserStats(QString userName, std::function<void(bool)> callback)
{
    QNetworkAccessManager *networkManager = new QNetworkAccessManager();

    QJsonObject json;

    json["name"] = userName;

    QJsonDocument jsonDoc(json);

    QNetworkRequest request(QUrl("http://localhost:555/getUserStats"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = networkManager->post(request, jsonDoc.toJson());

    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();

            QString responseString(response);

            QStringList dataList = responseString.split(", ");
            if (dataList.size() == 4) {
                QString name = dataList[0];
                QString email = dataList[1];
                QString lastname = dataList[2];
                QString bdate = dataList[3];

                setName(name);
                setEmail(email);
                setLastName(lastname);
                setBdate(bdate);
                callback(true);
            }
        } else {
            callback(false);
        }
    });
}

void UserData::deleteRecovery(const RecoveryData &recovery, std::function<void(bool)> callback)
{
    QNetworkAccessManager *networkManager = new QNetworkAccessManager();

    QJsonObject json;

    json["idRecovery"] = recovery.getId();

    QJsonDocument jsonDoc(json);

    QNetworkRequest request(QUrl("http://localhost:555/deleteRecovery"));
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
    });
}

void UserData::getRooms(std::function<void(bool)> callback)
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
    });
}

QList<Roomdata> UserData::getListRooms() const
{
    return listRooms;
}

void UserData::recoveryRoom(Roomdata *room, RecoveryData *data, std::function<void(bool)> callback)
{
    QNetworkAccessManager *networkManager = new QNetworkAccessManager();

    QJsonObject json;

    json["idUser"] = idUser;
    json["idRoom"] = room->getId();
    json["startDate"] = data->getStartDate().toString();
    json["lastDate"] = data->getLastDate().toString();

    QJsonDocument jsonDoc(json);

    QNetworkRequest request(QUrl("http://localhost:555/addRecovery"));
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
    });
}

QList<RecoveryData> UserData::getListRecovery() const
{
    return listRecovery;
}

QString UserData::getName() const
{
    return name;
}

void UserData::setName(const QString &newName)
{
    name = newName;
}

QString UserData::getLastName() const
{
    return lastName;
}

void UserData::setLastName(const QString &newLastName)
{
    lastName = newLastName;
}

QString UserData::getBdate() const
{
    return bdate;
}

void UserData::setBdate(const QString &newBdate)
{
    bdate = newBdate;
}

int UserData::getUserId() const
{
    return idUser;
}

void UserData::setUserId(int id)
{
    idUser = id;
}
QString UserData::getUserName() const
{
    return userName;
}

void UserData::setUserName(const QString &newUserName)
{
    userName = newUserName;
}

void UserData::setValidUser(bool isValid)
{
    isValidUser = isValid;
}

bool UserData::getValidUser()
{
    return isValidUser;
}

QString UserData::getPassword() const
{
    return password;
}

void UserData::setPassword(const QString &newPassword)
{
    password = newPassword;
}

QString UserData::getEmail() const
{
    return email;
}

void UserData::setEmail(const QString &newEmail)
{
    email = newEmail;
}
