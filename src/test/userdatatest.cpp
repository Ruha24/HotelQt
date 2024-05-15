#include "userdatatest.h"

UserDataTest::UserDataTest()
    : userData(nullptr)
    , callbackResult(false)
{}

void UserDataTest::testValidUser()
{
    QString validUsername = "ruslan";
    QString validPassword = "ruslan123!";

    userData->checkUser(validUsername, validPassword, [&](bool isValid) {
        callbackResult = isValid;
    });

    QTest::qWait(1000);

    QVERIFY2(callbackResult, "Тест прошёл проверку пользователя");
}

void UserDataTest::testAddUser()
{
    QString userName = "ruslan";
    QString password = "ruslan123";
    QString email = "test@gmail.com";

    userData->addUser(userName, password, email);

    QTest::qWait(2000);

    QCOMPARE(userData->getUserName(), userName);
    QCOMPARE(userData->getEmail(), email);
}
