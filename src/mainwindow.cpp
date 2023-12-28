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

    server->setMaxPendingConnections(30);

    if (server->listen(QHostAddress::LocalHost, 555)) {
        if(connecting())
        {
            ui->label->setText("Accept");
        }else
        {
            ui->label->setText("Error connect db");
        }
    }else
    {
        ui->label->setText("");
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
            qDebug() << requestParts;
            QString path = requestParts[1];
            if (path == "/select_user") handleSelectUserRequest();
            else if(path == "/add_user") addUser(requestLines.last());
            else if(path == "/get_role") getRole(requestLines.last());
            else if(path == "/getInformationUser") getInformation(requestLines.last());
            else if(path == "/searchRoom") searchRoom(requestLines.last());
            else if(path == "/search_room") search_room(requestLines.last());
            else if(path == "/uploadData") uploadData(requestData);
            else if(path == "/getType") getType();
            else sendHttpResponse(404, "Not found");
        }
    }
    else {
        qDebug() << "Data error";
    }
}


void MainWindow::uploadData(const QByteArray& requestData)
{
    QString fileName;

    QList<QByteArray> headers = requestData.split('\r\n');
    for (const QByteArray& header : headers) {
        if (header.startsWith("Content-Disposition: form-data") && header.contains("filename=")) {
            QList<QByteArray> parts = header.split(';');
            for (const QByteArray& part : parts) {
                if (part.trimmed().startsWith("filename=")) {
                    fileName = QString(part.split('"')[1]);
                    break;
                }
            }
            break;
        }
    }

    qDebug() << fileName;

    if (fileName.isEmpty()) {
        qDebug() << "Error: Unable to extract filename from request.";
        sendHttpResponse(400, "Bad Request");
        return;
    }

    if (InsertData(requestData, fileName)) {
        sendHttpResponse(200, "OK");
    } else {
        sendHttpResponse(500, "Internal Server Error");
    }
}


bool MainWindow::InsertData(const QByteArray& fileData, const QString& fileName)
{
    QString fileContent(fileData);

    QStringList lines = fileContent.split('\n');

    QSqlQuery query;

    if (fileName.contains("users", Qt::CaseInsensitive)) {

        foreach (const QString& line, lines) {
            QStringList values = line.split(',');

            if (values.size() == 6) {
                QString id = values[0].trimmed();
                QString name = values[1].trimmed();
                QString surname = values[2].trimmed();
                QString login = values[3].trimmed();
                QString password = values[4].trimmed();
                QString roleidStr = values[5].trimmed();

                bool ok;
                int roleid = roleidStr.toInt(&ok);

                if (ok) {
                    query.prepare("INSERT INTO Customers (name, surname, login, password, roleid) "
                                  "VALUES (:name, :surname, :login, :password, :roleid)");
                    query.bindValue(":name", name);
                    query.bindValue(":surname", surname);
                    query.bindValue(":login", login);
                    query.bindValue(":password", password);
                    query.bindValue(":roleid", roleid);

                    if (!query.exec()) {
                        qDebug() << "Error inserting data into the database:" << query.lastError().text();
                        return false;
                    }
                } else {
                    qDebug() << "Skipping invalid roleid:" << roleidStr;
                }
            }
        }
    } else if (fileName.contains("role", Qt::CaseInsensitive)) {

        foreach (const QString& line, lines) {
            QStringList values = line.split(',');

            if (values.size() == 2) {
                QString roleIdStr = values[0].trimmed();
                QString roleName = values[1].trimmed();

                bool ok;
                int roleid = roleIdStr.toInt(&ok);
                if (ok) {
                    query.prepare("INSERT INTO Role (name) VALUES (:name)");
                    query.bindValue(":name", roleName);

                    if (!query.exec()) {
                        qDebug() << "Error inserting data into the database:" << query.lastError().text();
                        return false;
                    }
                }else{
                    qDebug() << "Skipping invalid roleid:" << roleid;
                }


            }
        }
    }else if (fileName.contains("rooms", Qt::CaseInsensitive)) {
        foreach (const QString& line, lines) {
            QStringList values = line.split(',');

            if (values.size() == 6) {
                QString roomsId = values[0].trimmed();
                QString name = values[1].trimmed();
                QString typeroom = values[2].trimmed();
                double startprice = values[3].trimmed().toDouble();
                bool occupied = values[4].trimmed().toInt();
                QString image = values[5].trimmed();

                query.prepare("INSERT INTO rooms (name, typeroom, startprice, occupied, image) "
                              "VALUES (:name, :typeroom, :startprice, :occupied, :image)");
                query.bindValue(":name", name);
                query.bindValue(":typeroom", typeroom);
                query.bindValue(":startprice", startprice);
                query.bindValue(":occupied", occupied);
                query.bindValue(":image", image);

                if (!query.exec()) {
                    qDebug() << "Error inserting data into the database:" << query.lastError().text();
                    return false;
                }
            } else {
                qDebug() << "Skipping invalid data for rooms table:" << line;
            }
        }
    }

    else {
        qDebug() << "Unknown file type. Skipping file: " << fileName;
        return false;
    }

    qDebug() << "Data inserted into the database successfully!";
    return true;
}

