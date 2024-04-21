#include "roomdata.h"

Roomdata::Roomdata(int id, QString type, int price, int count, QString description)
{
    this->id = id;
    this->typeRoom = type;
    this->startPrice = price;
    this->countRoom = count;
    this->description = description;
}

int Roomdata::getId() const
{
    return id;
}

QString Roomdata::getTypeRoom() const
{
    return typeRoom;
}

int Roomdata::getStartPrice() const
{
    return startPrice;
}

int Roomdata::getCountRoom() const
{
    return countRoom;
}

QString Roomdata::getDescription() const
{
    return description;
}
