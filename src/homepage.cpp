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

    ui->stackedWidget->setCurrentIndex(0);

    SetVisibleUser();
}

HomePage::~HomePage()
{
    delete ui;
}

void HomePage::setUserData(UserData *data)
{
    if (userData != nullptr) {
        delete userData;
    }

    userData = data;

    if (userData->getName() != "") {
        userData->getIdUser(userData->getName(), [&](bool success) {
            if (success) {
                initAction();
                SetVisibleUser();
            }
        });
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
    ui->Profile->setVisible(!isVisible);
    ui->Profile_2->setVisible(!isVisible);
    ui->Profile_3->setVisible(!isVisible);
    ui->Profile_4->setVisible(!isVisible);
    ui->Profile_5->setVisible(!isVisible);
    ui->Profile_6->setVisible(!isVisible);

    ui->actionUser->setVisible(isVisible);
    ui->actionUser_2->setVisible(isVisible);
    ui->actionUser_3->setVisible(isVisible);
    ui->actionUser_5->setVisible(isVisible);
    ui->actionUser_4->setVisible(isVisible);
    ui->actionUser_6->setVisible(isVisible);
    isVisible = !isVisible;
}

void HomePage::initActionUser(QComboBox *cmb)
{
    cmb->clear();
    disconnect(cmb, &QComboBox::activated, this, nullptr);

    cmb->addItem("Профиль");
    cmb->addItem("Бронирования");

    bool isAdmin = userData->getRole() == "Админ";
    if (isAdmin) {
        cmb->addItem("Админ");
    }
    cmb->addItem("Выход");

    connect(cmb, QOverload<int>::of(&QComboBox::activated), this, [=](int index) {
        if (userData != nullptr) {
            switch (index) {
            case 0:
                getUserData();
                break;
            case 1:
                setRecovery();
                break;
            case 2:
                if (isAdmin) {
                    getAllUsers();
                } else {
                    SetVisibleUser();
                    ui->stackedWidget->setCurrentIndex(0);
                    delete userData;
                    userData = nullptr;
                }
                break;
            case 3:
                SetVisibleUser();
                ui->stackedWidget->setCurrentIndex(0);
                delete userData;
                userData = nullptr;

                break;
            }
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
                QWidget *recoveryWidget = new QWidget();
                recoveryWidget->setFixedSize(900, 100);
                recoveryWidget->setStyleSheet(
                    "QWidget { background-color: #DCDCDC; border-radius: 10px}");

                QHBoxLayout *layout = new QHBoxLayout(recoveryWidget);
                layout->setAlignment(Qt::AlignCenter);

                QLabel *roomLabel = new QLabel(recovery.getRoomName());
                roomLabel->setStyleSheet("QLabel { font-size: 24px}");

                QLocale russianLocale(QLocale::Russian);

                QString startDate = russianLocale.toString(recovery.getStartDate(), "d MMMM yyyy");
                QString lastDate = russianLocale.toString(recovery.getLastDate(), "d MMMM yyyy");

                QLabel *dateLabel = new QLabel(QString("%1 - %2").arg(startDate, lastDate));

                dateLabel->setStyleSheet("QLabel { font-size: 24px}");

                QSpacerItem *spacer = new QSpacerItem(20,
                                                      20,
                                                      QSizePolicy::Expanding,
                                                      QSizePolicy::Expanding);

                QSpacerItem *spacer2 = new QSpacerItem(20,
                                                       20,
                                                       QSizePolicy::Expanding,
                                                       QSizePolicy::Expanding);

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
                layout->addSpacerItem(spacer);
                layout->addWidget(dateLabel);
                layout->addSpacerItem(spacer2);
                layout->addWidget(infoButton);
                layout->addWidget(cancelButton);

                ui->verticalLayout_14->addWidget(recoveryWidget);
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
    RecoveryData *rec = new RecoveryData(recovery.getId(),
                                         recovery.getRoomName(),
                                         recovery.getDescription(),
                                         recovery.getStartDate(),
                                         recovery.getLastDate(),
                                         recovery.getImage());

    InfoReserv *infoReserv = new InfoReserv(this, rec);

    infoReserv->show();
}

void HomePage::cancelRecovery(const RecoveryData &recovery)
{
    if (QDate::currentDate() > recovery.getStartDate()) {
        QMessageBox::information(
            this, "Ошибка", "Вы не можете отменить бронирование, потому что оно уже началось.");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,
                                  "Отмена",
                                  "Вы точно хотите отменить бронирование?",
                                  QMessageBox::Ok | QMessageBox::Cancel);

    if (reply == QMessageBox::Ok) {
        userData->deleteRecovery(recovery, [&](bool success) {
            if (success) {
                QMessageBox::information(this, "Успешно", "Вы успешно отменили бронь");
                setRecovery();
            }
        });
    }
}

void HomePage::cancelAdminRecovery(const RecoveryData &recovery,
                                   UserData *user,
                                   QWidget *recoveryWidget)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,
                                  "Отмена",
                                  "Вы точно хотите отменить бронирование?",
                                  QMessageBox::Ok | QMessageBox::Cancel);

    if (reply == QMessageBox::Ok) {
        user->deleteRecovery(recovery, [=](bool success) {
            if (success) {
                QMessageBox::information(this, "Успешно", "Вы успешно отменили бронь");

                getAllUsers();
            }
        });
    }
}

void HomePage::getUserData()
{
    userData->getUserStats(userData->getName(), [&](bool success) {
        if (success) {
            ui->userNametxt->setText(userData->getUserName());
            ui->lastnametxt->setText(userData->getLastName());
            ui->birthdaytxt->setText(userData->getBdate());
            ui->emailtxt->setText(userData->getEmail());
        }
    });

    ui->stackedWidget->setCurrentIndex(4);
}

void HomePage::setInformationRoom()
{
    clearLayout(ui->verticalLayout_4);

    ui->verticalLayout_4->setAlignment(Qt::AlignCenter);

    userData->getRooms([&](bool success) {
        if (success) {
            QList<Roomdata> roomList = userData->getListRooms();

            for (const auto &room : roomList) {
                QWidget *roomWidget = new QWidget();

                roomWidget->setFixedSize(900, 200);
                roomWidget->setStyleSheet(
                    "QWidget { background-color: #DCDCDC; border-radius: 10px}");

                QHBoxLayout *layout = new QHBoxLayout(roomWidget);

                layout->setAlignment(Qt::AlignCenter);

                QFrame *line = new QFrame();
                line->setFrameShape(QFrame::HLine);
                line->setLineWidth(2);
                line->setMidLineWidth(0);
                line->setStyleSheet("QFrame{color: #d3d3d3}");

                QPixmap pixmap(room.getImage());

                QLabel *imageLabel = new QLabel();

                if (!pixmap.isNull()) {
                    imageLabel->setPixmap(pixmap);
                }

                imageLabel->setFixedSize(300, 150);

                imageLabel->setScaledContents(true);

                QVBoxLayout *infoLayout = new QVBoxLayout();

                QLabel *nameRoom = new QLabel(room.getTypeRoom());

                nameRoom->setStyleSheet("QLabel { color: #A70303; font-size: 22px;}");

                QHBoxLayout *recLayout = new QHBoxLayout();

                QPushButton *recoverybtn = new QPushButton("Забронировать");
                QLabel *price = new QLabel("от " + QString::number(room.getStartPrice())
                                           + " руб/сутки");

                recoverybtn->setStyleSheet(
                    "QPushButton{ color: white; background-color: #B50404; "
                    "border-radius: 10px; font-size: 20px;height: 45px; width:200px; }");
                price->setStyleSheet("QLabel{ color: #050505; font-size: 20px;}");

                recLayout->addWidget(recoverybtn);
                recLayout->addWidget(price);

                QLabel *description = new QLabel(room.getDescription());
                description->setWordWrap(true);

                description->setStyleSheet(
                    "QLabel{ color: #050505; font-size: 18px; width: 250px;}");

                infoLayout->addWidget(nameRoom);
                infoLayout->addWidget(description);
                infoLayout->addLayout(recLayout);

                QSpacerItem *spacer = new QSpacerItem(20,
                                                      20,
                                                      QSizePolicy::Fixed,
                                                      QSizePolicy::Fixed);

                layout->addWidget(imageLabel);
                layout->addSpacerItem(spacer);
                layout->addLayout(infoLayout);

                connect(recoverybtn, &QPushButton::clicked, this, [=]() { registerRoom(room); });

                ui->verticalLayout_4->addWidget(line);
                ui->verticalLayout_4->addWidget(roomWidget);
            }
        }
    });

    ui->stackedWidget->setCurrentIndex(3);
}

void HomePage::initAction()
{
    initActionUser(ui->actionUser);
    initActionUser(ui->actionUser_2);
    initActionUser(ui->actionUser_3);
    initActionUser(ui->actionUser_4);
    initActionUser(ui->actionUser_5);
    initActionUser(ui->actionUser_6);
}

void HomePage::clearTabsFromIndex(int startIndex)
{
    while (ui->tabWidget->count() > startIndex) {
        QWidget *widgetToRemove = ui->tabWidget->widget(startIndex);
        if (widgetToRemove) {
            ui->tabWidget->removeTab(startIndex);
        } else {
            ++startIndex;
        }
    }
}

void HomePage::getAllUsers()
{
    clearTabsFromIndex(3);

    userData->getUsers([&](bool success) {
        if (success) {
            for (UserData *user : userData->getListUsers()) {
                QWidget *userWidget = createUserWidget(user);
                QWidget *recoveryWidget = new QWidget();

                QWidget *combinedWidget = new QWidget();

                QVBoxLayout *mainLayout = new QVBoxLayout();

                QHBoxLayout *layout = new QHBoxLayout();

                QSpacerItem *spacer = new QSpacerItem(20,
                                                      20,
                                                      QSizePolicy::Fixed,
                                                      QSizePolicy::Fixed);

                QFrame *line = new QFrame();
                line->setFrameShape(QFrame::HLine);
                line->setLineWidth(2);
                line->setMidLineWidth(0);
                line->setStyleSheet("QFrame{color: black;}");

                QFrame *line2 = new QFrame();
                line2->setFrameShape(QFrame::VLine);
                line2->setLineWidth(2);
                line2->setMidLineWidth(0);
                line2->setStyleSheet("QFrame{color: black;}");

                mainLayout->setContentsMargins(0, 0, 0, 0);
                layout->setContentsMargins(0, 0, 0, 0);
                layout->setSpacing(0);

                mainLayout->addSpacerItem(spacer);
                mainLayout->addWidget(line);
                layout->addWidget(userWidget);
                layout->addWidget(line2);
                layout->addWidget(recoveryWidget);

                mainLayout->addLayout(layout);

                combinedWidget->setLayout(mainLayout);

                createRecoveryWidget(user, recoveryWidget);

                ui->tabWidget->addTab(combinedWidget, user->getUserName());
            }
            ui->stackedWidget->setCurrentIndex(5);
        }
    });
}

void HomePage::saveStatsUser(UserData *user,
                             QString name,
                             QString lastName,
                             QString bdate,
                             QString email,
                             QString newPassword)
{
    if (name.isEmpty() || lastName.isEmpty() || bdate.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(this, "", "Все поля должны быть заполнены");
        return;
    }

    static QRegularExpression dateRegex("^\\d{2}\\.\\d{2}\\.\\d{4}$");
    if (!dateRegex.match(bdate).hasMatch()) {
        QMessageBox::warning(this, "", "Формат даты должен быть 10.10.1000");
        return;
    }

    static QRegularExpression emailRegex("[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}");
    if (!emailRegex.match(email).hasMatch()) {
        QMessageBox::warning(this, "", "Некорректный адрес электронной почты");
        return;
    }

    if (user->getUserName() != name && user->getLastName() != lastName && user->getBdate() != bdate
        && user->getEmail() != email) {
        user->updateStats(name, lastName, bdate, email, [&](bool success) {
            if (success) {
                QMessageBox::information(this, "", "Данные успешно обновлены");
                getAllUsers();
            } else
                QMessageBox::information(this, "Ошибка", "Данные не были обновлены");
        });
    }

    if (newPassword.isEmpty())
        return;

    static QRegularExpression passwordRegex("^(?=.*[A-Z])(?=.*[!@#$%^&*])(?=.*[0-9]).{8,}$");
    if (!passwordRegex.match(newPassword).hasMatch()) {
        QMessageBox::warning(
            this,
            "",
            "Некорректный пароль\nПароль должен состоять из:\n- Минимум 8 символов\n- Хотя бы "
            "одной заглавной буквы\n- Хотя бы одного специального символа\n- Хотя бы одной цифры");
        return;
    }

    user->updatePassword(newPassword, [&](bool success) {
        if (success) {
            QMessageBox::information(this, "", "Пароль успешно изменён");
            getAllUsers();
        } else
            QMessageBox::information(this, "Ошибка", "Пароль не изменён");
    });
}

QWidget *HomePage::createUserWidget(UserData *user)
{
    QWidget *uWidget = new QWidget();

    uWidget->setStyleSheet("QLineEdit { font: 14pt 'Montserrat'; height: 50px; border-radius: 5px; "
                           "border: 2px solid #ccc; } "
                           "QLineEdit:focus { border-bottom: 2px solid #28a2a2; }");

    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setLineWidth(2);
    line->setMidLineWidth(0);
    line->setStyleSheet("QFrame{color: black;}");

    QSpacerItem *spacer = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Expanding);
    QSpacerItem *spacer2 = new QSpacerItem(20, 40, QSizePolicy::Fixed, QSizePolicy::Fixed);

    QHBoxLayout *textLayout = new QHBoxLayout();

    QLabel *mainText = new QLabel("Личные данные");
    mainText->setFixedSize(250, 40);
    mainText->setStyleSheet("font-size: 22px;");
    mainText->setAlignment(Qt::AlignHCenter);

    QPushButton *saveButton = new QPushButton("Сохранить");

    saveButton->setStyleSheet("height:40px; font-size: 18px; color: black; background-color: "
                              "rgb(185, 184, 184); border-radius: 10px;");

    QHBoxLayout *hLayout1 = new QHBoxLayout();
    hLayout1->setSpacing(55);

    QVBoxLayout *vLayout1_1 = new QVBoxLayout();
    vLayout1_1->setSpacing(9);
    vLayout1_1->setContentsMargins(0, 0, 0, 0);

    QLabel *userlbl = new QLabel("Имя");
    userlbl->setStyleSheet("font-size: 24px");
    QLineEdit *usertxt = new QLineEdit();

    usertxt->setText(user->getUserName());

    QVBoxLayout *vLayout1_2 = new QVBoxLayout();
    vLayout1_2->setSpacing(9);
    vLayout1_2->setContentsMargins(0, 0, 0, 0);

    QLabel *lastNamelbl = new QLabel("Фамилия");
    lastNamelbl->setStyleSheet("font-size: 24px");
    QLineEdit *lastNametxt = new QLineEdit();
    lastNametxt->setText(user->getLastName());

    vLayout1_2->addWidget(lastNamelbl);
    vLayout1_2->addWidget(lastNametxt);

    vLayout1_1->addWidget(userlbl);
    vLayout1_1->addWidget(usertxt);

    hLayout1->addLayout(vLayout1_1);
    hLayout1->addLayout(vLayout1_2);

    QHBoxLayout *hLayout2 = new QHBoxLayout();
    hLayout2->setSpacing(55);

    QVBoxLayout *vLayout2_1 = new QVBoxLayout();
    vLayout2_1->setSpacing(9);
    vLayout2_1->setContentsMargins(0, 0, 0, 0);

    QLabel *bDatelbl = new QLabel("Дата рождения");
    bDatelbl->setStyleSheet("font-size: 24px");
    QLineEdit *bDatetxt = new QLineEdit();
    bDatetxt->setText(user->getBdate());

    QVBoxLayout *vLayout2_2 = new QVBoxLayout();
    vLayout2_2->setSpacing(9);
    vLayout2_2->setContentsMargins(0, 0, 0, 0);

    QLabel *emaillbl = new QLabel("Почта");
    emaillbl->setStyleSheet("font-size: 24px");
    QLineEdit *emailtxt = new QLineEdit();
    emailtxt->setStyleSheet("font-size: 16px");
    emailtxt->setText(user->getEmail());

    vLayout2_1->addWidget(bDatelbl);
    vLayout2_1->addWidget(bDatetxt);

    vLayout2_2->addWidget(emaillbl);
    vLayout2_2->addWidget(emailtxt);

    hLayout2->addLayout(vLayout2_1);
    hLayout2->addLayout(vLayout2_2);

    QHBoxLayout *hLayout3 = new QHBoxLayout();
    hLayout3->setSpacing(55);

    QVBoxLayout *vLayout3_1 = new QVBoxLayout();
    vLayout3_1->setSpacing(9);
    vLayout3_1->setContentsMargins(0, 0, 0, 0);

    QLabel *passwordlbl = new QLabel("Пароль");
    passwordlbl->setStyleSheet("font-size: 24px");
    QLineEdit *passwordtxt = new QLineEdit();
    passwordtxt->setReadOnly(true);
    passwordtxt->setText(user->getPassword());

    QVBoxLayout *vLayout3_2 = new QVBoxLayout();
    vLayout3_2->setSpacing(9);
    vLayout3_2->setContentsMargins(0, 0, 0, 0);

    QLabel *newPasswordlbl = new QLabel("Новый пароль");
    newPasswordlbl->setStyleSheet("font-size: 24px");
    QLineEdit *newPasswordtxt = new QLineEdit();

    vLayout3_1->addWidget(passwordlbl);
    vLayout3_1->addWidget(passwordtxt);

    vLayout3_2->addWidget(newPasswordlbl);
    vLayout3_2->addWidget(newPasswordtxt);

    hLayout3->addLayout(vLayout3_1);
    hLayout3->addLayout(vLayout3_2);

    QHBoxLayout *layoutChild = new QHBoxLayout();
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSpacing(15);

    textLayout->addSpacerItem(spacer2);
    textLayout->addWidget(mainText);
    textLayout->addSpacerItem(spacer2);

    layout->addSpacerItem(spacer);
    layout->addLayout(hLayout1);
    layout->addLayout(hLayout2);
    layout->addLayout(hLayout3);
    layout->addWidget(saveButton);
    layout->addSpacerItem(spacer);

    layoutChild->addSpacerItem(spacer);
    layoutChild->addLayout(layout);
    layoutChild->addSpacerItem(spacer);

    mainLayout->addLayout(textLayout);
    mainLayout->addWidget(line);
    mainLayout->addSpacerItem(spacer);
    mainLayout->addLayout(layoutChild);
    mainLayout->addSpacerItem(spacer);
    uWidget->setLayout(mainLayout);

    connect(saveButton, &QPushButton::clicked, this, [=]() {
        saveStatsUser(user,
                      usertxt->text().trimmed(),
                      lastNametxt->text().trimmed(),
                      bDatetxt->text().trimmed(),
                      emailtxt->text().trimmed(),
                      newPasswordtxt->text().trimmed());
    });

    return uWidget;
}

void HomePage::createRecoveryWidget(UserData *user, QWidget *recoveryWidget)
{
    QVBoxLayout *recoveryLayout = new QVBoxLayout(recoveryWidget);

    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setLineWidth(2);
    line->setMidLineWidth(0);
    line->setStyleSheet("QFrame{color: black;}");

    QHBoxLayout *textLayout = new QHBoxLayout();

    QLabel *mainText = new QLabel("Бронирования");
    mainText->setFixedSize(250, 40);
    mainText->setAlignment(Qt::AlignHCenter);
    mainText->setStyleSheet("font-size: 22px;");

    QSpacerItem *spacer2 = new QSpacerItem(20, 40, QSizePolicy::Fixed, QSizePolicy::Fixed);
    QSpacerItem *spacer = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Expanding);

    textLayout->addSpacerItem(spacer2);
    textLayout->addWidget(mainText);
    textLayout->addSpacerItem(spacer2);

    recoveryLayout->addLayout(textLayout);
    recoveryLayout->addWidget(line);
    recoveryLayout->addSpacerItem(spacer);

    QHBoxLayout *recovery = new QHBoxLayout();
    clearLayout(recovery);

    QVBoxLayout *recoveryList = new QVBoxLayout();

    recovery->addSpacerItem(spacer);
    recovery->addLayout(recoveryList);
    recovery->addSpacerItem(spacer);

    recoveryLayout->addLayout(recovery);

    user->getUserRecovery([=](bool success) {
        if (success) {
            clearLayout(recoveryList);
            QList<RecoveryData> recList = user->getListRecovery();

            for (const auto &recovery : recList) {
                QWidget *recoveryItemWidget = new QWidget();

                recoveryItemWidget->setFixedSize(700, 100);
                recoveryItemWidget->setStyleSheet(
                    "QWidget { background-color: #DCDCDC; border-radius: 10px}");

                QHBoxLayout *recoveryItemLayout = new QHBoxLayout(recoveryItemWidget);

                QLabel *roomLabel = new QLabel(recovery.getRoomName());
                roomLabel->setStyleSheet("QLabel { font-size: 24px}");

                QLocale russianLocale(QLocale::Russian);
                QString startDate = russianLocale.toString(recovery.getStartDate(), "d MMMM yyyy");
                QString lastDate = russianLocale.toString(recovery.getLastDate(), "d MMMM yyyy");
                QLabel *dateLabel = new QLabel(QString("%1 - %2").arg(startDate, lastDate));
                dateLabel->setStyleSheet("QLabel { font-size: 24px}");

                QSpacerItem *spacer = new QSpacerItem(20,
                                                      20,
                                                      QSizePolicy::Expanding,
                                                      QSizePolicy::Expanding);
                QSpacerItem *spacer2 = new QSpacerItem(20,
                                                       20,
                                                       QSizePolicy::Expanding,
                                                       QSizePolicy::Expanding);

                QPushButton *cancelButton = new QPushButton("Отменить");
                cancelButton->setStyleSheet("QPushButton { border-radius: 10px; color: white; "
                                            "background-color: #B60000; font-size: 24px}");
                cancelButton->setFixedSize(135, 55);

                recoveryItemLayout->addWidget(roomLabel);
                recoveryItemLayout->addSpacerItem(spacer);
                recoveryItemLayout->addWidget(dateLabel);
                recoveryItemLayout->addSpacerItem(spacer2);
                recoveryItemLayout->addWidget(cancelButton);

                recoveryList->addWidget(recoveryItemWidget);

                connect(cancelButton, &QPushButton::clicked, this, [=]() {
                    cancelAdminRecovery(recovery, user, recoveryWidget);
                });
            }

            if (recList.size() == 0) {
                QLabel *text = new QLabel("У пользователя нет забронированных мест");
                text->setStyleSheet("font-size: 18px;");
                text->setAlignment(Qt::AlignCenter);
                recoveryList->addWidget(text);
            }

            recoveryLayout->addSpacerItem(spacer);
        }
    });
}

