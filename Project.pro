QT       += core gui sql core network printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/roomdetails.cpp \
    src/filterform.cpp \
    src/roomdetailsform.cpp \
    src/checkimage.cpp \
    src/homepage.cpp \
    src/registration.cpp \
    src/authorization.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/testui.cpp

HEADERS += \
    includes/roomdetails.h \
    includes/filterform.h \
    includes/checkimage.h \
    includes/homepage.h \
    includes/registration.h \
    includes/authorization.h \
    includes/mainwindow.h \
    includes/testui.h \
    includes/roomdetailsform.h


FORMS += \
    design/checkimage.ui \
    design/homepage.ui \
    design/registration.ui \
    design/Authorization.ui \
    design/mainwindow.ui \
    design/roomdetails.ui \
    design/testui.ui \
    design/roomdetailsform.ui \
    design/filterform.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += \
    "D:/PostgreSQL/include/"

LIBS += -L"D:/PostgreSQL/bin" -llibpq

RESOURCES += \
    resourses/resourse_homepage.qrc




