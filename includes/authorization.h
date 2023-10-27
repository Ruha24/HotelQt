#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QMainWindow>
#include <QLabel>
#include "includes/databases.h"
#include "includes/registration.h"
#include "includes/homepage.h"

namespace Ui {
class Authorization;
}

class Authorization : public QMainWindow
{
    Q_OBJECT

public:
    explicit Authorization(QWidget *parent = nullptr);
    ~Authorization();

    void closeEvent(QCloseEvent* event);

private slots:
    bool connect();
    void on_login_2_clicked();
    void on_buttonReg_clicked();

private:
    QString login;
    QString password;

    Ui::Authorization *ui;
};

#endif // AUTHORIZATION_H