void HomePage::getIdRooms()
{
    ui->indexCmb->clear();

    room->getRooms([=](bool success) {
        if (success) {
            QList<Roomdata> roomList = room->getListRooms();
            for (const auto &room : roomList) {
                ui->indexCmb->addItem(QString::number(room.getId()));
            }
        }
    });
}

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

    countPlacesRoom = countChild + countGrown;

    ui->questtlb->setText(text);
}

void HomePage::on_searchbtn_clicked()
{
    if (ui->startTimebtn->text().contains("Заезд") || ui->endTimeBtn->text().contains("Выезд")) {
        QMessageBox::critical(this, "Ошибка", "Заполните все поля");
        return;
    }

    room->getRoomsSearch(countPlacesRoom, [=](bool success) {
        if (success) {
            clearLayout(ui->verticalLayout_4);

            ui->verticalLayout_4->setAlignment(Qt::AlignCenter);

            QList<Roomdata> roomList = room->getListRooms();
            for (const auto &room : roomList) {
                QWidget *roomWidget = new QWidget();

                roomWidget->setFixedSize(900, 200);
                roomWidget->setStyleSheet(
                    "QWidget { background-color: #DCDCDC; border-radius: 10px}");

                QHBoxLayout *layout = new QHBoxLayout(roomWidget);
                layout->setAlignment(Qt::AlignCenter);

                QFrame *line = new QFrame();
                line->setFrameShape(QFrame::HLine);
                line->setLineWidth(2);
                line->setMidLineWidth(0);
                line->setStyleSheet("QFrame{color: #d3d3d3}");

                QPixmap pixmap(room.getImage());

                QLabel *imageLabel = new QLabel();

                if (!pixmap.isNull()) {
                    imageLabel->setPixmap(pixmap);
                }

                imageLabel->setScaledContents(true);

                QVBoxLayout *infoLayout = new QVBoxLayout();

                QLabel *nameRoom = new QLabel(room.getTypeRoom());

                nameRoom->setStyleSheet("QLabel { color: #A70303; font-size: 22px;}");

                QHBoxLayout *recLayout = new QHBoxLayout();

                QPushButton *recoverybtn = new QPushButton("Забронировать");
                QLabel *price = new QLabel("от " + QString::number(room.getStartPrice())
                                           + " руб/сутки");

                recoverybtn->setStyleSheet(
                    "QPushButton{ color: white; background-color: #B50404; "
                    "border-radius: 10px; font-size: 20px;height: 45px; width:200px; }");
                price->setStyleSheet("QLabel{ color: #050505; font-size: 20px;}");

                recLayout->addWidget(recoverybtn);
                recLayout->addWidget(price);

                QLabel *description = new QLabel(room.getDescription());

                description->setWordWrap(true);
                description->setStyleSheet(
                    "QLabel{ color: #050505; font-size: 18px; width: 250px;}");

                QSpacerItem *spacer = new QSpacerItem(20,
                                                      20,
                                                      QSizePolicy::Fixed,
                                                      QSizePolicy::Fixed);

                infoLayout->addWidget(nameRoom);
                infoLayout->addWidget(description);
                infoLayout->addLayout(recLayout);

                layout->addWidget(imageLabel);
                layout->addSpacerItem(spacer);
                layout->addLayout(infoLayout);

                connect(recoverybtn, &QPushButton::clicked, this, [=]() { registerRoom(room); });

                ui->verticalLayout_4->addWidget(line);
                ui->verticalLayout_4->addWidget(roomWidget);
            }

            ui->stackedWidget->setCurrentIndex(3);
        }
    });
}

