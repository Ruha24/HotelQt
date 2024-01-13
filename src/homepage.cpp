#include "includes/homepage.h"
#include "ui_homepage.h"


#include "includes/roomdetails.h"
#include "includes/filterform.h"


Homepage::Homepage(QWidget *parent, QString name) :
    QMainWindow(parent),
    ui(new Ui::Homepage)
{
    ui->setupUi(this);
    ui->full_menu_widget->hide();
    customDialog();
    connectionAll();

    UserName = name;
    getRole(UserName);

    getInformation(UserName);
    ui->loginEdit->setText(name);

    HideElement();
}

Homepage::~Homepage()
{
    delete ui;
}

void Homepage::HideElement()
{
    if(UserRole != "Админ")
    {
        QLayoutItem *child;
        while ((child = ui->Work->takeAt(0)) != nullptr) {
            if (QWidget *widget = child->widget()) {
                widget->hide();
            }
            delete child;
        }
    }
}

void Homepage::connectionAll()
{

    // Эти connect предназначены для отображения картинки при нажатие на кнопку
    connect(ui->pushButton_2, &QPushButton::toggled, this, &Homepage::onAnyButtonToggled);
    connect(ui->pushButton_3, &QPushButton::toggled, this, &Homepage::onAnyButtonToggled);
    connect(ui->pushButton_16, &QPushButton::toggled, this, &Homepage::onAnyButtonToggled);
    connect(ui->pushButton_11, &QPushButton::toggled, this, &Homepage::onAnyButtonToggled);
    connect(ui->room1btn, &QPushButton::toggled, this, &Homepage::onAnyButtonToggled);
    connect(ui->room2btn, &QPushButton::toggled, this, &Homepage::onAnyButtonToggled);
    connect(ui->pushButton_12, &QPushButton::toggled, this, &Homepage::onAnyButtonToggled);
    connect(ui->pushButton_13, &QPushButton::toggled, this, &Homepage::onAnyButtonToggled);
    connect(ui->pushButton_14, &QPushButton::toggled, this, &Homepage::onAnyButtonToggled);
    connect(ui->pushButton_15, &QPushButton::toggled, this, &Homepage::onAnyButtonToggled);
}

void Homepage::on_search_btn_clicked()
{
    QString search_text = ui->search_input->text();

    if (!search_text.isEmpty())
    {
        if (Type == "") {
            getRoomInformation(search_text);
        } else {
            getRoomInfo(search_text, Type, IsOccupied);
        }
    }
}



void Homepage::on_filterbtn_clicked()
{
    FilterForm* filter = new FilterForm(this);
     connect(filter, &FilterForm::filterAccepted, this, &Homepage::handleFilterAccepted);
    filter->show();
}

void Homepage::handleFilterAccepted(const QString &type, bool isOccupied)
{
    Type = type;
    IsOccupied = isOccupied;
}

void Homepage::getRoomInformation(const QString &searchText)
{
    QNetworkAccessManager networkManager;
    QJsonObject json;
    json["name"] = searchText;

    QJsonDocument jsonDoc(json);

    QNetworkRequest request(QUrl("http://localhost:555/searchRoom"));

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply* reply = networkManager.post(request, jsonDoc.toJson());

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray responseData = reply->readAll();
        QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);

        if (responseDoc.isObject())
        {
            QJsonObject response = responseDoc.object();

            if (response.contains("status"))
            {
                QString status = response["status"].toString();

                if (status == "success")
                {
                    if (response.contains("data"))
                    {
                        while (QLayoutItem* item = ui->verticalLayout_30->takeAt(0)) {
                            delete item->widget();
                            delete item;
                        }
                        roomsArray = response["data"].toArray();

                        roomdetails *roomdetail = new roomdetails(ui->stackedWidget);

                        for (const QJsonValue &roomValue : roomsArray)
                        {
                            QJsonObject roomData = roomValue.toObject();

                            roomdetail->setRoomData(
                                roomData["name"].toString(),
                                roomData["typeroom"].toString(),
                                roomData["startprice"].toDouble(),
                                roomData["occupied"].toBool()
                                );

                            ui->verticalLayout_30->addWidget(roomdetail);
                        }
                        ui->stackedWidget->setCurrentIndex(4);
                    }

                    else
                    {
                        qDebug() << "Error: not 'data' ";
                    }
                }
                else
                {
                    qDebug() << "Error: Unsuccessful response - " << status;
                }
            }
        }
    }
    else
    {
        qDebug() << "Error:" << reply->errorString();
    }

    reply->deleteLater();
}


