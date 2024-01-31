#include "includes/filterform.h"
#include "ui_filterform.h"

FilterForm::FilterForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FilterForm)
{
    ui->setupUi(this);

    getTypeRoom();
}


FilterForm::~FilterForm()
{
    delete ui;
}

void FilterForm::on_acceptbtn_clicked()
{
    QString type = ui->comboBox->currentText();
    bool isoccupt = ui->checkBox->isChecked();

    emit filterAccepted(type, isoccupt);

    close();
}

void FilterForm::getTypeRoom()
{
    QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl("http://localhost:555/getType"));
    QNetworkReply *reply = networkManager->get(request);

    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);

            if (!jsonResponse.isNull() && jsonResponse.isObject()) {
                QJsonObject jsonObject = jsonResponse.object();

                if (jsonObject.contains("room_types")) {
                    QJsonArray roomTypesArray = jsonObject["room_types"].toArray();
                    QStringList roomTypes;

                    for (const QJsonValue &roomTypeValue : roomTypesArray) {
                        roomTypes << roomTypeValue.toString();
                    }

                    ui->comboBox->addItems(roomTypes);
                }
            }
        } else {
            qDebug() << "Network request error: " << reply->errorString();
        }

        reply->deleteLater();
    });
}
