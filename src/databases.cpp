#include "include/databases.h"
#include <QMessageBox>

Databases::Databases()
{
    connect();
}

bool Databases::connect()
{
    if (db.isValid() && db.isOpen()) {
        return true;
    }

    db = QSqlDatabase::addDatabase("QPSQL");
    db.setDatabaseName("Hotel");
    db.setHostName("172.20.7.8");
    db.setPort(5432);
    db.setUserName("st1991");
    db.setPassword("pwd1991");


    if (db.open()) {
        return true;
    } else {

        return false;
    }
}



void Databases::closeDB()
{
    QSqlDatabase::removeDatabase("Hotel");
    db.close();
}