void Homepage::on_user_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
}

void Homepage::getInformation(const QString &name)
{
    QNetworkAccessManager networkManager;
    QJsonObject json;
    json["name"] = name;

    QJsonDocument jsonDoc(json);

    QNetworkRequest request(QUrl("http://localhost:555/getInformationUser"));

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply* reply = networkManager.post(request, jsonDoc.toJson());

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray responseData = reply->readAll();
        QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);

        if(responseDoc.isObject())
        {
            QJsonObject response = responseDoc.object();

            if (response.contains("status"))
            {
                QString status = response["status"].toString();

                if (status == "success")
                {
                    if (response.contains("data"))
                    {
                        QJsonObject userData = response["data"].toObject();

                        ui->UserEdit->setText(userData["name"].toString());
                        ui->SurnameEdit->setText(userData["surname"].toString());
                    }
                    else
                    {
                        qDebug() << "Error: not 'data' ";
                    }
                }
                else
                {
                    qDebug() << "Error: Unsuccessful response - " << status;
                }
            }
        }
    } else {
        qDebug() << "Error:" << reply->errorString();
    }

    reply->deleteLater();
}

void Homepage::getRole(QString& name)
{
    QNetworkAccessManager networkManager;
    QJsonObject json;
    json["name"] = name;

    QJsonDocument jsonDoc(json);

    QNetworkRequest request(QUrl("http://localhost:555/get_role"));

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply* reply = networkManager.post(request, jsonDoc.toJson());

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray responseData = reply->readAll();
        QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);

        if (responseDoc.isObject()) {
            QJsonObject responseObject = responseDoc.object();
            if (responseObject.contains("role")) {
                QString role = responseObject.value("role").toString();
                UserRole = role;
                ui->Rolelbl->setText("Роль: " + role);
            } else {
                qDebug() << "Error: Response does not contain role information.";
            }
        } else {
            qDebug() << "Error: Invalid response format.";
            qDebug() << "Response Data:" << responseData;
        }
    } else {
        qDebug() << "Error:" << reply->errorString();
    }

    reply->deleteLater();
}

void Homepage::onAnyButtonToggled()
{
    QPushButton *senderButton = qobject_cast<QPushButton*>(sender());
    if (senderButton) {
        setImage(senderButton);
    }
}

void Homepage::getRoomInfo(const QString &text, const QString &type, bool isOccupied)
{
        QNetworkAccessManager networkManager;

        QJsonObject json;
        json["search_text"] = text;
        json["type"] = type;
        json["isOccupied"] = isOccupied;

        QJsonDocument jsonDoc(json);

        QNetworkRequest request(QUrl("http://localhost:555/search_room"));

        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QNetworkReply* reply = networkManager.post(request, jsonDoc.toJson());

        QEventLoop loop;

        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

        loop.exec();

        if(reply->error() == QNetworkReply::NoError)
        {
            QByteArray responseData = reply->readAll();
            QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);

            if(responseDoc.isObject())
            {
                QJsonObject response = responseDoc.object();

                if (response.contains("status"))
                {
                    QString status = response["status"].toString();

                    if (status == "success")
                    {
                        if (response.contains("data"))
                        {
                            roomsArray = response["data"].toArray();

                            while (QLayoutItem* item = ui->verticalLayout_30->takeAt(0)) {
                                delete item->widget();
                                delete item;
                            }

                            for (const QJsonValue &roomValue : roomsArray)
                            {
                                roomdetails *roomdetail = new roomdetails(ui->stackedWidget);

                                QJsonObject roomData = roomValue.toObject();

                                roomdetail->setRoomData(
                                    roomData["name"].toString(),
                                    roomData["typeroom"].toString(),
                                    roomData["startprice"].toDouble(),
                                    roomData["occupied"].toBool()
                                    );

                                ui->verticalLayout_30->addWidget(roomdetail);
                        }

                        ui->stackedWidget->setCurrentIndex(4);

                        }

                        else
                        {
                            qDebug() << "Error: not 'data' ";
                        }
                    }
                    else
                    {
                        qDebug() << "Error: Unsuccessful response - " << status;
                    }
                }
            }
        }
        else
        {
            qDebug() << "Error:" << reply->errorString();
        }

        reply->deleteLater();

}

