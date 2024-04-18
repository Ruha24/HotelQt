#include "recoverypassword.h"
#include "ui_recoverypassword.h"

#include <QApplication>
#include <QMessageBox>
#include <QScreen>

RecoveryPassword::RecoveryPassword(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RecoveryPassword)
{
    ui->setupUi(this);

    m_userData = new UserData();

    setWindowFlags(Qt::Popup);

    ui->stackedWidget->setCurrentIndex(0);

    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    move(screenGeometry.center() - rect().center());
}

RecoveryPassword::~RecoveryPassword()
{
    delete ui;
}

void RecoveryPassword::send(QString to)
{
    Smtp *smtp = new Smtp("rusikaid50@gmail.com", "awzebgrcalfudrci", "smtp.gmail.com", 465);
    connect(smtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));
    smtp->sendMail("rusikaid50@gmail.com", to, "Hi", "hello");
}

void RecoveryPassword::mailSent(QString status)
{
    if (status == "Message sent") {
        ui->stackedWidget->setCurrentIndex(1);
    }
}

void RecoveryPassword::on_sendEmailbtn_clicked()
{
    if (ui->emaitxt->text() == "")
        return;

    m_userData->setEmail(ui->emaitxt->text());

    m_userData->checkEmail([=](bool haveEmail) {
        if (haveEmail) {
            send(m_userData->getEmail());
        } else {
            QMessageBox::information(this, "Ошибка", "Такого пользователя не существует");
        }
    });
}

void RecoveryPassword::on_acceptbtn_clicked()
{
    QString pass = ui->passwordtxt->text();
    QString confPass = ui->confirmpasstxt->text();

    if (pass == "" || confPass == "") {
        QMessageBox::information(this, "Ошибка", "Введите пароли");
        return;
    }

    if (pass != confPass) {
        QMessageBox::information(this, "Ошибка", "Вы ввели не одинаковые пароли");
        return;
    }
}
