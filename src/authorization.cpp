#include "includes/authorization.h"
#include "ui/ui_Authorization.h"

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

    bool authenticated = false;
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

        return true;
    }

    qDebug() << "oh no";
    return false;
}

void Authorization::on_button_registr_clicked()
{
    this->close();
    Registration* form = new Registration();
    form->show();
}


void Authorization::on_button_auth_clicked()
{
    connect();
    this->close();
    Homepage* form = new Homepage();
    form->show();
}