void Homepage::on_importbtn_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open CSV File"), "", tr("CSV Files (*.csv);;All Files (*)"));

    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray fileData = file.readAll();
            file.close();

            QNetworkAccessManager* networkManager = new QNetworkAccessManager(this);

            QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

            QHttpPart filePart;
            filePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                               QVariant("form-data; name=\"file\"; filename=\"" + QFileInfo(filePath).fileName() + "\""));
            filePart.setBody(fileData);
            multiPart->append(filePart);

            QNetworkRequest request(QUrl("http://localhost:555/uploadData"));
            request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("multipart/form-data; boundary=" + multiPart->boundary()));
            QNetworkReply* reply = networkManager->post(request, multiPart);

            connect(reply, &QNetworkReply::finished, this, &Homepage::uploadFinished);

            multiPart->setParent(reply);
        } else {
            QMessageBox::critical(this, tr("Error"), tr("Unable to open file: %1").arg(filePath));
        }
    }
}


void Homepage::uploadFinished()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();

        } else {
            QMessageBox::critical(this, tr("Error"), tr("File upload failed: %1").arg(reply->errorString()));
        }

        reply->deleteLater();
    }
}




void Homepage::on_exportbtn_clicked()
{
    exportToPDF(roomsArray);
}

void Homepage::exportToPDF(const QJsonArray &roomsArray)
{
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Сохранить PDF", "", "PDF Files (*.pdf)");

    if (fileName.isEmpty())
    {
        return;
    }

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPageSize::A4);

    printer.setOutputFileName(fileName);

    QPainter painter(&printer);

    QFont font;
    font.setPixelSize(12);
    painter.setFont(font);

    QStringList headers = {"Название", "Тип", "Начальная цена", "Занято"};

    const int columnCount = headers.size();
    const int cellPadding = 10;
    const int columnWidth = 125;

    int totalTableWidth = columnWidth * columnCount;

    int dataCellHeight = cellPadding * 4;
    int dataCellWidth = totalTableWidth / columnCount;

    for (int i = 0; i < roomsArray.size() + 1; ++i)
    {
        QStringList rowData;

        if (i == 0)
        {
            rowData = headers;
        }
        else
        {
            QJsonObject roomData = roomsArray[i - 1].toObject();
            rowData << roomData["name"].toString()
                    << roomData["typeroom"].toString()
                    << QString::number(roomData["startprice"].toDouble())
                    << (roomData["occupied"].toBool() ? "Да" : "Нет");
        }

        for (int j = 0; j < columnCount; ++j)
        {
            painter.drawRect(j * dataCellWidth, i * dataCellHeight, dataCellWidth, dataCellHeight);
            painter.drawText(j * dataCellWidth, i * dataCellHeight, dataCellWidth, dataCellHeight, Qt::AlignCenter, rowData[j]);
        }
    }

    QDateTime dateTime = QDateTime::currentDateTime();
    QString currentDate = dateTime.toString("dd.MM.yyyy hh:mm:ss");

    int dateMargin = 10;
    QRect dateRect(dateMargin, printer.pageLayout().paintRect().height() - dateMargin - font.pixelSize(), printer.pageLayout().paintRect().width() - 2 * dateMargin, font.pixelSize());
    painter.drawText(dateRect, Qt::AlignCenter, QDateTime::currentDateTime().toString("dd.MM.yyyy"));

}