void MainWindow::searchRoom(const QString &data)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8());

    if (!jsonDoc.isNull() && jsonDoc.isObject())
    {
        QJsonObject jsonObject = jsonDoc.object();
        if (jsonObject.contains("name"))
        {
            QString roomName = jsonObject["name"].toString();

            QSqlQuery query;
            query.prepare("SELECT * FROM rooms WHERE name = :name");
            query.bindValue(":name", roomName);

            if (query.exec())
            {
                QJsonArray roomsArray;

                while (query.next())
                {
                    QJsonObject roomObject;
                    roomObject["name"] = query.value("name").toString();
                    roomObject["typeroom"] = query.value("typeroom").toString();
                    roomObject["startprice"] = query.value("startprice").toDouble();
                    roomObject["occupied"] = query.value("occupied").toBool();
                    roomObject["image"] = query.value("image").toString();

                    roomsArray.append(roomObject);
                }

                QJsonObject jsonResponse;
                jsonResponse["status"] = "success";
                jsonResponse["data"] = roomsArray;

                QJsonDocument jsonDoc(jsonResponse);
                QString httpResponse = "HTTP/1.1 200 OK\r\n";
                httpResponse += "Content-Type: application/json\r\n\r\n";
                httpResponse += jsonDoc.toJson(QJsonDocument::Compact);

                sendHttpData(httpResponse);
            }
            else
            {
                qDebug() << "Query error: " << query.lastError().text();
                sendHttpResponse(500, "500 Internal Server Error");
            }
        }
        else
        {
            sendHttpResponse(400, "400 Bad Request");
        }
    }
    else
    {
        sendHttpResponse(400, "400 Bad Request");
    }
}


void MainWindow::search_room(const QString& searchData)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(searchData.toUtf8());

    if (!jsonDoc.isNull() && jsonDoc.isObject())
    {
        QJsonObject jsonObject = jsonDoc.object();

        if (jsonObject.contains("search_text") && jsonObject.contains("type") && jsonObject.contains("isOccupied"))
        {
            QString searchText = jsonObject["search_text"].toString();
            QString type = jsonObject["type"].toString();
            bool isOccupied = jsonObject["isOccupied"].toBool();

            QSqlQuery query;

            QString queryString = "SELECT * FROM rooms WHERE name LIKE :searchText AND typeroom LIKE :type AND occupied = :isOccupied";
            query.prepare(queryString);
            query.bindValue(":searchText", "%" + searchText + "%");
            query.bindValue(":type", "%" + type + "%");
            query.bindValue(":isOccupied", isOccupied);

            if (query.exec())
            {
                QJsonArray roomsArray;

                while (query.next())
                {
                    QJsonObject roomObject;
                    roomObject["name"] = query.value("name").toString();
                    roomObject["typeroom"] = query.value("typeroom").toString();
                    roomObject["startprice"] = query.value("startprice").toDouble();
                    roomObject["occupied"] = query.value("occupied").toBool();

                    roomsArray.append(roomObject);
                }

                QJsonObject jsonResponse;
                jsonResponse["status"] = "success";
                jsonResponse["data"] = roomsArray;

                QJsonDocument jsonDoc(jsonResponse);
                QString httpResponse = "HTTP/1.1 200 OK\r\n";
                httpResponse += "Content-Type: application/json\r\n\r\n";
                httpResponse += jsonDoc.toJson(QJsonDocument::Compact);

                sendHttpData(httpResponse);
            }
            else
            {
                qDebug() << "Query error: " << query.lastError().text();
                sendHttpResponse(500, "Internal Server Error");
            }
        }
        else
        {
            sendHttpResponse(400, "Bad Request");
        }
    }
    else
    {
        sendHttpResponse(400, "Bad Request");
    }
}


#include <QDir>

