#include "includes/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

// сделать, так чтобы все файлы хранились в создаваемой папке appdata|
//