void Homepage::setImage(QPushButton *button)
{
    chcImage = new CheckImage(this);

    QRegularExpression regex("border-image:\\s*url\\(([^)]+)\\)");
    QRegularExpressionMatch match = regex.match(button->styleSheet());

    if (match.hasMatch()) {
        QString imagePath = match.captured(1);

        chcImage->setImage(imagePath);
        chcImage->show();
    } else {
        qDebug() << "Ошибка, ничего нету в stylesheet";
    }
}


void Homepage::customDialog()
{
    menuDialog.setWindowFlag(Qt::Popup);

    startDateCalendar = new QCalendarWidget();
    endDateCalendar = new QCalendarWidget();
    applyButton = new QPushButton("Применить");

    QDateTime date = QDateTime::currentDateTime();
    QDate currentDate = QDate::currentDate();

    QString date_format = "MMMM мм";

    startDate = new QLabel(date.toString("ddd") + ", " + date.toString("dd MMM"));
    endDate = new QLabel(date.toString("ddd") + ", " + date.toString("dd MMM"));

    startDateCalendar->setMinimumDate(currentDate);
    endDateCalendar->setSelectedDate(currentDate.addMonths(1));

    HlayoutDate.addWidget(startDate);
    HlayoutDate.addWidget(endDate);

    HlayoutCalendar.addWidget(startDateCalendar);
    HlayoutCalendar.addWidget(endDateCalendar);

    Vlayout.addLayout(&HlayoutDate);
    Vlayout.addLayout(&HlayoutCalendar);
    Vlayout.addWidget(applyButton);

    connect(applyButton, &QPushButton::clicked, this, &Homepage::onApplyButtonClicked);
    connect(startDateCalendar, &QCalendarWidget::clicked, this, &Homepage::onDateSelected);
    connect(endDateCalendar, &QCalendarWidget::clicked, this, &Homepage::onDateSelected);
    menuDialog.setLayout(&Vlayout);
}

void Homepage::on_endDateBtn_clicked()
{
    menuDialog.exec();
}


void Homepage::on_startDateBtn_clicked()
{
    menuDialog.exec();
}

void Homepage::onDateSelected(const QDate& date)
{
    if(isStartDateSelected)
    {
        startDate->setText(date.toString("ddd") + ", " + date.toString("dd MMM"));
        isStartDateSelected = false;

    }
    else
    {
        endDate->setText(date.toString("ddd") + ", " + date.toString("dd MMM"));
        isStartDateSelected = true;
    }
}

void Homepage::on_home_btn_1_toggled(bool checked)
{
    ui->stackedWidget->setCurrentIndex(0);
}

void Homepage::on_shop_btn_1_toggled(bool checked)
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Homepage::on_stat_btn_1_toggled(bool checked)
{
    ui->stackedWidget->setCurrentIndex(3);
}

void Homepage::on_setting_btn_1_toggled(bool checked)
{
    ui->stackedWidget->setCurrentIndex(4);
}


void Homepage::onApplyButtonClicked()
{
    QStringList startDateParts = startDate->text().split(" ");
    QStringList endDateParts = endDate->text().split(" ");

    QString startDateText = startDateParts[1] + " " + startDateParts[2];
    QString endDateText = endDateParts[1] + " " + endDateParts[2];

    ui->startDateBtn->setText("Заезд\n" + startDateText);

    ui->endDateBtn->setText("Выезд\n" + endDateText);

    menuDialog.accept();
}

void Homepage::on_searchBtn_clicked()
{

}

void Homepage::on_pushButton_4_clicked()
{

}

void Homepage::links(const QString &link)
{
    numPage = link.at(link.length() - 1).digitValue();
    setPages(numPage);
}

