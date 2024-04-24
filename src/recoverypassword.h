#ifndef RECOVERYPASSWORD_H
#define RECOVERYPASSWORD_H

#include <QRandomGenerator>
#include <QString>
#include <QWidget>
#include "Data/userdata.h"

#include "smtp.h"

namespace Ui {
class RecoveryPassword;
}

class RecoveryPassword : public QWidget
{
    Q_OBJECT

public:
    explicit RecoveryPassword(QWidget *parent = nullptr);
    ~RecoveryPassword();

    void send(QString to);

private slots:
    void on_sendEmailbtn_clicked();
    void mailSent(QString status);
    void on_acceptbtn_clicked();

signals:
    void emailSent();

private:
    QString generateRandomCode();
    QString recoveryCode = "";
    UserData *m_userData;

    Ui::RecoveryPassword *ui;
};

#endif // RECOVERYPASSWORD_H
