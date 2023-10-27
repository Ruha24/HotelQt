#include "includes/authorization.h"
#include "ui_Authorization.h"

Authorization::Authorization(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Authorization)
{
    ui->setupUi(this);
}

Authorization::~Authorization()
{
    delete ui;
}

void Authorization::closeEvent(QCloseEvent *event)
{

    QMainWindow::closeEvent(event);
}



bool Authorization::connect()
{
    login = ui->login->text();
    password = ui->password->text();

    bool authenticated = false;

    QSqlQuery query;
    QList<QPair<QString, QString>> userCredentials;

    if (query.exec("SELECT name, password FROM users"))
    {
        while (query.next())
        {
            QString name = query.value(0).toString();
            QString password = query.value(1).toString();
            userCredentials.push_back(QPair<QString, QString>(name, password));
        }
    }

    for (const QPair<QString, QString>& credentials : userCredentials)
    {

            if (credentials.first == login && credentials.second == password)
            {
                qDebug() << "connect";
                authenticated = true;
                break;
            }
    }

    if (authenticated)
    {
            qDebug() << "kaif";
        return true;
    }

    return false;
}

void Authorization::on_login_2_clicked()
{
    connect();
    this->close();
        Homepage* form = new Homepage();
        form->show();

}


void Authorization::on_buttonReg_clicked()
{
    this->close();
    Registration* form = new Registration();
    form->show();
}