void Homepage::setPages(int num)
{
    switch(num)
    {
        case 1:
            ui->Title->setText("Двухместный стандарт с одной кроватью");
            ui->SubTitle1->setText("№ 1. Двухместный стандарт с одной кроватью");
            ui->SubTitle2->setText("№ 2. Двухместный стандарт с одной кроватью");
            ui->room1btn->setStyleSheet("QPushButton {border-image: url(:/image/image/oneplace1.jpg)  0 0 0 0 stretch;}"
                                        "QPushButton:hover {border: 5px solid red;}");
            ui->room2btn->setStyleSheet("QPushButton {border-image: url(:/image/image/oneplace2.jpg) 0 0 0 0 stretch;}"
                                        "QPushButton:hover {border: 5px solid red;}");
            ui->InfoLabel1_1->setText("одна двуспальная кровать;");
            ui->InfoLabel2_1->setText("1 стол, 2 стула;");
            ui->InfoLabel3_1->setText("шкаф-купе;");
            ui->InfoLabel4_1->setText("холодильник, набор посуды.");
            ui->InfoLabel1_2->setText("одна двуспальная кровать;");
            ui->InfoLabel2_2->setText("2 тумбы, 1 стол, 2 стула;");
            ui->InfoLabel3_2->setText("холодильник, набор посуды;");
            ui->InfoLabel4_2->setText("ванная комната.");
            ui->stackedWidget->setCurrentIndex(2);
            break;
        case 2:
            ui->Title->setText("Двухместный стандарт с двумя кроватями");
            ui->SubTitle1->setText("№ 1. Двухместный стандарт с двумя кроватями");
            ui->SubTitle2->setText("№ 2. Двухместный стандарт с двумя кроватями");
            ui->room1btn->setStyleSheet("QPushButton {border-image: url(:/image/image/twoplace1.jpg)  0 0 0 0 stretch;}"
                                        "QPushButton:hover {border: 5px solid red;}");
            ui->room2btn->setStyleSheet("QPushButton {border-image: url(:/image/image/twoplace2.jpg) 0 0 0 0 stretch;}"
                                        "QPushButton:hover {border: 5px solid red;}");
            ui->InfoLabel1_1->setText("2 односпальные кровати;");
            ui->InfoLabel2_1->setText("1 стол, 2 стула;");
            ui->InfoLabel3_1->setText("кондиционер, холодильник;");
            ui->InfoLabel4_1->setText("ванная комната.");
            ui->InfoLabel1_2->setText("2 односпальные кровати;");
            ui->InfoLabel2_2->setText("1 стол, 2 стула;");
            ui->InfoLabel3_2->setText("холодильник, набор посуды;");
            ui->InfoLabel4_2->setText("фен.");
            ui->stackedWidget->setCurrentIndex(2);
            break;
        case 3:
            ui->Title->setText("Двухместный улучшенный");
            ui->SubTitle1->setText("№ 1. Двухместный улучшенный с одной кроватью");
            ui->SubTitle2->setText("№ 2. Двухместный улучшенный с одной кроватью");
            ui->room1btn->setStyleSheet("QPushButton {border-image: url(:/image/image/UppPlace.jpg)  0 0 0 0 stretch;}"
                                        "QPushButton:hover {border: 5px solid red;}");
            ui->room2btn->setStyleSheet("QPushButton {border-image: url(:/image/image/UppPlace2.jpg) 0 0 0 0 stretch;}"
                                        "QPushButton:hover {border: 5px solid red;}");
            ui->InfoLabel1_1->setText("одна двуспальная кровать;");
            ui->InfoLabel2_1->setText("2 тумбы, 1 стол, 2 стула;");
            ui->InfoLabel3_1->setText("ванная комната;");
            ui->InfoLabel4_1->setText("набор посуды.");
            ui->InfoLabel1_2->setText("одна двуспальная кровать;");
            ui->InfoLabel2_2->setText("тумба, 1 стол, 2 стула;");
            ui->InfoLabel3_2->setText("ванная комната;");
            ui->InfoLabel4_2->setText("набор посуды.");
            ui->stackedWidget->setCurrentIndex(2);
            break;
        case 4:
            ui->Title->setText("Двухместный полулюкс");
            ui->SubTitle1->setText("№ 1. Двухместный полулюкс с одной кроватью");
            ui->SubTitle2->setText("№ 2. Двухместный полулюкс с одной кроватью");
            ui->room1btn->setStyleSheet("QPushButton {border-image: url(:/image/image/LuxPlace.jpg)  0 0 0 0 stretch;}"
                                        "QPushButton:hover {border: 5px solid red;}");
            ui->room2btn->setStyleSheet("QPushButton {border-image: url(:/image/image/LuxPlace2.jpg) 0 0 0 0 stretch;}"
                                        "QPushButton:hover {border: 5px solid red;}");
            ui->InfoLabel1_1->setText("одна двуспальная кровать;");
            ui->InfoLabel2_1->setText("2 тумбы, 1 стол, 2 стула;");
            ui->InfoLabel3_1->setText("холодильник;");
            ui->InfoLabel4_1->setText("ванная комната.");
            ui->InfoLabel1_2->setText("одна двуспальная кровать;");
            ui->InfoLabel2_2->setText("2 тумбы, 1 стол, 2 стула;");
            ui->InfoLabel3_2->setText("ванная комната;");
            ui->InfoLabel4_2->setText("утюг.");
            ui->stackedWidget->setCurrentIndex(2);
            break;
        }
}

