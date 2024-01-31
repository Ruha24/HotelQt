#include "includes/homepage.h"
#include "ui_homepage.h"

Homepage::Homepage(QWidget *parent, QString name)
    : QMainWindow(parent)
    , ui(new Ui::Homepage)
    , networkManager()
    , commonRequest(QUrl("http://localhost:555/"))
    , commonJsonObject()
{
    ui->setupUi(this);
    ui->full_menu_widget->hide();

    customDialog();
    connectionAll();

    getRole(name);
    getInformation(name);
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
                    if (response.contains("data")) {
                        roomsArray = response["data"].toArray();

                        roomdetails *roomdetail = new roomdetails(ui->stackedWidget);

                        for (const QJsonValue &roomValue : roomsArray)
                        {
                            QJsonObject roomData = roomValue.toObject();

                            roomdetail->setRoomData(roomData["name"].toString(),
                                                    QStringList()
                                                        << roomData["typeroom"].toString()
                                                        << QString::number(
                                                               roomData["startprice"].toDouble())
                                                        << (roomData["occupied"].toBool()
                                                                ? "Занято"
                                                                : "Не занято"));
                        }
                        ui->stackedWidget->setCurrentIndex(4);
                    }

                    else {
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

void Homepage::handleServerResponse(QNetworkReply *reply,
                                    const QString &successStatus,
                                    const std::function<void(QJsonObject)> &successCallback)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);

        if (responseDoc.isObject()) {
            QJsonObject response = responseDoc.object();

            if (response.contains("status")) {
                QString status = response["status"].toString();

                if (status == successStatus) {
                    if (response.contains("data")) {
                        successCallback(response["data"].toObject());
                    } else {
                        qDebug() << "Error: not 'data' ";
                    }
                } else {
                    qDebug() << "Error: Unsuccessful response - " << status;
                }
            }
        }
    } else {
        qDebug() << "Error:" << reply->errorString();
    }

    reply->deleteLater();
}

void Homepage::getInformation(const QString &name)
{
    commonJsonObject["name"] = name;

    QNetworkReply *reply = networkManager.post(commonRequest,
                                               QJsonDocument(commonJsonObject).toJson());

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    handleServerResponse(reply, "success", [this](QJsonObject data) {
        QJsonObject userData = data["data"].toObject();

        ui->UserEdit->setText(userData["name"].toString());
        ui->SurnameEdit->setText(userData["surname"].toString());
    });
}