bool MainWindow::connecting()
{
    if (db.isValid() && db.isOpen()) {
        return true;
    }

    QString appDir = QCoreApplication::applicationDirPath();


    QCoreApplication::addLibraryPath(appDir);

    db = QSqlDatabase::addDatabase("QPSQL");
    db.setDatabaseName("Hotels");
    db.setHostName("172.20.7.8");
    db.setPort(5432);
    db.setUserName("st1991");
    db.setPassword("pwd1991");


    if (db.open()) {
        return true;
    } else {
        QMessageBox::information(this, "", "Failed to connect to the database: " + db.lastError().text());
        return false;
    }
}


void MainWindow::getInformation(const QString &name)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(name.toUtf8());

    if (!jsonDoc.isNull() && jsonDoc.isObject())
    {
        QJsonObject jsonObject = jsonDoc.object();
        if (jsonObject.contains("name")) {
            QString actualName = jsonObject["name"].toString();

            QSqlQuery query;
            query.prepare("SELECT name, surname, login, password FROM Customers WHERE login = :name");
            query.bindValue(":name", actualName);

            if (query.exec()) {
                if (query.next()) {
                    QJsonObject userObject;
                    userObject["name"] = query.value("name").toString();
                    userObject["surname"] = query.value("surname").toString();
                    userObject["login"] = query.value("login").toString();
                    userObject["password"] = query.value("password").toString();

                    QJsonObject jsonResponse;
                    jsonResponse["status"] = "success";
                    jsonResponse["data"] = userObject;

                    QJsonDocument jsonDoc(jsonResponse);
                    QString httpResponse = "HTTP/1.1 200 OK\r\n";
                    httpResponse += "Content-Type: application/json\r\n\r\n";
                    httpResponse += jsonDoc.toJson(QJsonDocument::Compact);

                    sendHttpData(httpResponse);
                } else {
                    sendHttpResponse(404, "User not found");
                }
            } else {
                qDebug() << "Query error: " << query.lastError().text();
                sendHttpResponse(500, "Internal Server Error");
            }
        }
    } else {
        sendHttpResponse(400, "Bad Request");
    }
}



void MainWindow::getRole(const QString& name)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(name.toUtf8());

    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        QJsonObject jsonObject = jsonDoc.object();
        if (jsonObject.contains("name")) {
            QString actualName = jsonObject["name"].toString();

            QSqlQuery query;

            if (query.exec("SELECT GetRoleName('" + actualName + "') AS role")) {
                if (query.next()) {
                    QString role = query.value("role").toString();

                    QJsonObject jsonResponse;
                    jsonResponse["status"] = "success";
                    jsonResponse["role"] = role;

                    QJsonDocument jsonDoc(jsonResponse);
                    QString httpResponse = "HTTP/1.1 200 OK\r\n";
                    httpResponse += "Content-Type: application/json\r\n\r\n";
                    httpResponse += jsonDoc.toJson(QJsonDocument::Compact);

                    sendHttpData(httpResponse);
                } else {
                    sendHttpResponse(404, "Role not found");
                }
            } else {
                qDebug() << "Query error: " << query.lastError().text();
            }
        } else {
            qDebug() << "Error: JSON does not contain 'name' field.";
        }
    } else {
        qDebug() << "Error: Invalid JSON format.";
    }
}

void MainWindow::getType()
{
    QSqlQuery query("SELECT DISTINCT typeroom FROM rooms");

    QJsonArray roomTypesArray;

    while (query.next()) {
        QString roomType = query.value("typeroom").toString();
        roomTypesArray.append(roomType);
    }

    QJsonObject responseObject;
    responseObject["room_types"] = roomTypesArray;

    QJsonDocument jsonResponse(responseObject);

    QString httpResponse = "HTTP/1.1 200 OK\r\n";
    httpResponse += "Content-Type: application/json\r\n\r\n";
    httpResponse += jsonResponse.toJson(QJsonDocument::Compact);

    sendHttpData(httpResponse);
}



void MainWindow::handleSelectUserRequest()
{
    QSqlQuery query;

    if (query.exec("SELECT login, password FROM Customers")) {
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

    if (query.exec("Select login, password from Customers"))
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

        if (query.prepare("INSERT INTO Customers (login, password) VALUES (:login, :password)")) {
            query.bindValue(":login", login);
            query.bindValue(":password", password);
            if (query.exec()) {
                 sendHttpData("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nUser registered successfully");
            } else {
                 sendHttpResponse(400, "Bad Reques");
            }
        }


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