void HomePage::on_bronlbl_linkActivated(const QString &link)
{
    ui->stackedWidget->setCurrentIndex(0);
}

void HomePage::on_placelbl_linkActivated(const QString &link)
{
    setInformationRoom();
}

void HomePage::on_aboutlbl_linkActivated(const QString &link)
{
    ui->stackedWidget->setCurrentIndex(2);
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
    ui->stackedWidget->setCurrentIndex(2);
}

void HomePage::on_placelbl_3_linkActivated(const QString &link)
{
    setInformationRoom();
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
    setInformationRoom();
}

void HomePage::on_aboutlbl_5_linkActivated(const QString &link)
{
    ui->stackedWidget->setCurrentIndex(2);
}

void HomePage::on_Profile_3_clicked()
{
    authorizationUser();
}

void HomePage::on_saveDataUser_clicked()
{
    if (ui->userNametxt->text().trimmed().isEmpty() || ui->lastnametxt->text().trimmed().isEmpty()
        || ui->birthdaytxt->text().trimmed().isEmpty()
        || ui->emailtxt->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "", "Все поля должны быть заполнены");
        return;
    }

    static QRegularExpression dateRegex("^\\d{2}\\.\\d{2}\\.\\d{4}$");
    if (!dateRegex.match(ui->birthdaytxt->text()).hasMatch()) {
        QMessageBox::warning(this, "", "Формат даты должен быть 10.10.1000");
        return;
    }

    static QRegularExpression emailRegex("[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}");
    if (!emailRegex.match(ui->emailtxt->text()).hasMatch()) {
        QMessageBox::warning(this, "", "Некорректный адрес электронной почты");
        return;
    }

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

    QString password = ui->newPasswordtxt->text();
    static QRegularExpression passwordRegex("^(?=.*[A-Z])(?=.*[!@#$%^&*])(?=.*[0-9]).{8,}$");
    if (!passwordRegex.match(password).hasMatch()) {
        QMessageBox::warning(
            this,
            "",
            "Некорректный пароль\nПароль должен состоять из:\n- Минимум 8 символов\n- Хотя бы "
            "одной заглавной буквы\n- Хотя бы одного специального символа\n- Хотя бы одной цифры");
        return;
    }

    userData->updatePassword(ui->newPasswordtxt->text(), [&](bool success) {
        if (success)
            QMessageBox::information(this, "", "Пароль успешно изменён");
        else
            QMessageBox::information(this, "Ошибка", "Пароль не изменён");
    });
}

