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

private:
    bool isValidUser = false;

    QString userName = "";
    int idUser = -1;
};

#endif // USERDATA_H
