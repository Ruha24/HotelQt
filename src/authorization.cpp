#include "includes/authorization.h"
#include "ui_Authorization.h"

Authorization::Authorization(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Authorization)
{
    ui->setupUi(this);
}

Authorization::~Authorization()
{
    delete ui;
}

bool Authorization::connect()
{
    login = ui->login->text();
    password = ui->password->text();

    bool authenticated = false;

    if (login.isEmpty() || password.isEmpty())
        return false;


    QNetworkAccessManager networkManager;
    QNetworkRequest request(QUrl("http://localhost:555/select_user"));
    QNetworkReply* reply = networkManager.get(request);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray responseData = reply->readAll();

        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);

        if (!jsonResponse.isNull())
        {
            if (jsonResponse.isObject())
            {
                QJsonObject jsonObject = jsonResponse.object();

                if (jsonObject.contains("data"))
                {
                    QJsonValue dataValue = jsonObject["data"];
                    if (dataValue.isArray())
                    {
                        QJsonArray dataArray = dataValue.toArray();

                        for (const QJsonValue& item : dataArray)
                        {
                            QJsonObject itemObject = item.toObject();
                            QString name = itemObject["name"].toString();
                            QString jsonPassword = itemObject["password"].toString();

                            if (name == login && jsonPassword == password)
                            {
                                qDebug() << "connect";
                                authenticated = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
        else
        {
            qDebug() << "Failed to parse JSON response";
        }
    }
    else
    {
        qDebug() << "Network request error: " << reply->errorString();
    }

    reply->deleteLater();

    if (authenticated)
    {
        qDebug() << "kaif";
        return true;
    }

    return false;
}


void Authorization::on_login_2_clicked()
{
    if(connect())
    {
        this->close();
        Homepage* form = new Homepage();
        form->show();
    }
}


void Authorization::on_buttonReg_clicked()
{
    this->close();
    Registration* form = new Registration();
    form->show();
}

