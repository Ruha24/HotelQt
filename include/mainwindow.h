#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>

#include <QCryptographicHash>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

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
    void clientDisconnected();
    void addNewClient(QTcpSocket* socket);

private:
    void sendHttpResponse(int statusCode, const QString &statusText);
    void sendHttpData(const QString &data);
    void getUserStats(const QString &name);
    bool checkUser(const QString &name, const QString &password);
    void handleCheckUser(const QString &data);
    void handleAddUser(const QString &data);
    void handleUpdateStats(const QString &data);
    void handleUpdatePass(const QString &data);
    void handleUpdateEmailPass(const QString &data);
    void handleCheckEmail(const QString &data);
    void handleUserRecovery(const QString &data);
    void handleUserId(const QString &data);
    void handleDeleteRecovery(const QString &data);
    void handleGetRooms();
    void handleDeleteRoom(const QString &data);
    void handleAddRoom(const QString &data);
    void handleSearchRooms(const QString &data);
    void handleAddRecovery(const QString &data);
    void handleGetUsers();

    QSet<QTcpSocket *> Sockets;
    QByteArray data;
    QTcpServer* server;

    bool connecting();
    QSqlDatabase db;

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
