#include "homepage.h"
#include "./ui_homepage.h"

#include "CustomWidget/customcalendar.h"

HomePage::HomePage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HomePage)
{
    userData = new UserData();

    ui->setupUi(this);

    CreateElementMenu();
    connectionElement();

    ui->stackedWidget->setCurrentIndex(0);

    initActionUser(ui->actionUser);
    initActionUser(ui->actionUser_2);
    initActionUser(ui->actionUser_3);
    initActionUser(ui->actionUser_4);
    initActionUser(ui->actionUser_6);

    SetVisibleUser();
}

HomePage::~HomePage()
{
    delete ui;
}

void HomePage::setUserData(UserData *data)
{
    userData = data;

    if (userData->getUserName() != "") {
        userData->getUserStats(userData->getUserName());
        SetVisibleUser();
    }
}

void HomePage::on_startTimebtn_clicked()
{
    QRect buttonRect = ui->startTimebtn->geometry();
    QPoint buttonCenter = buttonRect.center();
    QPoint buttonPos = ui->startTimebtn->mapTo(this, QPoint(0, 0));

    customCalendar = new CustomCalendar(this,
                                        buttonCenter.x() + ui->startTimebtn->width(),
                                        buttonPos.y() + ui->startTimebtn->height());

    ui->startTimebtn->setText(customCalendar->getStartDate());
    ui->endTimeBtn->setText(customCalendar->getEndDate());
}

void HomePage::on_endTimeBtn_clicked()
{
    QRect buttonRect = ui->endTimeBtn->geometry();
    QPoint buttonCenter = buttonRect.center();
    QPoint buttonPos = ui->endTimeBtn->mapTo(this, QPoint(0, 0));

    customCalendar = new CustomCalendar(this,
                                        buttonCenter.x() + ui->endTimeBtn->width(),
                                        buttonPos.y() + ui->endTimeBtn->height());

    ui->startTimebtn->setText(customCalendar->getStartDate());
    ui->endTimeBtn->setText(customCalendar->getEndDate());
}

void HomePage::connectionElement() {}

void HomePage::CreateElementMenu()
{
    QMenu *userInputMenu = new QMenu(ui->questtlb);
    QWidget *widget = new QWidget(ui->questtlb);
    widget->setStyleSheet("QLabel {color: black;}");
    QVBoxLayout *mainLayout = new QVBoxLayout(widget);

    // Счётчик взрослых
    QLabel *titlelbl = new QLabel("Взрослые");
    titlelbl->setMinimumWidth(130);
    titlelbl->setMaximumWidth(130);

    QSpacerItem *spacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Expanding);
    QSpacerItem *spacer2 = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Expanding);
    QHBoxLayout *layout = new QHBoxLayout(widget);
    minusButton = new QPushButton("-");
    minusButton->setStyleSheet(
        "QPushButton { font-size: 18px; background-color: rgb(194, 193, 184); border-radius: "
        "20px; min-height: 40px; max-height: "
        "40px; min-width: 40px; max-width: 40px; }");
    addButton = new QPushButton("+");
    addButton->setStyleSheet(
        "QPushButton { font-size: 18px; background-color: rgb(194, 193, 184);; border-radius: "
        "20px; min-height: 40px; max-height: "
        "40px; min-width: 40px; max-width: 40px; }");
    countlbl = new QLabel("1");
    layout->addWidget(titlelbl);
    layout->addSpacerItem(spacer);
    layout->addWidget(minusButton);
    layout->addSpacerItem(spacer2);
    layout->addWidget(countlbl);
    layout->addSpacerItem(spacer2);
    layout->addWidget(addButton);
    layout->addSpacerItem(spacer);

    // Счётчик детей
    QLabel *titlelbl2 = new QLabel("Дети");
    titlelbl2->setMinimumWidth(130);
    titlelbl2->setMaximumWidth(130);

    QHBoxLayout *layout2 = new QHBoxLayout(widget);
    minusButton2 = new QPushButton("-");
    minusButton2->setStyleSheet(
        "QPushButton { font-size: 18px; background-color: rgb(194, 193, 184); border-radius: "
        "20px; min-height: 40px; max-height: "
        "40px; min-width: 40px; max-width: 40px; }");

    addButton2 = new QPushButton("+");
    addButton2->setStyleSheet(
        "QPushButton { font-size: 18px; background-color: rgb(194, 193, 184);; border-radius: "
        "20px; min-height: 40px; max-height: "
        "40px; min-width: 40px; max-width: 40px; }");

    countChildlbl = new QLabel("0");
    layout2->addWidget(titlelbl2);
    layout2->addSpacerItem(spacer);
    layout2->addWidget(minusButton2);
    layout2->addSpacerItem(spacer2);
    layout2->addWidget(countChildlbl);
    layout2->addSpacerItem(spacer2);
    layout2->addWidget(addButton2);
    layout2->addSpacerItem(spacer);

    mainLayout->addLayout(layout);
    mainLayout->addLayout(layout2);

    connect(ui->questtlb, &QToolButton::clicked, ui->questtlb, &QToolButton::showMenu);
    connect(minusButton, &QAbstractButton::clicked, this, &HomePage::changedMinusGrown);
    connect(addButton, &QAbstractButton::clicked, this, &HomePage::changedAddGrown);
    connect(minusButton2, &QAbstractButton::clicked, this, &HomePage::changedMinusChild);
    connect(addButton2, &QAbstractButton::clicked, this, &HomePage::changedAddChild);

    QWidgetAction *widgetAction = new QWidgetAction(ui->questtlb);
    widgetAction->setDefaultWidget(widget);
    userInputMenu->addAction(widgetAction);

    ui->questtlb->setMenu(userInputMenu);
}