void HomePage::on_aboutlbl_6_linkActivated(const QString &link)
{
    ui->stackedWidget->setCurrentIndex(2);
}

void HomePage::on_placelbl_6_linkActivated(const QString &link)
{
    setInformationRoom();
}

void HomePage::on_bronlbl_6_linkActivated(const QString &link)
{
    ui->stackedWidget->setCurrentIndex(0);
}

void HomePage::on_Title_6_linkActivated(const QString &link)
{
    ui->stackedWidget->setCurrentIndex(0);
}

void HomePage::registerRoom(const Roomdata &room)
{
    if (userData->getName() == "") {
        QMessageBox::information(this, "Ошибка", "Чтобы забронировать, войдите в свой аккаунт");
        return;
    }

    Roomdata *r = new Roomdata(room);

    Booking *booking = new Booking(nullptr, r, userData);

    booking->show();
}

void HomePage::on_aboutlbl_4_linkActivated(const QString &link)
{
    ui->stackedWidget->setCurrentIndex(2);
}

void HomePage::on_placelbl_4_linkActivated(const QString &link)
{
    setInformationRoom();
}

void HomePage::on_bronlbl_4_linkActivated(const QString &link)
{
    ui->stackedWidget->setCurrentIndex(0);
}

void HomePage::on_Title_4_linkActivated(const QString &link)
{
    ui->stackedWidget->setCurrentIndex(0);
}

