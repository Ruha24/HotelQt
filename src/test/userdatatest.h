#ifndef USERDATATEST_H
#define USERDATATEST_H

#include <QObject>
#include <QtTest/QTest>
#include "../Data/userdata.h"

class UserDataTest : public QObject
{
    Q_OBJECT

private:
    UserData *userData;
    bool callbackResult;

public:
    UserDataTest();

private slots:
    void initTestCase() { userData = new UserData(); }
    void cleanupTestCase() { delete userData; }

    void testValidUser();
    void testAddUser();
};

#endif // USERDATATEST_H