void HomePage::SetVisibleUser()
{
    static bool isVisible = false;

    ui->Profile->setVisible(!isVisible);
    ui->Profile_2->setVisible(!isVisible);
    ui->Profile_3->setVisible(!isVisible);
    ui->Profile_5->setVisible(!isVisible);
    ui->Profile_6->setVisible(!isVisible);

    ui->actionUser->setVisible(isVisible);
    ui->actionUser_2->setVisible(isVisible);
    ui->actionUser_3->setVisible(isVisible);
    ui->actionUser_4->setVisible(isVisible);
    ui->actionUser_6->setVisible(isVisible);
    isVisible = !isVisible;
}

void HomePage::initActionUser(QComboBox *cmb)
{
    cmb->addItem("Профиль");
    cmb->addItem("Бронирования");
    cmb->addItem("Выход");

    connect(cmb, QOverload<int>::of(&QComboBox::activated), this, [=](int index) {
        switch (index) {
        case 0:
            ui->stackedWidget->setCurrentIndex(4);
            break;
        case 1:
            setRecovery();
            break;
        case 2:
            SetVisibleUser();
            ui->stackedWidget->setCurrentIndex(0);
            break;
        }
    });
}

void clearLayout(QLayout *layout)
{
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        if (auto widget = child->widget()) {
            delete widget;
        }
        delete child;
    }
}

void HomePage::setRecovery()
{
    clearLayout(ui->verticalLayout_14);

    userData->getUserRecovery([&](bool success) {
        if (success) {
            recList = userData->getListRecovery();
            for (const auto &recovery : recList) {
                QWidget *roomWidget = new QWidget();
                roomWidget->setFixedSize(900, 100);
                roomWidget->setStyleSheet(
                    "QWidget { background-color: #DCDCDC; border-radius: 10px}");

                QHBoxLayout *layout = new QHBoxLayout(roomWidget);
                layout->setAlignment(Qt::AlignCenter);

                QLabel *roomLabel = new QLabel(recovery.getRoomName());
                roomLabel->setStyleSheet("QLabel { font-size: 24px}");

                QLocale russianLocale(QLocale::Russian);

                QString startDate = russianLocale.toString(recovery.getStartDate(), "d MMMM yyyy");
                QString lastDate = russianLocale.toString(recovery.getLastDate(), "d MMMM yyyy");

                QLabel *dateLabel = new QLabel(QString("%1 - %2").arg(startDate).arg(lastDate));

                dateLabel->setStyleSheet("QLabel { font-size: 24px}");
                QPushButton *infoButton = new QPushButton("Подробнее");
                infoButton->setStyleSheet("QPushButton { border-radius: 10px;  color: white; "
                                          "background-color: #8C8C8C; font-size: 24px}");
                infoButton->setFixedSize(135, 55);

                QPushButton *cancelButton = new QPushButton("Отменить");
                cancelButton->setStyleSheet(""
                                            "QPushButton { border-radius: 10px;  color: white; "
                                            "background-color: #B60000; font-size: 24px}");
                cancelButton->setFixedSize(135, 55);

                layout->addWidget(roomLabel);
                layout->addWidget(dateLabel);
                layout->addWidget(infoButton);
                layout->addWidget(cancelButton);

                ui->verticalLayout_14->addWidget(roomWidget);
                connect(cancelButton, &QPushButton::clicked, this, [&]() {
                    cancelRecovery(recovery);
                });

                connect(infoButton, &QPushButton::clicked, this, [&]() {
                    showDetailedInfo(recovery);
                });
            }

            if (recList.isEmpty()) {
                ui->label_15->setVisible(true);
            } else {
                ui->label_15->setVisible(false);
            }

            ui->stackedWidget->setCurrentIndex(1);
        }
    });
}

void HomePage::showDetailedInfo(const RecoveryData &recovery)
{
    RecoveryData *rec = new RecoveryData(recovery.getRoomName(),
                                         recovery.getDescription(),
                                         recovery.getStartDate(),
                                         recovery.getLastDate());

    InfoReserv *infoReserv = new InfoReserv(this, rec);

    infoReserv->show();
}

void HomePage::cancelRecovery(const RecoveryData &recovery) {}

void HomePage::changedMinusChild()
{
    int count = countChildlbl->text().toInt();
    if (count > 0) {
        countChildlbl->setText(QString::number(count - 1));
        setTextGuest();
    }
}

