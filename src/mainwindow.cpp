#include "include/mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    server = new QTcpServer(this);

    connect(server, &QTcpServer::newConnection, this, &MainWindow::incommingconnection);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::incommingconnection()
{
    qDebug() << "New client connected";

    socket = new QTcpSocket;
    socket->setSocketDescriptor(server->nextPendingConnection()->socketDescriptor());

    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::clientDisconnected);

    Sockets.push_back(socket);
}

void MainWindow::clientDisconnected()
{
    QTcpSocket *disconnectedSocket = qobject_cast<QTcpSocket*>(sender());

    if (disconnectedSocket) {
        qDebug() << "Client disconnected";

        Sockets.removeOne(disconnectedSocket);

        disconnectedSocket->disconnect();
        disconnectedSocket->deleteLater();
    }
}


void MainWindow::slotReadyRead()
{
    socket = qobject_cast<QTcpSocket*>(sender());
    QByteArray requestData = socket->readAll();
    qDebug() << requestData;

    qDebug() << "read";
    QString requestDataStr(requestData);

    qDebug() << "requestData: " << requestDataStr;

    QStringList requestLines = requestDataStr.split("\r\n");
    if (requestLines.size() > 0) {
        QStringList requestParts = requestLines[0].split(" ");
        if (requestParts.size() == 3 && requestParts[0] == "GET") {
            QString path = requestParts[1];
            qDebug() << "path: " << path;

            if (path == "/select_user") {
                handleSelectUserRequest();
            } else {
                sendHttp404Response();
            }
        }
    }
    else {
        qDebug() << "Data error";
    }
}

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
void MainWindow::handleSelectUserRequest()
{
    QSqlQuery query;

    if (query.exec("SELECT name, password FROM users")) {
        if (query.size() > 0) {
            QJsonArray jsonArray;

            while (query.next()) {
                QString name = query.value(0).toString();
                QString password = query.value(1).toString();
                QJsonObject jsonObject;
                jsonObject["name"] = name;
                jsonObject["password"] = password;
                jsonArray.append(jsonObject);
            }

            QJsonObject jsonResponse;
            jsonResponse["status"] = "success";
            jsonResponse["data"] = jsonArray;

            QJsonDocument jsonDoc(jsonResponse);
            QString httpResponse = "HTTP/1.1 200 OK\r\n";
            httpResponse += "Content-Type: application/json\r\n\r\n";
            httpResponse += jsonDoc.toJson(QJsonDocument::Compact);

            qDebug() << "query: " << httpResponse;
            sendHttpData(httpResponse);
        } else {
            sendHttp404Response();
        }
    } else {
        qDebug() << "Query error: " << query.lastError().text();

    }
}

void MainWindow::sendHttp404Response()
{
    QString httpResponse = "HTTP/1.1 404 Not Found\r\n";
    httpResponse += "Content-Type: text/plain\r\n\r\n";
    httpResponse += "404 Not Found";

    sendHttpData(httpResponse);
}

void MainWindow::sendHttpData(const QString& data)
{
    socket->write(data.toUtf8());
    socket->waitForBytesWritten();
    socket->close();
}


void MainWindow::sendToClient(QString str)
{
    data.clear();
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);
    out << str;
    socket->write(data);
}


void MainWindow::on_pushButton_clicked()
{
    if(server->listen(QHostAddress::LocalHost, 555))
    {
        db.connect();
    }
}

