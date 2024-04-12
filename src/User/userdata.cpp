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
    QObject::connect(reply, &QNetworkReply::finished, [=, &isValidUser]() mutable {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();

            if (responseData.startsWith("200") && responseData.contains("true"))
                isValidUser = true;
            else
                isValidUser = false;
        } else {
            qDebug() << "Error:" << reply->errorString();
        }

        reply->deleteLater();
        networkManager->deleteLater();

        if (callback)
            callback(isValidUser);
    });
}

void UserData::addUser(QString name, QString password)
{
    QNetworkAccessManager *networkManager = new QNetworkAccessManager();

    QJsonObject json;
    json["name"] = name;
    json["password"] = password;
    QJsonDocument jsonDoc(json);

    QNetworkRequest request(QUrl("http://localhost:555/addUser"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = networkManager->post(request, jsonDoc.toJson());

    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();

            if (response.contains("User")) {
                setUserName(name);
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

    json["name"] = userName;

    QJsonDocument jsonDoc(json);

    QNetworkRequest request(QUrl("http://localhost:555/checkEmail"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = networkManager->post(request, jsonDoc.toJson());

    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QString responseStr(response);

            int emailIndex = responseStr.indexOf("email: ");

            if (emailIndex != -1) {
                QString email = responseStr.mid(emailIndex + QString("email: ").length());
                setEmail(email);
                callback(true);
            } else {
                qDebug() << "Адрес электронной почты не найден в ответе сервера";
                callback(false);
            }
        } else {
            callback(false);
        }
    });
}
