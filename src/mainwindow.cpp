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

    if (server->listen(QHostAddress::LocalHost, 555)) {
        db.connect();
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::incommingconnection()
{

    while(server->hasPendingConnections())
    {
        qDebug() << "New client connected";
        addNewClient(server->nextPendingConnection());
    }
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

void MainWindow::addNewClient(QTcpSocket *socket)
{
    Sockets.append(socket);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::clientDisconnected);
}


void MainWindow::slotReadyRead()
{
    socket = qobject_cast<QTcpSocket*>(sender());
    qDebug() << socket;
    QByteArray requestData = socket->readAll();

    QString requestDataStr(requestData);

    QStringList requestLines = requestDataStr.split("\r\n");

    if (requestLines.size() > 0) {
        QStringList requestParts = requestLines[0].split(" ");

        if (requestParts.size() == 3) {
            QString path = requestParts[1];
            if (path == "/select_user") handleSelectUserRequest();
            else if(path == "/add_user") addUser(requestLines.last());
            else sendHttpResponse(404, "Not found");
        }
    }
    else {
        qDebug() << "Data error";
    }
}

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

            sendHttpData(httpResponse);
        } else {
            sendHttpResponse(404, "Not found");
        }
    } else {
        qDebug() << "Query error: " << query.lastError().text();

    }
}

void MainWindow::addUser(const QString& postData)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(postData.toUtf8());
    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        sendHttpResponse(400, "Bad request");
        return;
    }

    QJsonObject jsonObject = jsonDoc.object();

    QString login;
    QString password;

    if (jsonObject.contains("name")) {
        login = jsonObject["name"].toString();
    }

    if (jsonObject.contains("password")) {
        password = jsonObject["password"].toString();
    }

    QSqlQuery query;

    if (query.exec("Select name, password from users"))
    {
        while (query.next())
        {
            QString user = query.value(0).toString();
            if (login == user)
            {
                sendHttpData("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nUser already exists");
                return;
            }
        }

        if (query.exec("Insert into users(name, password) values('" + login + "', '" + password + "')"))
           sendHttpData("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nUser registered successfully");
    }
}

void MainWindow::sendHttpResponse(int statusCode, const QString &statusText)
{
    QString httpResponse = QString("HTTP/1.1 %1 %2\r\n").arg(statusCode).arg(statusText);
    httpResponse += "Content-Type: text/plain\r\n\r\n";
    httpResponse += QString("%1 %2").arg(statusCode).arg(statusText);

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

}

