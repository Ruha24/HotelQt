#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QWidget>

#include <QMessageBox>
#include "User/userdata.h"
#include "registration.h"

namespace Ui {
class Authorization;
}

class Authorization : public QWidget
{
    Q_OBJECT

public:
    explicit Authorization(QWidget *parent = nullptr);
    ~Authorization();

    void xd();

signals:
    void userAuthenticated(UserData *userData);

private slots:
    void on_buttonReg_clicked();

    void on_login_2_clicked();

private:
    UserData *m_userData;

    Ui::Authorization *ui;
};

#endif // AUTHORIZATION_H