void HomePage::changedAddChild()
{
    int count = countChildlbl->text().toInt();

    if (count < 4) {
        countChildlbl->setText(QString::number(count + 1));
        setTextGuest();
    }
}

void HomePage::changedMinusGrown()
{
    int count = countlbl->text().toInt();
    if (count > 1) {
        countlbl->setText(QString::number(count - 1));
        setTextGuest();
    }
}

void HomePage::changedAddGrown()
{
    int count = countlbl->text().toInt();
    if (count < 4) {
        countlbl->setText(QString::number(count + 1));
        setTextGuest();
    }
}

void HomePage::setTextGuest()
{
    int countGrown = countlbl->text().toInt();
    int countChild = countChildlbl->text().toInt();

    QString text;
    switch (countGrown) {
    case 1:
        text += "1 взрослый";
        break;
    default:
        text += QString::number(countGrown) + " взрослых";
        break;
    }

    switch (countChild) {
    case 0:
        break;
    case 1:
        text += ", 1 ребёнок";
        break;
    default:
        text += ", " + QString::number(countChild) + " ребёнка";
        break;
    }

    ui->questtlb->setText(text);
}

void HomePage::on_searchbtn_clicked()
{
    if (ui->startTimebtn->text().contains("Заезд") || ui->endTimeBtn->text().contains("Выезд")) {
        QMessageBox::critical(this, "Ошибка", "Заполните все поля");
        return;
    }

    ui->stackedWidget->setCurrentIndex(4);
}

void HomePage::on_bronlbl_linkActivated(const QString &link)
{
    ui->stackedWidget->setCurrentIndex(3);
}

void HomePage::on_placelbl_linkActivated(const QString &link)
{
    ui->stackedWidget->setCurrentIndex(2);
}

void HomePage::on_aboutlbl_linkActivated(const QString &link)
{
    ui->stackedWidget->setCurrentIndex(1);
}

void HomePage::on_Title_linkActivated(const QString &link)
{
    ui->stackedWidget->setCurrentIndex(0);
}

void HomePage::on_Title_2_linkActivated(const QString &link)
{
    ui->stackedWidget->setCurrentIndex(0);
}

void HomePage::on_Title_3_linkActivated(const QString &link)
{
    ui->stackedWidget->setCurrentIndex(0);
}

void HomePage::on_aboutlbl_2_linkActivated(const QString &link)
{
    ui->stackedWidget->setCurrentIndex(1);
}

void HomePage::on_placelbl_3_linkActivated(const QString &link)
{
    ui->stackedWidget->setCurrentIndex(2);
}

void HomePage::on_bronlbl_3_linkActivated(const QString &link)
{
    ui->stackedWidget->setCurrentIndex(0);
}

void HomePage::on_bronlbl_2_linkActivated(const QString &link)
{
    ui->stackedWidget->setCurrentIndex(0);
}

void HomePage::on_Profile_2_clicked()
{
    authorizationUser();
}

void HomePage::on_Profile_clicked()
{
    authorizationUser();
}

void HomePage::authorizationUser()
{
    Authorization *authWindow = new Authorization();
    connect(authWindow, &Authorization::userAuthenticated, this, &HomePage::setUserData);
    authWindow->show();
}

void HomePage::on_Title_5_linkActivated(const QString &link)
{
    ui->stackedWidget->setCurrentIndex(0);
}

void HomePage::on_bronlbl_5_linkActivated(const QString &link)
{
    ui->stackedWidget->setCurrentIndex(0);
}

void HomePage::on_placelbl_5_linkActivated(const QString &link)
{
    ui->stackedWidget->setCurrentIndex(2);
}

void HomePage::on_aboutlbl_5_linkActivated(const QString &link)
{
    ui->stackedWidget->setCurrentIndex(1);
}

void HomePage::on_Profile_3_clicked()
{
    authorizationUser();
}

void HomePage::on_saveDataUser_clicked()
{
    if (ui->userNametxt->text() == "" || ui->lastnametxt->text() == ""
        || ui->birthdaytxt->text() == "" || ui->emailtxt->text() == "")
        return;

    userData->setEmail(ui->emailtxt->text());

    userData->updateStats(ui->userNametxt->text(),
                          ui->lastnametxt->text(),
                          ui->birthdaytxt->text(),
                          ui->emailtxt->text(),
                          [&](bool success) {
                              if (success)
                                  QMessageBox::information(this, "", "Данные успешно обновлены");
                              else
                                  QMessageBox::information(this,
                                                           "Ошибка",
                                                           "Данные не были обновлены");
                          });
}

void HomePage::on_savePasswordbtn_clicked()
{
    if (ui->currentPasswordtxt->text() == "" || ui->newPasswordtxt->text() == "")
        return;

    if (ui->currentPasswordtxt->text() != userData->getPassword()) {
        QMessageBox::information(this, "Ошибка", "Введённый пароль не совпадает с нынешним");
        return;
    }

    userData->updatePassword(ui->newPasswordtxt->text(), [&](bool success) {
        if (success)
            QMessageBox::information(this, "", "Пароль успешно изменён");
        else
            QMessageBox::information(this, "Ошибка", "Пароль не изменён");
    });
}
