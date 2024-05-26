#include "roomdatatest.h"

RoomDataTest::RoomDataTest() {}

void RoomDataTest::testGetRooms()
{
    roomData->getRooms([&](bool isSuccess) { callbackResult = isSuccess; });

    QTest::qWait(2000);

    QVERIFY2(callbackResult, "Комнаты были успешно получены");

    QVERIFY2(!roomData->getListRooms().isEmpty(), "Было получено 0 комнат");
}
