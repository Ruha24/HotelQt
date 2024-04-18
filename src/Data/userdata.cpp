#include "userdata.h"

#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

UserData::UserData() {}

int UserData::getUserId() const
{
    return idUser;
}

void UserData::setUserId(int id)
{
    idUser = id;
}

void UserData::setValidUser(bool isValid)
{
    isValidUser = isValid;
}

bool UserData::getValidUser()
{
    return isValidUser;
}

void UserData::getUserStats(QString userName)
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

            bool ok;
            int userId = responseData.toInt(&ok);

            if (ok) {
                setUserId(userId);
            } else {
                qDebug() << "Error parsing userId from response";
                qDebug() << "Response Data:" << responseData;
            }
        } else {
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

QString UserData::getUserName() const
{
    return userName;
}

void UserData::setUserName(const QString &newUserName)
{
    userName = newUserName;
}

void UserData::updateStats(
    QString name, QString lastName, QString date, QString email, std::function<void(bool)> callback)
{
    QNetworkAccessManager *networkManager = new QNetworkAccessManager();

    QJsonObject json;

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

                    RecoveryData recovery = RecoveryData(roomValue["roomName"].toString(),
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

QList<RecoveryData> UserData::getListRecovery() const
{
    return listRecovery;
}
