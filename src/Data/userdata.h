#ifndef USERDATA_H
#define USERDATA_H

#include <QJsonArray>
#include <QList>
#include <QString>

#include "recoverydata.h"

class UserData
{
public:
    UserData();

    int getUserId() const;
    void setUserId(int id);

    void setValidUser(bool isValid);
    bool getValidUser();

    void getUserStats(QString name);
    void checkUser(QString name, QString password, std::function<void(bool)> callback);
    void addUser(QString name, QString password, QString email);

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

private:
    bool isValidUser = false;

    QString userName = "";
    QString password = "";
    QString email;

    QList<RecoveryData> listRecovery;

    int idUser = -1;
};

#endif // USERDATA_H
