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
        QMessageBox::information(this,
                                 "",
                                 "Failed to connect to the database: " + db.lastError().text());
        return false;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::incommingconnection()
{
    while (server->hasPendingConnections()) {
        qDebug() << "New client connected";
        addNewClient(server->nextPendingConnection());
    }
}

void MainWindow::clientDisconnected()
{
    QTcpSocket *disconnectedSocket = qobject_cast<QTcpSocket *>(sender());

    if (disconnectedSocket) {
        qDebug() << "Client disconnected";

        Sockets.remove(disconnectedSocket);

        disconnectedSocket->disconnect();
        disconnectedSocket->deleteLater();
    }
}

void MainWindow::addNewClient(QTcpSocket *socket)
{
    Sockets.insert(socket);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::clientDisconnected);
}

void MainWindow::slotReadyRead()
{
    socket = qobject_cast<QTcpSocket *>(sender());
    qDebug() << socket;
    QByteArray requestData = socket->readAll();

    QString requestDataStr(requestData);

    QStringList requestLines = requestDataStr.split("\r\n");

    if (requestLines.size() > 0) {
        QStringList requestParts = requestLines[0].split(" ");

        if (requestParts.size() == 3) {
            QString path = requestParts[1];

            if (path == "/getUserStats")
                getUserStats(requestLines.last());
            else if (path == "/checkUser")
                handleCheckUser(requestLines.last());
            else if (path == "/addUser")
                handleAddUser(requestLines.last());
            else if (path == "/updateStats")
                handleUpdateStats(requestLines.last());
            else if (path == "/updatePass")
                handleUpdatePass(requestLines.last());
            else if (path == "/checkEmail")
                handleCheckEmail(requestLines.last());
            else if (path == "/getRecovery")
                handleUserRecovery(requestLines.last());
            else if (path == "/get_userId")
                handleUserId(requestLines.last());
            else if (path == "/deleteRecovery")
                handleDeleteRecovery(requestLines.last());
            else if (path == "/getRooms")
                handleGetRooms();
            else if (path == "/addRecovery")
                handleAddRecovery(requestLines.last());
            else if (path == "/updatePassOnEmail")
                handleUpdateEmailPass(requestLines.last());
            else if (path == "/getSearchRooms")
                handleSearchRooms(requestLines.last());
            else if (path == "/getUsers")
                handleGetUsers();
            else if (path == "/deleteRoom")
                handleDeleteRoom(requestLines.last());
            else if (path == "/addRoom")
                handleAddRoom(requestLines.last());
            else {
                sendHttpResponse(404, "Not found");
            }
        }
    } else {
        qDebug() << "Data error";
    }
}
void MainWindow::sendHttpResponse(int statusCode, const QString &statusText)
{
    QString httpResponse = QString("HTTP/1.1 %1 %2\r\n").arg(statusCode).arg(statusText);
    httpResponse += "Content-Type: text/plain\r\n\r\n";
    httpResponse += QString("%1 %2").arg(statusCode).arg(statusText);

    sendHttpData(httpResponse);
}

void MainWindow::getUserStats(const QString &data)
{
    QString userName;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8());
    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        QJsonObject jsonObject = jsonDoc.object();
        if (jsonObject.contains("name")) {
            userName = jsonObject["name"].toString();
        }
    }

    QSqlQuery query;
    query.prepare("SELECT firstName, email, lastname, bdate FROM users WHERE login = :name");
    query.bindValue(":name", userName);

    if (query.exec()) {
        if (query.next()) {
            QString name = query.value(0).toString();
            QString email = query.value(1).toString();
            QString lastname = query.value(2).toString();
            QString bdate = query.value(3).toString();
            QString response
                = QString("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n%1, %2, %3, %4")
                      .arg(name, email, lastname, bdate);
            sendHttpData(response);
        } else {
            sendHttpResponse(404, "User not found");
        }
    } else {
        sendHttpResponse(500, "Internal Server Error");
    }
}

