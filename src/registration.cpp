#include "includes/registration.h"
#include "ui_registration.h"

Registration::Registration(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::registration)
{
    ui->setupUi(this);
}

Registration::~Registration()
{
    delete ui;
}

void Registration::closeEvent(QCloseEvent *event)
{
    this->close();
    Authorization* form = new Authorization();
    form->show();
    QMainWindow::closeEvent(event);
}

void Registration::on_buttonReg_clicked()
{
    QString login = ui->login_edit->text();
    QString password = ui->password_edit->text();
    QString currPassword = ui->confirmpass_edit->text();

    messagebox = new QMessageBox();

    if (login.length() < 1)
        messagebox->critical(this, "Ошибка", "login должен быть больше 1 символа");
    else if (!checkPassword(password))
        messagebox->critical(this, "Ошибка", "password не прошёл проверку, наведитесь на ввод (посмотрите что должен содержать)");
    else if (password != currPassword)
        messagebox->critical(this, "Ошибка", "повторный пароль не совпал");
    else
    {
        QJsonObject registrationData;
        registrationData["name"] = login;
        registrationData["password"] = password;

        QJsonDocument jsonDoc(registrationData);
        QByteArray jsonData = jsonDoc.toJson();

        QNetworkAccessManager networkManager;
        QNetworkRequest request(QUrl("http://localhost:555/add_user"));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QNetworkReply* reply = networkManager.post(request, jsonData);

        QEventLoop loop;
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        if (reply->error() == QNetworkReply::NoError)
        {
            QByteArray responseData = reply->readAll();
            QString responseString(responseData);

            if (responseString.contains("User already exists"))
            {
                QMessageBox::critical(this, "Registration Error", "Такой пользователь уже существует");
            }
            else if (responseString.contains("User registered successfully"))
            {
                QMessageBox::information(this, "Registration", "Регистрация успешна");
                close();
            }
            else
            {
                QMessageBox::critical(this, "HTTP Request Error", "Неизвестный ответ от сервера");
            }
        }
        else
        {
            QMessageBox::critical(this, "HTTP Request Error", reply->errorString());
        }

        reply->deleteLater();
    }
}


bool Registration::checkPassword(const QString &str)
{
    QRegularExpression passwordPattern("^(?=.*[A-Z])(?=.*[0-9])(?=.*[!@#$%^&*])[A-Za-z0-9!@#$%^&*]{8,}$");
    if(passwordPattern.match(str).hasMatch())
    {
        return true;
    }
    return false;
}

