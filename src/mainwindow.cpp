#include "includes/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    manager = new QNetworkAccessManager(this);


}

MainWindow::~MainWindow()
{
    delete ui;
}

#include <QMessageBox>


void MainWindow::slotConnected()
{
    QMessageBox::information(this, "" , "Connected to the server");
}

void MainWindow::slotDisconnected()
{
    QMessageBox::information(this, "" ,"Disconnected from the server");

}


void MainWindow::connectToServer(const QString &serverAddress, quint16 serverPort)
{
    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::connected, this, &MainWindow::slotConnected);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::slotDisconnected);

    socket->connectToHost(serverAddress, serverPort);
}



void MainWindow::openForm()
{

    authorizationForm = new Authorization(this);
    authorizationForm->show();
}


void MainWindow::on_pushButton_clicked()
{
    openForm();
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->label->setText("xd");
    connectToServer("127.0.0.1", 555);
}


void MainWindow::selectUsers()
{
    ui->label->setText("request sending");

    QUrl url("http://localhost:555/select_user");
    QNetworkRequest request(url);

    QNetworkReply* reply = manager->get(request);

    QObject::connect(reply, &QNetworkReply::finished, this, &MainWindow::onRequestFinished);
}

void MainWindow::onRequestFinished()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (reply && reply->error() == QNetworkReply::NoError)
    {
        QByteArray responseData = reply->readAll();
        QString responseString(responseData);

        ui->label->setText(responseString);
    }
    else
    {
        QMessageBox::critical(this, "HTTP Request Error", (reply ? reply->errorString() : "Unknown error"));
    }

    reply->deleteLater();
}


void MainWindow::on_pushButton_3_clicked()
{
      selectUsers();
}