void MainWindow::handleCheckUser(const QString &data)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8());

    QString userName, password;

    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        QJsonObject jsonObject = jsonDoc.object();

        if (jsonObject.contains("name") && jsonObject.contains("password")) {
            userName = jsonObject["name"].toString();
            password = jsonObject["password"].toString();
        }
        QByteArray passwordHash
            = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();

        QString hashPassword = QString::fromUtf8(passwordHash);

        QSqlQuery query;
        query.prepare("SELECT COUNT(*) FROM users WHERE login = :name AND password = :pass");
        query.bindValue(":name", userName);
        query.bindValue(":pass", hashPassword);

        QString isValidUser = "false";

        if (query.exec() && query.next()) {
            int userCount = query.value(0).toInt();
            if (userCount > 0)
                isValidUser = "true";
        } else {
            sendHttpResponse(500, "Internal Server Error");
            return;
        }

        sendHttpResponse(200, isValidUser);
    } else
        sendHttpResponse(404, "User not found");
}

void MainWindow::handleAddUser(const QString &data)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8());

    QString userName, password, email;

    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        QJsonObject jsonObject = jsonDoc.object();

        if (jsonObject.contains("name") && jsonObject.contains("password")
            && jsonObject.contains("email")) {
            userName = jsonObject["name"].toString();
            password = jsonObject["password"].toString();
            email = jsonObject["email"].toString();
        }
        QByteArray passwordHash = QCryptographicHash::hash(password.toUtf8(),
                                                           QCryptographicHash::Sha256)
                                      .toHex(); // Шифрование SHA256

        QString hashPassword = QString::fromUtf8(passwordHash);

        QSqlQuery query;
        query.prepare("insert into users(login, password, email) values(:name, :pass, :em)");
        query.bindValue(":name", userName);
        query.bindValue(":pass", hashPassword);
        query.bindValue(":em", email);

        if (query.exec()) {
            sendHttpResponse(200, "User registered successfully");
        } else
            sendHttpResponse(400, "Bad request");
    }
}

void MainWindow::handleUpdateStats(const QString &data)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8());

    QString userName, firstName, lastName, bDate, email;

    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        QJsonObject jsonObject = jsonDoc.object();

        if (jsonObject.contains("name") && jsonObject.contains("firstName")
            && jsonObject.contains("lastName") && jsonObject.contains("date")
            && jsonObject.contains("email")) {
            userName = jsonObject["name"].toString();
            firstName = jsonObject["firstName"].toString();
            lastName = jsonObject["lastName"].toString();
            bDate = jsonObject["date"].toString();
            email = jsonObject["email"].toString();
        }

        QString queryStr = QString("UPDATE users SET firstname = :firstName, lastname = :lastName, "
                                   "bdate = :bDate, email = :email WHERE login = :userName");

        QSqlQuery query;
        query.prepare(queryStr);
        query.bindValue(":firstName", firstName);
        query.bindValue(":lastName", lastName);
        query.bindValue(":bDate", bDate);
        query.bindValue(":email", email);
        query.bindValue(":userName", userName);

        if (query.exec()) {
            sendHttpResponse(200, "User stats update");
        } else {
            sendHttpResponse(400, "Bad request");
        }
    } else
        sendHttpResponse(404, "User not found");
}

void MainWindow::handleUpdatePass(const QString &data)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8());

    QString userName, password;

    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        QJsonObject jsonObject = jsonDoc.object();

        if (jsonObject.contains("name") && jsonObject.contains("password")) {
            userName = jsonObject["name"].toString();
            password = jsonObject["password"].toString();

            QByteArray passwordHash
                = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();

            QString hashPassword = QString::fromUtf8(passwordHash);

            QSqlQuery query;

            query.prepare("Update users set password = :pass where login = :name");
            query.bindValue(":name", userName);
            query.bindValue(":pass", hashPassword);

            if (query.exec())
                sendHttpResponse(200, "User password update");
            else
                sendHttpResponse(400, "Bad request");
        }
    } else
        sendHttpResponse(404, "User not found");
}

void MainWindow::handleUpdateEmailPass(const QString &data)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8());

    QString email, password;

    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        QJsonObject jsonObject = jsonDoc.object();

        if (jsonObject.contains("email") && jsonObject.contains("password")) {
            email = jsonObject["email"].toString();
            password = jsonObject["password"].toString();

            QByteArray passwordHash
                = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();

            QString hashPassword = QString::fromUtf8(passwordHash);

            QSqlQuery query;

            query.prepare("Update users set password = :pass where email = :email");
            query.bindValue(":email", email);
            query.bindValue(":pass", hashPassword);

            if (query.exec())
                sendHttpResponse(200, "success");
            else
                sendHttpResponse(400, "Bad request");
        }
    } else
        sendHttpResponse(404, "User not found");
}

