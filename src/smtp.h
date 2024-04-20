#ifndef SMTP_H
#define SMTP_H

#include <QByteArray>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QTextStream>
#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QSslSocket>
#include <QtWidgets/QMessageBox>

class Smtp : public QObject
{
    Q_OBJECT

public:
    Smtp(const QString &user,
         const QString &pass,
         const QString &host,
         int port = 465,
         int timeout = 30000);
    ~Smtp();

    void sendMail(const QString &from,
                  const QString &to,
                  const QString &subject,
                  const QString &body,
                  QStringList files = QStringList());

signals:
    void status(const QString &);

private slots:
    void stateChanged(QAbstractSocket::SocketState socketState);
    void errorReceived(QAbstractSocket::SocketError socketError);
    void disconnected();
    void connected();
    void readyRead();

private:
    int timeout;
    QString message;
    QTextStream *t;
    QSslSocket *socket;
    QString from;
    QString rcpt;
    QString response;
    QString user;
    QString pass;
    QString host;

    int port;
    enum states { Tls, HandShake, Auth, User, Pass, Rcpt, Mail, Data, Init, Body, Quit, Close };
    int state;
};
#endif
