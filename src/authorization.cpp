#include "authorization.h"
#include "ui_authorization.h"

#include <QApplication>
#include <QScreen>

Authorization::Authorization(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Authorization)
{
    ui->setupUi(this);

    m_userData = new UserData();

    setWindowFlags(Qt::Popup);

    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    move(screenGeometry.center() - rect().center());
}

Authorization::~Authorization()
{
    delete ui;
}

void Authorization::on_buttonReg_clicked()
{
    Registration *regWindow = new Registration(this);

    regWindow->show();
}

void Authorization::on_login_2_clicked()
{
    QString userName = ui->login->text();
    QString password = ui->password->text();

    if (userName.isEmpty() || password.isEmpty())
        return;

    m_userData->checkUser(userName, password, [&, userName, password](bool isValidUser) {
        if (isValidUser) {
            m_userData->setUserName(userName);
            m_userData->setPassword(password);
            emit userAuthenticated(m_userData);
            QMessageBox::information(this, "", "Данные успешны");
        } else {
            emit userAuthenticated(m_userData);
            QMessageBox::information(this, "", "Вы ввели неправильные данные");
        }
    });
}

void Authorization::on_label_4_linkActivated(const QString &link)
{
    if (link == "1") {
        RecoveryPassword *recPass = new RecoveryPassword(this);
        recPass->show();
        this->close();
    }
}