void MainWindow::handleCheckEmail(const QString &data)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8());

    QString email;

    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        QJsonObject jsonObject = jsonDoc.object();

        if (jsonObject.contains("email")) {
            email = jsonObject["email"].toString();

            QSqlQuery query;
            query.prepare("SELECT COUNT(*) FROM users WHERE email = :email");
            query.bindValue(":email", email);

            if (query.exec()) {
                if (query.next()) {
                    int count = query.value(0).toInt();
                    if (count > 0) {
                        sendHttpResponse(200, "success");
                    } else {
                        sendHttpResponse(404, "Email not found");
                    }
                } else {
                    sendHttpResponse(500, "Database error");
                }
            } else {
                sendHttpResponse(500, "Query execution error");
            }
        }
    }
}

void MainWindow::handleUserRecovery(const QString &data)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8());

    int idUser;

    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        QJsonObject jsonObject = jsonDoc.object();

        if (jsonObject.contains("userId")) {
            idUser = jsonObject["userId"].toInt();

            QSqlQuery query;

            query.prepare("SELECT rooms.typeroom, rooms.description, booking.*, image.imagename "
                          "FROM booking JOIN "
                          "rooms ON booking.idroom "
                          "= rooms.id inner join image on rooms.image = image.id WHERE "
                          "booking.\"idUser\" = :id");

            query.bindValue(":id", idUser);

            if (query.exec()) {
                QJsonArray recoveryArr;

                while (query.next()) {
                    QJsonObject recoveryObj;

                    QDate startDate = query.value("booking.startDate").toDate();
                    QDate lastDate = query.value("booking.lastDate").toDate();

                    recoveryObj["recoveryId"] = query.value("booking.id").toInt();
                    recoveryObj["roomName"] = query.value("rooms.typeroom").toString();
                    recoveryObj["startDate"] = startDate.toString(Qt::ISODate);
                    recoveryObj["lastDate"] = lastDate.toString(Qt::ISODate);
                    recoveryObj["description"] = query.value("rooms.description").toString();

                    QString imageName = query.value("imagename").toString();
                    recoveryObj["imageName"] = imageName;
                    QString imagePath = "image/" + imageName;

                    QFile imageFile(imagePath);
                    if (imageFile.open(QIODevice::ReadOnly)) {
                        QByteArray imageData = imageFile.readAll();
                        recoveryObj["imageData"] = QString(imageData.toBase64());
                        imageFile.close();
                    } else {
                        qDebug() << "Failed to open image: " << imageName;
                        continue;
                    }

                    recoveryArr.append(recoveryObj);
                }

                QJsonObject jsonResponse;
                jsonResponse["status"] = "success";
                jsonResponse["data"] = recoveryArr;

                QJsonDocument jsonDoc(jsonResponse);
                QString httpResponse = "HTTP/1.1 200 OK\r\n";
                httpResponse += "Content-Type: application/json\r\n\r\n";
                httpResponse += jsonDoc.toJson(QJsonDocument::Compact);

                sendHttpData(httpResponse);
            }
        }
    }
}

void MainWindow::handleUserId(const QString &data)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8());

    QString name;

    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        QJsonObject jsonObject = jsonDoc.object();

        if (jsonObject.contains("name")) {
            name = jsonObject["name"].toString();

            QSqlQuery query;

            query.prepare(
                "select users.id,  role.name  from users inner join role on users.\"idRole\" = "
                "role.id where login = :user");
            query.bindValue(":user", name);

            if (query.exec()) {
                if (query.next()) {
                    QJsonObject object;

                    object["id"] = query.value(0).toInt();
                    object["nameRole"] = query.value(1).toString();

                    QJsonObject jsonResponse;
                    jsonResponse["status"] = "success";
                    jsonResponse["data"] = object;

                    QJsonDocument jsonDoc(jsonResponse);
                    QString httpResponse = "HTTP/1.1 200 OK\r\n";
                    httpResponse += "Content-Type: application/json\r\n\r\n";
                    httpResponse += jsonDoc.toJson(QJsonDocument::Compact);

                    sendHttpData(httpResponse);
                } else {
                    sendHttpResponse(400, "Bad request");
                }
            }
        }
    }
}