void Homepage::setInfoRoom()
{
        switch(numPage)
        {
        case 1:
            ui->pushButton_11->setStyleSheet("QPushButton {border-image: url(:/image/image/2place1/image2.jpg)  0 0 0 0 stretch;}"
                                             "QPushButton:hover {border: 5px solid red");
            ui->pushButton_12->setStyleSheet("QPushButton {border-image: url(:/image/image/2place1/image2.jpg)  0 0 0 0 stretch;}"
                                             "QPushButton:hover {border: 5px solid red");
            ui->pushButton_13->setStyleSheet("QPushButton {border-image: url(:/image/image/2place1/image2.jpg)  0 0 0 0 stretch;}"
                                             "QPushButton:hover {border: 5px solid red");
            ui->pushButton_14->setStyleSheet("QPushButton {border-image: url(:/image/image/2place1/image2.jpg)  0 0 0 0 stretch;}"
                                             "QPushButton:hover {border: 5px solid red");
            ui->pushButton_15->setStyleSheet("QPushButton {border-image: url(:/image/image/2place1/image2.jpg)  0 0 0 0 stretch;}"
                                             "QPushButton:hover {border: 5px solid red");
            ui->pushButton_16->setStyleSheet("QPushButton {border-image: url(:/image/image/2place1/image2.jpg)  0 0 0 0 stretch;}"
                                             "QPushButton:hover {border: 5px solid red");
            ui->NameRoom->setText("Двухместный стандарт с двумя кроватями");
            ui->stackedWidget->setCurrentIndex(3);
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        }
}



void Homepage::on_room1_1btn_clicked()
{
        setInfoRoom();
}



void Homepage::on_label_14_linkActivated(const QString &link)
{
    links(link);
}

void Homepage::on_label_35_linkActivated(const QString &link)
{
    links(link);
}

void Homepage::on_label_37_linkActivated(const QString &link)
{
    links(link);
}


void Homepage::on_label_38_linkActivated(const QString &link)
{
    links(link);
}


void Homepage::on_label_36_linkActivated(const QString &link)
{
    links(link);
}


void Homepage::on_label_10_linkActivated(const QString &link)
{
    links(link);
}

void Homepage::on_label_4_linkActivated(const QString &link)
{
    links(link);
}

void Homepage::on_label_12_linkActivated(const QString &link)
{
    links(link);
}

void Homepage::on_label_11_linkActivated(const QString &link)
{
    links(link);
}

void Homepage::on_Hoteln1_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void Homepage::on_pushButton_5_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
