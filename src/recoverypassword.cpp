#include "recoverypassword.h"
#include "ui_recoverypassword.h"

#include <QApplication>
#include <QMessageBox>
#include <QScreen>

RecoveryPassword::RecoveryPassword(QWidget *parent, UserData *userData)
    : QWidget(parent)
    , m_userData(userData)
    , ui(new Ui::RecoveryPassword)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Popup);

    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    move(screenGeometry.center() - rect().center());
}

RecoveryPassword::~RecoveryPassword()
{
    delete ui;
}

void RecoveryPassword::on_sendEmailbtn_clicked()
{
    if (m_userData->getEmail() == "") {
        m_userData->checkEmail([&](bool haveEmail) {
            if (haveEmail) {
            } else
                QMessageBox::information(this,
                                         "Ошибка",
                                         "Установите почту в настройках профиля, чтобы возможно "
                                         "было восстановить пароль");
        });
    }
}
