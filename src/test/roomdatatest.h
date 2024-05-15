#ifndef ROOMDATATEST_H
#define ROOMDATATEST_H

#include <QObject>
#include <QtTest/QTest>
#include "../Data/roomdata.h"

class RoomDataTest
{
private slots:
    void testGetRooms();

private:
    Roomdata *roomData;
    bool callbackResult;

public:
    RoomDataTest();

private slots:
    void initTestCase() { roomData = new Roomdata(); }

    void cleanupTestCase() { delete roomData; }
};

#endif // ROOMDATATEST_H
