#ifndef ROOMDATA_H
#define ROOMDATA_H

#include <QString>

class Roomdata
{
public:
    Roomdata(int id, QString type, int price, int count, QString description);

    int getId() const;

    QString getTypeRoom() const;

    int getStartPrice() const;

    int getCountRoom() const;

    QString getDescription() const;

private:
    int id;
    QString typeRoom;
    int startPrice;
    int countRoom;
    QString description;
};

#endif // ROOMDATA_H
