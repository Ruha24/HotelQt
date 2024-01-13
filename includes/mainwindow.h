#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

#include "includes/authorization.h"
#include "includes/testui.h"
#include "includes/checkimage.h"


// test

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void openForm();
    void connectToServer(const QString &serverAddress, quint16 serverPort);

    void on_pushButton_2_clicked();

    void slotConnected();
    void slotDisconnected();

    void on_pushButton_3_clicked();
    void onRequestFinished(QNetworkReply* reply);

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    // test
    void selectUsers();
    QNetworkAccessManager *manager;


    QTcpSocket* socket = nullptr;


    CheckImage* checkImage;
    Authorization* authorizationForm;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
