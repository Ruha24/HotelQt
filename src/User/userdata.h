#ifndef USERDATA_H
#define USERDATA_H

#include <QString>

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
    void addUser(QString name, QString password);

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

private:
    bool isValidUser = false;

    QString userName = "";
    QString password = "";

    int idUser = -1;
};

#endif // USERDATA_H