void MainWindow::handleDeleteRecovery(const QString &data)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8());

    int idRec;

    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        QJsonObject jsonObject = jsonDoc.object();

        if (jsonObject.contains("idRecovery")) {
            idRec = jsonObject["idRecovery"].toInt();

            QSqlQuery paymentQuery;

            paymentQuery.prepare("delete from payment where idbooking = :idRec");
            paymentQuery.bindValue(":idRec", idRec);

            if (paymentQuery.exec()) {
                QSqlQuery query;

                query.prepare("delete from booking where id = :idRec");
                query.bindValue(":idRec", idRec);

                if (query.exec()) {
                    sendHttpResponse(200, "success");
                } else {
                    sendHttpResponse(400, "Bad request");
                }
            } else
                sendHttpResponse(400, "Bad request");
        }
    }
}

void MainWindow::handleGetRooms()
{
    QSqlQuery query;

    query.prepare(
        "select  rooms.*, image.imagename from rooms inner join image on rooms.image = image.id");

    if (query.exec()) {
        QJsonArray roomsArr;

        while (query.next()) {
            QJsonObject roomObj;

            roomObj["idRoom"] = query.value(0).toInt();
            roomObj["typeRoom"] = query.value(1).toString();
            roomObj["price"] = query.value(2).toInt();
            roomObj["description"] = query.value(4).toString();
            roomObj["count"] = query.value(5).toInt();

            QString imageName = query.value(6).toString();
            roomObj["imageName"] = imageName;
            QString imagePath = "image/" + imageName;

            QFile imageFile(imagePath);
            if (imageFile.open(QIODevice::ReadOnly)) {
                QByteArray imageData = imageFile.readAll();
                roomObj["imageData"] = QString(imageData.toBase64());
                imageFile.close();
            } else {
                continue;
            }

            roomsArr.append(roomObj);
        }

        QJsonObject jsonResponse;
        jsonResponse["status"] = "success";
        jsonResponse["data"] = roomsArr;

        QJsonDocument jsonDoc(jsonResponse);
        QString httpResponse = "HTTP/1.1 200 OK\r\n";
        httpResponse += "Content-Type: application/json\r\n\r\n";
        httpResponse += jsonDoc.toJson(QJsonDocument::Compact);

        sendHttpData(httpResponse);
    }
}

void MainWindow::handleDeleteRoom(const QString &data)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8());

    int idRoom;

    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        QJsonObject jsonObject = jsonDoc.object();

        if (jsonObject.contains("idRoom")) {
            idRoom = jsonObject["idRoom"].toInt();

            QSqlQuery getBookId;
            getBookId.prepare("select id from booking where idroom = :id");
            getBookId.bindValue(":id", idRoom);

            if (!getBookId.exec()) {
                sendHttpResponse(400, "Bad request");
                return;
            }

            int idBooking = 0;

            if (getBookId.next()) {
                idBooking = getBookId.value(0).toInt();
            }

            QSqlQuery deletePayment;
            deletePayment.prepare("delete from payment where idbooking = :id");
            deletePayment.bindValue(":id", idBooking);

            if (!deletePayment.exec()) {
                sendHttpResponse(400, "Bad request");
                return;
            }

            QSqlQuery deleteBooking;
            deleteBooking.prepare("delete from booking where idroom = :id");
            deleteBooking.bindValue(":id", idRoom);

            if (!deleteBooking.exec()) {
                sendHttpResponse(400, "Bad request");
                return;
            }

            QSqlQuery roomQuery;
            roomQuery.prepare("Delete from rooms where id = :idRoom");
            roomQuery.bindValue(":idRoom", idRoom);

            if (roomQuery.exec()) {
                sendHttpResponse(200, "success");
            } else {
                sendHttpResponse(400, "Bad request");
            }
        }
    } else {
        sendHttpResponse(503, "Server error");
    }
}

