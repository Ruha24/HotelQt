#include "includes/roomdetails.h"
#include "ui_roomdetails.h"

roomdetails::roomdetails(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::roomdetails)
{
    ui->setupUi(this);

    main = new QVBoxLayout(this);
}

roomdetails::~roomdetails()
{
    delete ui;
}

void roomdetails::setRoomData(const QString &name, const QStringList &infoList)
{
    QPushButton *roombtn = new QPushButton(this);
    roombtn->setMinimumSize(250, 150);
    roombtn->setMaximumSize(250, 250);
    roombtn->setStyleSheet(
        "QPushButton { border: none; border-image: url(:/image/image/twoplace1_1.jpg)  0 0 0 0 "
        "stretch; } QPushButton:hover { border: 5px solid red;}");

    mainL = new QHBoxLayout;
    infoMainL = new QVBoxLayout;
    infoMainL->setSpacing(2);
    infoMainL->setContentsMargins(0, 0, 0, 0);

    QLabel *nameLabel = new QLabel(name, this);

    QVBoxLayout *infoRoom = new QVBoxLayout(this);
    infoRoom->setSpacing(2);
    infoRoom->setContentsMargins(0, 0, 0, 0);

    QLabel *texts = new QLabel("В номере", this);

    infoRoom->addWidget(texts);

    for (const QString &info : infoList) {
        QTextEdit *infoLabel = new QTextEdit(info, this);
        infoLabel->setReadOnly(true);
        infoLabel->setStyleSheet(
            "border: none; background-color: rgba(0, 0, 0, 0); margin: 0; padding: 0;");
        infoRoom->addWidget(infoLabel);
    }

    QPushButton *infobtn = new QPushButton("Подробнее", this);
    infobtn->setMaximumSize(150, 50);
    infobtn->setMinimumSize(150, 50);
    infobtn->setStyleSheet(
        "QPushButton { color: white; background-color: red; border-radius: 5px;}");

    infoMainL->addLayout(infoRoom);
    infoMainL->addWidget(infobtn);

    mainL->addWidget(roombtn);
    mainL->addLayout(infoMainL);

    main->addWidget(nameLabel);
    main->addLayout(mainL);
}

void roomdetails::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
}
