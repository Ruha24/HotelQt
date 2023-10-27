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

void Registration::on_buttonReg_clicked()
{
    QString login = ui->lineEdit->text();
    QString password = ui->password->text();
    QString currPassword = ui->curpass->text();

    messagebox = new QMessageBox();

    if(login.length() < 1)
        messagebox->critical(this, "Ошибка", "login должен быть больше 1 символа");


    else if(!checkPassword(password))
        messagebox->critical(this, "Ошибка", "password не прошёл проверку, наведитесь на ввод (посмотрите что должен содержать)");

    else if(password != currPassword)
       messagebox->critical(this, "Ошибка", "повторный пароль не совпал");
    else
    {
        if(db.connect()){
            QSqlQuery query;

            if (query.exec("select name from users")) {
                bool userExists = false;
                while (query.next()) {
                    QString user = query.value(0).toString();
                    if (user == login) {
                        userExists = true;
                        break;
                    }
                }

                if (userExists) {
                    messagebox->critical(this, "Ошибка", "Такой пользователь существует");
                } else {
                    if (query.exec("insert into users(name, password) values('" + login + "', '" + password + "')")) {
                        QMessageBox::information(this, "Добавление", "Вы зарегистрировались");

                        this->close();

                        Authorization *authForm = new Authorization();
                        authForm->show();
                    }
                }
            }
            else
                messagebox->critical(this, "Ошибка", "Не удалось выполнить запрос: " + query.lastError().text());
        }
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

