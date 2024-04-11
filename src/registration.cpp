#include "registration.h"
#include "ui_registration.h"

#include <QApplication>
#include <QScreen>

Registration::Registration(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Registration)
{
    ui->setupUi(this);

    m_userData = new UserData();

    setWindowFlags(Qt::Popup);

    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    move(screenGeometry.center() - rect().center());
}

Registration::~Registration()
{
    delete ui;
}

void Registration::on_buttonReg_clicked()
{
    if (!m_userData) {
        qDebug() << "UserData is not initialized!";
        return;
    }

    if (ui->login_edit->text().isEmpty() || ui->password_edit->text().isEmpty()
        || ui->confirmpass_edit->text().isEmpty())
        return;

    if (ui->password_edit->text() != ui->confirmpass_edit->text())
        return;

    m_userData->checkUser(ui->login_edit->text(), ui->password_edit->text(), [&](bool isValidUser) {
        if (isValidUser) {
            QMessageBox::information(this, "", "Такой пользователь уже существует");
        } else {
            m_userData->addUser(ui->login_edit->text(), ui->password_edit->text());
            QMessageBox::information(this, "", "Вы успешно зарегестрировались");

            Authorization *auth = new Authorization(this);
            auth->show();

            this->close();
        }
    });
}
