#ifndef USERDATA_H
#define USERDATA_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QString>
#include <QtAlgorithms>

#include "recoverydata.h"
#include "roomdata.h"

class UserData
{
public:
    UserData();
    UserData(int idUser,
             QString login,
             QString password,
             QString bdate,
             QString name,
             QString lastName,
             QString email);

    int getUserId() const;
    void setUserId(int id);

    void setValidUser(bool isValid);
    bool getValidUser();

    void getUserStats(QString name, std::function<void(bool)> callback);
    void checkUser(QString name, QString password, std::function<void(bool)> callback);
    void addUser(QString name, QString password, QString email);
    void getIdUser(QString name, std::function<void(bool)> callback);

    QString getUserName() const;
    void setUserName(const QString &newUserName);

    void updateStats(QString name,
                     QString lastName,
                     QString date,
                     QString email,
                     std::function<void(bool)> callback);

    void updatePassword(QString pass, std::function<void(bool)> callback);

    QString getPassword() const;
    void setPassword(const QString &newPassword);

    QString getEmail() const;
    void setEmail(const QString &newEmail);
    void checkEmail(std::function<void(bool)> callback);

    void getUserRecovery(std::function<void(bool)> callback);

    QList<RecoveryData> getListRecovery() const;

    void sendRequest(const QString &url,
                     const QJsonObject &data,
                     std::function<void(const QByteArray &)> callback);

    QString getName() const;
    void setName(const QString &newName);

    QString getLastName() const;
    void setLastName(const QString &newLastName);

    QString getBdate() const;
    void setBdate(const QString &newBdate);

    void deleteRecovery(const RecoveryData &recovery, std::function<void(bool)> callback);

    void getRooms(std::function<void(bool)> callback);

    QList<Roomdata> getListRooms() const;

    void recoveryRoom(Roomdata *room, RecoveryData *data, std::function<void(bool)> callback);

    void updatePasswordonEmail(QString pass, std::function<void(bool)> callback);

    QString getRole() const;
    void setRole(const QString &newRole);

    void getUsers(std::function<void(bool)> callback);

    QList<UserData *> getListUsers() const;

private:
    bool isValidUser = false;

    QString name = "";
    QString lastName = " ";
    QString bdate = "";
    QString userName = "";
    QString password = "";
    QString email = "";
    QString role;

    QList<RecoveryData> listRecovery;
    QList<Roomdata> listRooms;
    QList<UserData *> listUsers;

    int idUser = -1;
};

#endif // USERDATA_H
