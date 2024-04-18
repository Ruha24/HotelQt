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

    QString email = ui->email_edit->text();
    QRegularExpression emailRegex("\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Z|a-z]{2,}\\b");
    if (!emailRegex.match(email).hasMatch()) {
        QMessageBox::warning(this, "", "Некорректный адрес электронной почты");
        return;
    }

    QString password = ui->password_edit->text();
    QRegularExpression passwordRegex("^(?=.*[A-Z])(?=.*[!@#$%^&*])(?=.*[0-9]).{8,}$");
    if (!passwordRegex.match(password).hasMatch()) {
        QMessageBox::warning(
            this,
            "",
            "Некорректный пароль\nПароль должен состоять из:\n- Минимум 8 символов\n- Хотя бы "
            "одной заглавной буквы\n- Хотя бы одного специального символа\n- Хотя бы одной цифры");
        return;
    }

    m_userData->checkUser(ui->login_edit->text(), ui->password_edit->text(), [&](bool isValidUser) {
        if (isValidUser) {
            QMessageBox::information(this, "", "Такой пользователь уже существует");
        } else {
            m_userData->addUser(ui->login_edit->text(),
                                ui->password_edit->text(),
                                ui->email_edit->text());
            QMessageBox::information(this, "", "Вы успешно зарегестрировались");

            Authorization *auth = new Authorization(this);
            auth->show();
            this->close();
        }
    });
}