void HomePage::on_tabWidget_currentChanged(int index)
{
    if (index == 1) {
        getIdRooms();
    }
}

void HomePage::on_deleteRoombtn_clicked()
{
    if (ui->plainTextEdit_2->toPlainText().isEmpty()) {
        QMessageBox::information(this, "Ошибка", "Введите причину удаления комнаты");
        return;
    }

    int idRoom = ui->indexCmb->currentText().toInt();

    room->deleteRoom(idRoom, [=](bool success) {
        if (success) {
            QMessageBox::information(this, "Комната", "Комната успешно удалена");
            getIdRooms();
        } else {
            QMessageBox::information(this, "Ошибка", "Возникла ошибка при удаление комнаты");
        }
    });
}

void HomePage::on_addRoombtn_clicked()
{
    QString description = ui->plainTextEdit->toPlainText();
    QString roomName = ui->roomtxt->text();
    QString image = ui->imagelbl->text();

    bool ok;
    int price = ui->pricetxt->text().toInt(&ok);

    if (!ok || price <= 0) {
        QMessageBox::information(this, "Ошибка", "Введите корректную цену");
        return;
    }

    if (description == "" || roomName == "") {
        QMessageBox::information(this, "Ошибка", "Заполните все поля");
        return;
    }

    if (selectedImagePath == "") {
        QMessageBox::information(this, "Ошибка", "Выберите картинку");
        return;
    }

    room->addRoom(roomName, price, description, selectedImagePath, [=](bool success) {
        if (success) {
            QMessageBox::information(this, "Успешно", "Комната была успешно добавлена");
        } else {
            QMessageBox::information(this, "Ошибка", "Возникла ошибка при добавление комнаты");
        }
    });
}

void HomePage::on_choiceImagebtn_clicked()
{
    QString imagePath = QFileDialog::getOpenFileName(this,
                                                     tr("Выберите изображение"),
                                                     "",
                                                     tr("Изображения (*.png *.jpg)"));

    if (!imagePath.isEmpty()) {
        ui->imagelbl->setPixmap(QPixmap(imagePath));

        selectedImagePath = imagePath;
    }
}
