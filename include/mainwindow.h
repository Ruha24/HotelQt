#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


#include "include/databases.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QTcpSocket *socket;


public slots:
    void incommingconnection();
    void slotReadyRead();
    void sendToClient(QString str);


private slots:
    void on_pushButton_clicked();
    void clientDisconnected();
    void addNewClient(QTcpSocket* socket);

private:

    void sendHttpResponse(int statusCode, const QString &statusText);
    void handleSelectUserRequest();
    void addUser(const QString& postData);
    void sendHttpData(const QString& data);

    QVector <QTcpSocket*> Sockets;
    QByteArray data;
    QTcpServer* server;

    void SentToClient();

    Databases db;

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
