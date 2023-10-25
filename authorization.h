#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QMainWindow>
#include <QLabel>
#include "databases.h"

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
    void on_button_registr_clicked();

    void on_button_auth_clicked();

private:


    Databases db;
    QString login;
    QString password;

    Ui::Authorization *ui;
};

#endif // AUTHORIZATION_H
