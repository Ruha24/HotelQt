#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QMainWindow>
#include <QMessageBox>
#include <QRegularExpression>

#include <QSqlQuery>
#include "includes/authorization.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


namespace Ui { class registration;}

class Registration : public QMainWindow
{
    Q_OBJECT
public:
    Registration(QWidget *parent = nullptr);
    ~Registration();

    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_buttonReg_clicked();

private:
    bool checkPassword(const QString& str);
    QMessageBox* messagebox;
    Ui::registration *ui;
};


#endif // REGISTRATION_H
