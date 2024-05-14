#include "userdata.h"

#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

UserData::UserData() {}

UserData::UserData(int idUser,
                   QString login,
                   QString password,
                   QString bdate,
                   QString name,
                   QString lastName,
                   QString email)
{
    this->idUser = idUser;
    this->user = login;
    this->password = password;
    this->bdate = bdate;
    this->userName = name;
    this->lastName = lastName;
    this->email = email;
}

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
            QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);

            QJsonObject response = responseDoc.object();

            if (response.contains("data")) {
                QJsonObject data = response["data"].toObject();
                if (data.contains("id") && data.contains("nameRole")) {
                    setUserId(data["id"].toInt());
                    setRole(data["nameRole"].toString());
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

    setUserName(name);
    setLastName(lastName);
    setBdate(date);
    setEmail(email);

    json["name"] = user;
    json["firstName"] = userName;
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
    json["name"] = user;

    QJsonDocument jsonDoc(json);

    QNetworkRequest request(QUrl("http://localhost:555/updatePass"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = networkManager->post(request, jsonDoc.toJson());

    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();

            if (response.contains("update")) {
                setPassword(pass);
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
                callback(false);
            }
        } else {
            callback(false);
        }

        reply->deleteLater();
        networkManager->deleteLater();
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
            listRecovery.clear();
            QByteArray responseData = reply->readAll();
            QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);

            QJsonObject response = responseDoc.object();
            QJsonArray jsonArray;

            if (response.contains("data")) {
                jsonArray = response["data"].toArray();

                for (const QJsonValue &roomValue : qAsConst(jsonArray)) {
                    QString startDateString = roomValue["startDate"].toString();
                    QDate startDate = QDate::fromString(startDateString, "yyyy-MM-dd");

                    QString lastDateString = roomValue["lastDate"].toString();
                    QDate lastDate = QDate::fromString(lastDateString, "yyyy-MM-dd");

                    QByteArray imageData = QByteArray::fromBase64(
                        roomValue["imageData"].toString().toUtf8());

                    QString imageName = roomValue["imageName"].toString();

                    int idRec = roomValue["recoveryId"].toInt();

                    QString imagePath = "src/" + imageName;
                    QFile imageFile(imagePath);
                    if (imageFile.open(QIODevice::WriteOnly)) {
                        imageFile.write(imageData);
                        imageFile.close();
                    } else {
                        qDebug() << "Failed to save image file: " << imagePath;
                        continue;
                    }

                    RecoveryData recovery = RecoveryData(idRec,
                                                         roomValue["roomName"].toString(),
                                                         roomValue["description"].toString(),
                                                         startDate,
                                                         lastDate,
                                                         imagePath);

                    listRecovery.append(recovery);
                }
                callback(true);
            }
        } else
            callback(false);

        reply->deleteLater();
        networkManager->deleteLater();
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
        reply->deleteLater();
        networkManager->deleteLater();
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
        reply->deleteLater();
        networkManager->deleteLater();
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
                    QString imageName = roomObject["imageName"].toString();

                    QByteArray imageData = QByteArray::fromBase64(
                        roomObject["imageData"].toString().toUtf8());

                    QString imagePath = "src/" + imageName;
                    QFile imageFile(imagePath);

                    if (!imageFile.exists()) {
                        if (imageFile.open(QIODevice::WriteOnly)) {
                            imageFile.write(imageData);
                            imageFile.close();
                        } else {
                            qDebug() << "Failed to save image file: " << imagePath;
                        }
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

void UserData::recoveryRoom(
    Roomdata *room, RecoveryData *data, QString card, int price, std::function<void(bool)> callback)
{
    QNetworkAccessManager *networkManager = new QNetworkAccessManager();

    QJsonObject json;

    json["idUser"] = idUser;
    json["idRoom"] = room->getId();
    json["startDate"] = data->getStartDate().toString();
    json["lastDate"] = data->getLastDate().toString();
    json["card"] = card;
    json["nowDate"] = QDate::currentDate().toString();
    json["price"] = price;

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

        reply->deleteLater();
        networkManager->deleteLater();
    });
}

void UserData::getUsers(std::function<void(bool)> callback)
{
    QNetworkAccessManager *networkManager = new QNetworkAccessManager();

    QNetworkRequest request(QUrl("http://localhost:555/getUsers"));

    QNetworkReply *reply = networkManager->get(request);

    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);

            QJsonObject response = responseDoc.object();
            QJsonArray jsonArray;

            if (response.contains("status") && response["status"].toString() == "success") {
                listUsers.clear();

                jsonArray = response["data"].toArray();

                for (const QJsonValue &user : qAsConst(jsonArray)) {
                    int idUser = user["id"].toInt();
                    QString name = user["name"].toString();
                    QString lastName = user["lastName"].toString();
                    QString login = user["login"].toString();
                    QString password = user["password"].toString();
                    QString email = user["email"].toString();
                    QString bdate = user["date"].toString();

                    UserData *dataUser
                        = new UserData(idUser, login, password, bdate, name, lastName, email);

                    listUsers.append(dataUser);
                }
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

QList<UserData *> UserData::getListUsers() const
{
    return listUsers;
}

QString UserData::getRole() const
{
    return role;
}

void UserData::setRole(const QString &newRole)
{
    role = newRole;
}

QList<Roomdata> UserData::getListRooms() const
{
    return listRooms;
}

QList<RecoveryData> UserData::getListRecovery() const
{
    return listRecovery;
}

QString UserData::getName() const
{
    return user;
}

void UserData::setName(const QString &newName)
{
    user = newName;
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
