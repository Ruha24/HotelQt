#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QMainWindow>
#include <QMessageBox>
#include <QRegularExpression>

#include <QSqlQuery>
#include "includes/databases.h"
#include "includes/authorization.h"

namespace Ui { class registration;}

class Registration : public QMainWindow
{
    Q_OBJECT
public:
    Registration(QWidget *parent = nullptr);
    ~Registration();

private slots:
    void on_buttonReg_clicked();

private:
    bool checkPassword(const QString& str);
    Databases db;
    QMessageBox* messagebox;
    Ui::registration *ui;
};


#endif // REGISTRATION_H