void MainWindow::handleAddRoom(const QString &data)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8());

    if (!jsonDoc.isObject()) {
        qDebug() << "Invalid JSON data:" << data;
        sendHttpResponse(400, "Bad request: Invalid JSON data");
        return;
    }

    QJsonObject jsonObject = jsonDoc.object();

    if (!jsonObject.contains("roomName") || !jsonObject.contains("price")
        || !jsonObject.contains("description") || !jsonObject.contains("image")
        || !jsonObject.contains("imageName")) {
        qDebug() << "Missing or invalid JSON fields:" << data;
        sendHttpResponse(400, "Bad request: Missing or invalid JSON fields");
        return;
    }

    QString roomName = jsonObject["roomName"].toString();
    QString description = jsonObject["description"].toString();
    int price = jsonObject["price"].toInt();
    QString imageName = jsonObject["imageName"].toString();
    QString imageBase64 = jsonObject["image"].toString();

    qDebug() << "Room Name:" << roomName;
    qDebug() << "Description:" << description;
    qDebug() << "Price:" << price;
    qDebug() << "Image Name:" << imageName;

    QByteArray imageData = QByteArray::fromBase64(imageBase64.toUtf8());

    QString imagePath = "image/" + imageName;
    QFile file(imagePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to save image:" << imagePath;
        sendHttpResponse(500, "Internal server error: Failed to save image");
        return;
    }
    file.write(imageData);
    file.close();

    qDebug() << "Image saved to:" << imagePath;

    QSqlQuery query;

    query.prepare("INSERT INTO image (imagename) VALUES (:image)");
    query.bindValue(":image", imageName);
    if (!query.exec()) {
        qDebug() << "Failed to insert image into database:" << query.lastError().text();
        sendHttpResponse(500, "Internal server error: Failed to insert image into database");
        return;
    }
    int idImage = query.lastInsertId().toInt();

    qDebug() << "Image inserted into database with ID:" << idImage;

    query.prepare("INSERT INTO rooms (typeroom, startprice, image, description) "
                  "VALUES (:name, :price, :image, :desc)");
    query.bindValue(":name", roomName);
    query.bindValue(":price", price);
    query.bindValue(":image", idImage);
    query.bindValue(":desc", description);
    if (!query.exec()) {
        qDebug() << "Failed to insert room into database:" << query.lastError().text();
        sendHttpResponse(500, "Internal server error: Failed to insert room into database");
        return;
    }

    qDebug() << "Room added successfully";
    sendHttpResponse(200, "Success: Room added successfully");
}

void MainWindow::handleSearchRooms(const QString &data)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8());

    int count;

    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        QJsonObject jsonObject = jsonDoc.object();

        if (jsonObject.contains("count")) {
            count = jsonObject["count"].toInt();

            QSqlQuery query;

            query.prepare("select  rooms.*, image.imagename from rooms inner join image on "
                          "rooms.image = image.id  where \"countPlace\" <= :countP");
            query.bindValue(":countP", count);

            if (query.exec()) {
                QJsonArray roomsArr;

                while (query.next()) {
                    QJsonObject roomObj;

                    roomObj["idRoom"] = query.value(0).toInt();
                    roomObj["typeRoom"] = query.value(1).toString();
                    roomObj["price"] = query.value(2).toInt();
                    roomObj["description"] = query.value(4).toString();
                    roomObj["count"] = query.value(5).toInt();

                    QString imageName = query.value(6).toString();

                    roomObj["imageName"] = imageName;
                    QString imagePath = "image/" + imageName;

                    QFile imageFile(imagePath);
                    if (imageFile.open(QIODevice::ReadOnly)) {
                        QByteArray imageData = imageFile.readAll();
                        roomObj["imageData"] = QString(imageData.toBase64());
                        imageFile.close();
                    } else {
                        qDebug() << "Failed to open image: " << imagePath;
                    }

                    roomsArr.append(roomObj);
                }

                QJsonObject jsonResponse;
                jsonResponse["status"] = "success";
                jsonResponse["data"] = roomsArr;

                QJsonDocument jsonDoc(jsonResponse);
                QString httpResponse = "HTTP/1.1 200 OK\r\n";
                httpResponse += "Content-Type: application/json\r\n\r\n";
                httpResponse += jsonDoc.toJson(QJsonDocument::Compact);

                sendHttpData(httpResponse);
            }
        }
    }
}

