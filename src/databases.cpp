#include "include/databases.h"

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
    db.setHostName("localhost");
    db.setPort(5432);
    db.setUserName("postgres");
    db.setPassword("admin");

    if (db.open()) {
        qDebug() << "Database connection established.";
        return true;
    } else {
        qDebug() << "Failed to connect to the database: " << db.lastError().text();
        return false;
    }
}



void Databases::closeDB()
{
    QSqlDatabase::removeDatabase("Hotel");
    db.close();
}