void Homepage::getRole(QString& name)
{
    QNetworkAccessManager networkManager;
    QJsonObject json;
    json["name"] = name;

    QJsonDocument jsonDoc(json);

    QNetworkRequest request(QUrl("http://localhost:555/get_role"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = networkManager.post(request, jsonDoc.toJson());

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
    commonJsonObject["name"] = text;

    QNetworkReply *reply = networkManager.post(commonRequest,
                                               QJsonDocument(commonJsonObject).toJson());

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    handleServerResponse(reply, "success", [this](QJsonObject data) {
        roomsArray = data["data"].toArray();

        roomdetails *roomdetail = new roomdetails(ui->stackedWidget);

        for (const QJsonValue &roomValue : roomsArray) {
            QJsonObject roomData = roomValue.toObject();

            roomdetail->setRoomData(roomData["name"].toString(),
                                    QStringList()
                                        << roomData["typeroom"].toString()
                                        << QString::number(roomData["startprice"].toDouble())
                                        << (roomData["occupied"].toBool() ? "Занято" : "Не занято"));
        }

        ui->stackedWidget->setCurrentIndex(4);
    });
}

void Homepage::setImage(QPushButton *button)
{
    chcImage = new CheckImage(this);

    QString styleSheet = button->styleSheet();
    QRegularExpression regex("border-image:\\s*url\\(([^)]+)\\)");
    QRegularExpressionMatch match = regex.match(styleSheet);

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

    startDate = createLabelWithDate();
    endDate = createLabelWithDate();
    startDateCalendar->setMinimumDate(QDate::currentDate());
    endDateCalendar->setSelectedDate(QDate::currentDate().addMonths(1));

    HlayoutDate.addWidget(startDate);
    HlayoutDate.addWidget(endDate);

    HlayoutCalendar.addWidget(startDateCalendar);
    HlayoutCalendar.addWidget(endDateCalendar);

    Vlayout.addLayout(&HlayoutDate);
    Vlayout.addLayout(&HlayoutCalendar);
    Vlayout.addWidget(applyButton);

    connect(applyButton, &QPushButton::clicked, this, &Homepage::updateDateButtons);
    connect(startDateCalendar, &QCalendarWidget::clicked, this, &Homepage::onDateSelected);
    connect(endDateCalendar, &QCalendarWidget::clicked, this, &Homepage::onDateSelected);

    menuDialog.setLayout(&Vlayout);
}

QLabel *Homepage::createLabelWithDate()
{
    QLabel *label = new QLabel(getFormattedDate(QDate::currentDate()));
    return label;
}

QString Homepage::getFormattedDate(const QDate &date)
{
    return date.toString("ddd") + ", " + date.toString("dd MMM");
}

void Homepage::on_endDateBtn_clicked()
{
    menuDialog.exec();
}

void Homepage::on_startDateBtn_clicked()
{
    menuDialog.exec();
}

void Homepage::onDateSelected(const QDate &date)
{
    QLabel *selectedLabel = isStartDateSelected ? startDate : endDate;
    selectedLabel->setText(getFormattedDate(date));
    isStartDateSelected = !isStartDateSelected;
}

void Homepage::setPageIndex(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
}

void Homepage::on_home_btn_1_toggled(bool checked)
{
    setPageIndex(0);
}

void Homepage::on_shop_btn_1_toggled(bool checked)
{
    setPageIndex(1);
}

void Homepage::on_stat_btn_1_toggled(bool checked)
{
    setPageIndex(3);
}

void Homepage::on_setting_btn_1_toggled(bool checked)
{
    setPageIndex(4);
}

void Homepage::onApplyButtonClicked()
{
    updateDateButtons();
    menuDialog.accept();
}

void Homepage::updateDateButtons()
{
    QStringList startDateParts = startDate->text().split(" ");
    QStringList endDateParts = endDate->text().split(" ");

    QString startDateText = startDateParts[1] + " " + startDateParts[2];
    QString endDateText = endDateParts[1] + " " + endDateParts[2];

    ui->startDateBtn->setText("Заезд\n" + startDateText);
    ui->endDateBtn->setText("Выезд\n" + endDateText);
}

void Homepage::on_searchBtn_clicked()
{
    // TODO: Implement search functionality
}

void Homepage::on_pushButton_4_clicked()
{
    // TODO: Implement button click functionality
}

void Homepage::links(const QString &link)
{
    numPage = link.at(link.length() - 1).digitValue();
    setPages(numPage);
}

void Homepage::setPages(int num)
{
    enum RoomNumber { Room1 = 1, Room2, Room3, Room4 };

    roomdetails *roomDetail = new roomdetails(this);

    switch (num) {
    case Room1:
        setRoomDataForPage(roomDetail, "oneplace1", "Двухместный стандарт с одной кроватью");
        break;

    case Room2:
        setRoomDataForPage(roomDetail, "twoplace1", "Двухместный стандарт с двумя кроватями");
        break;

    case Room3:
        setRoomDataForPage(roomDetail, "UppPlace", "Двухместный улучшенный");
        break;

    case Room4:
        setRoomDataForPage(roomDetail, "LuxPlace", "Двухместный полулюкс");
        break;
    }

    ui->verticalLayout_8->addWidget(roomDetail);
    ui->stackedWidget->setCurrentIndex(2);
}

void Homepage::setRoomDataForPage(roomdetails *roomDetail,
                                  const QString &image,
                                  const QString &title)
{
    QStringList roomInfoList = {"одна двуспальная кровать;",
                                "1 стол, 2 стула;",
                                "шкаф-купе;",
                                "холодильник, набор посуды"};
    roomDetail->setRoomData(title, roomInfoList);

    ui->room1btn->setStyleSheet(createButtonStyleSheet(image + "/" + image + ".jpg"));
    ui->room2btn->setStyleSheet(createButtonStyleSheet(image + "/" + image + ".jpg"));
}

QString Homepage::createButtonStyleSheet(const QString &imagePath)
{
    return "QPushButton {border-image: url(" + prefix + imagePath
           + ")  0 0 0 0 stretch;}"
             "QPushButton:hover {border: 5px solid red;}";
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
