/********************************************************************************
** Form generated from reading UI file 'Authorization.ui'
**
** Created by: Qt User Interface Compiler version 6.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AUTHORIZATION_H
#define UI_AUTHORIZATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Authorization
{
public:
    QWidget *centralwidget;
    QLabel *title;
    QLineEdit *login;
    QLineEdit *password;
    QPushButton *button_auth;
    QPushButton *button_registr;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Authorization)
    {
        if (Authorization->objectName().isEmpty())
            Authorization->setObjectName("Authorization");
        Authorization->resize(311, 349);
        centralwidget = new QWidget(Authorization);
        centralwidget->setObjectName("centralwidget");
        title = new QLabel(centralwidget);
        title->setObjectName("title");
        title->setGeometry(QRect(90, 10, 111, 71));
        QFont font;
        font.setPointSize(28);
        title->setFont(font);
        title->setAlignment(Qt::AlignCenter);
        login = new QLineEdit(centralwidget);
        login->setObjectName("login");
        login->setGeometry(QRect(90, 100, 113, 24));
        password = new QLineEdit(centralwidget);
        password->setObjectName("password");
        password->setGeometry(QRect(90, 150, 113, 24));
        button_auth = new QPushButton(centralwidget);
        button_auth->setObjectName("button_auth");
        button_auth->setGeometry(QRect(90, 200, 111, 41));
        button_registr = new QPushButton(centralwidget);
        button_registr->setObjectName("button_registr");
        button_registr->setGeometry(QRect(90, 260, 111, 41));
        Authorization->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Authorization);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 311, 21));
        Authorization->setMenuBar(menubar);
        statusbar = new QStatusBar(Authorization);
        statusbar->setObjectName("statusbar");
        Authorization->setStatusBar(statusbar);

        retranslateUi(Authorization);

        QMetaObject::connectSlotsByName(Authorization);
    } // setupUi

    void retranslateUi(QMainWindow *Authorization)
    {
        Authorization->setWindowTitle(QCoreApplication::translate("Authorization", "Authorization", nullptr));
        title->setText(QCoreApplication::translate("Authorization", "Hotel", nullptr));
        button_auth->setText(QCoreApplication::translate("Authorization", "Connect", nullptr));
        button_registr->setText(QCoreApplication::translate("Authorization", "Registation", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Authorization: public Ui_Authorization {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUTHORIZATION_H
