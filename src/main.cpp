#include "homepage.h"

#include <QApplication>
#include "test/roomdatatest.h"
#include "test/userdatatest.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HomePage w;

    UserDataTest userDataTest;
    QTest::qExec(&userDataTest);

    RoomDataTest roomDataTest;
    QTest::qExec(&roomDataTest);

    //w.show();
    w.showFullScreen();
    return a.exec();
}
