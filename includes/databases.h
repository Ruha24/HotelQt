#ifndef DATABASES_H
#define DATABASES_H

#include <QSqlQuery>
#include <QSqlError>
#include <qsqldatabase.h>

class Databases
{
public:
    Databases();

    bool connect();
    void closeDB();

private:
    QSqlDatabase db;
};
#endif // DATABASES_H