void MainWindow::handleAddRecovery(const QString &data)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8());

    QDate startDate, lastDate, nowDate;

    QString card;

    int idUser, idRoom, price;

    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        QJsonObject jsonObject = jsonDoc.object();

        if (jsonObject.contains("idUser") && jsonObject.contains("idRoom")
            && jsonObject.contains("startDate") && jsonObject.contains("lastDate")
            && jsonObject.contains("card") && jsonObject.contains("nowDate")
            && jsonObject.contains("price")) {
            idUser = jsonObject["idUser"].toInt();
            idRoom = jsonObject["idRoom"].toInt();
            price = jsonObject["price"].toInt();

            qDebug() << idUser;
            qDebug() << idRoom;

            QString inputStartDate = jsonObject["startDate"].toString();
            QString inputLastDate = jsonObject["lastDate"].toString();
            QString inputNowDate = jsonObject["nowDate"].toString();

            startDate = QDate::fromString(inputStartDate, "ddd MMM dd yyyy");
            lastDate = QDate::fromString(inputLastDate, "ddd MMM dd yyyy");
            nowDate = QDate::fromString(inputNowDate, "ddd MMM dd yyyy");

            card = jsonObject["card"].toString();

            QSqlQuery checkQuery;
            checkQuery.prepare(
                "SELECT COUNT(*) FROM booking WHERE \"idUser\" = :user AND idroom = :room");
            checkQuery.bindValue(":user", idUser);
            checkQuery.bindValue(":room", idRoom);
            if (checkQuery.exec() && checkQuery.next()) {
                int existingCount = checkQuery.value(0).toInt();

                if (existingCount >= 1) {
                    sendHttpResponse(400, "Booking already exists for this user and room");
                    return;
                }
            } else {
                qDebug() << "Error";
                sendHttpResponse(500, "Internal server error");
                return;
            }

            QSqlQuery bookingQuery;

            bookingQuery.prepare(
                "INSERT INTO booking(\"idUser\", idroom, startDate, lastDate) VALUES(:user, "
                ":room, :start, :last);");

            bookingQuery.bindValue(":user", idUser);
            bookingQuery.bindValue(":room", idRoom);
            bookingQuery.bindValue(":start", startDate);
            bookingQuery.bindValue(":last", lastDate);

            if (bookingQuery.exec()) {
                int idBook = bookingQuery.lastInsertId().toInt();

                qDebug() << "Дата: " << nowDate;
                qDebug() << "ид бро: " << idBook;
                qDebug() << "цена: " << price;
                qDebug() << "card: " << card;

                QSqlQuery paymentQuery;
                paymentQuery.prepare("insert into payment(idbooking, price, datepayment, card) "
                                     "values(:idBook, :price, :now, :card);");
                paymentQuery.bindValue(":idBook", idBook);
                paymentQuery.bindValue(":price", price);
                paymentQuery.bindValue(":now", nowDate);
                paymentQuery.bindValue(":card", card);

                if (paymentQuery.exec()) {
                    QJsonObject jsonResponse;
                    jsonResponse["status"] = "success";

                    QJsonDocument jsonDoc(jsonResponse);
                    QString httpResponse = "HTTP/1.1 200 OK\r\n";
                    httpResponse += "Content-Type: application/json\r\n\r\n";
                    httpResponse += jsonDoc.toJson(QJsonDocument::Compact);

                    sendHttpData(httpResponse);
                } else
                    sendHttpResponse(400, "Bad request");
            } else {
                sendHttpResponse(400, "Bad request");
            }
        }
    }
}

void MainWindow::handleGetUsers()
{
    QSqlQuery query("SELECT * FROM users");

    QJsonArray usersArr;

    while (query.next()) {
        QJsonObject user;

        user["id"] = query.value("id").toInt();
        user["name"] = query.value("firstname").toString();
        user["lastName"] = query.value("lastname").toString();
        user["login"] = query.value("login").toString();
        user["password"] = query.value("password").toString();
        user["email"] = query.value("email").toString();
        user["date"] = query.value("bdate").toString();

        usersArr.append(user);
    }

    QJsonObject jsonResponse;
    jsonResponse["status"] = "success";
    jsonResponse["data"] = usersArr;

    QJsonDocument jsonDoc(jsonResponse);
    QString httpResponse = "HTTP/1.1 200 OK\r\n";
    httpResponse += "Content-Type: application/json\r\n\r\n";
    httpResponse += jsonDoc.toJson(QJsonDocument::Compact);

    sendHttpData(httpResponse);
}

void MainWindow::sendHttpData(const QString &data)
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
