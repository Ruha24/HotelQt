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
    recoveryCode = generateRandomCode();
    smtp->sendMail("rusikaid50@gmail.com",
                   to,
                   "Luxury Hotel",
                   "Добрый день! Ваш проверочный код для доступа к сервису: " + recoveryCode
                       + " . Пожалуйста, введите этот "
                         "код в соответствующее поле.\n Если вы не запрашивали код, проигнорируйте "
                         "это сообщение.");
}

void RecoveryPassword::mailSent(QString status)
{
    if (status == "Message sent") {
        ui->stackedWidget->setCurrentIndex(1);
    } else {
        QMessageBox::information(this, "Ошибка", "Не верно указаная почта");
    }
}

void RecoveryPassword::on_sendEmailbtn_clicked()
{
    if (ui->emailtxt_2->text() == "")
        return;

    QString email = ui->emailtxt_2->text();

    QRegularExpression emailRegex("[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+.[A-Za-z]{2,}");
    if (!emailRegex.match(email).hasMatch()) {
        QMessageBox::warning(this, "", "Некорректный адрес электронной почты");
        return;
    }

    m_userData->setEmail(ui->emailtxt_2->text());

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

    if (ui->emailtxt->text() != recoveryCode) {
        QMessageBox::information(this, "Ошибка", "Вы ввели неправильный код подтверджения");
        return;
    }

    if (pass == "" || confPass == "") {
        QMessageBox::information(this, "Ошибка", "Введите пароли");
        return;
    }

    if (pass != confPass) {
        QMessageBox::information(this, "Ошибка", "Вы ввели не одинаковые пароли");
        return;
    }

    static QRegularExpression passwordRegex("^(?=.*[A-Z])(?=.*[!@#$%^&*])(?=.*[0-9]).{8,}$");
    if (!passwordRegex.match(pass).hasMatch()) {
        QMessageBox::warning(
            this,
            "",
            "Некорректный пароль\nПароль должен состоять из:\n- Минимум 8 символов\n- Хотя бы "
            "одной заглавной буквы\n- Хотя бы одного специального символа\n- Хотя бы одной цифры");
        return;
    }

    if (!passwordRegex.match(confPass).hasMatch()) {
        QMessageBox::warning(
            this,
            "",
            "Некорректный пароль\nПароль должен состоять из:\n- Минимум 8 символов\n- Хотя бы "
            "одной заглавной буквы\n- Хотя бы одного специального символа\n- Хотя бы одной цифры");
        return;
    }

    m_userData->updatePasswordonEmail(pass, [&](bool success) {
        if (success) {
            QMessageBox::information(this, "Пароль", "Пароль успешно изменён");
            this->close();
        }
    });
}

QString RecoveryPassword::generateRandomCode()
{
    QRandomGenerator *generator = QRandomGenerator::global();

    int code = generator->bounded(10000);

    return QString("%1").arg(code, 4, 10, QChar('0'));
}
