QT       += core gui sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/homepage.cpp \
    src/registration.cpp \
    src/authorization.cpp \
    src/databases.cpp \
    src/main.cpp \
    src/mainwindow.cpp

HEADERS += \
    includes/homepage.h \
    includes/registration.h \
    includes/authorization.h \
    includes/databases.h \
    includes/mainwindow.h \


FORMS += \
    ui/homepage.ui \
    ui/registration.ui \
    ui/Authorization.ui \
    ui/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += \
    "D:/PostgreSQL/include/"

LIBS += -L"D:/PostgreSQL/bin" -llibpq




