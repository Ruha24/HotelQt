#include "includes/roomdetails.h"
#include "ui_roomdetails.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

roomdetails::roomdetails(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::roomdetails)
{
    ui->setupUi(this);

    // главный layout
    main = new QVBoxLayout(this);


}

roomdetails::~roomdetails()
{
    delete ui;
}

void roomdetails::setRoomData(const QString &name, const QString &type, double startPrice, bool isOccupied)
{
    QPushButton* roombtn = new QPushButton(this);
    roombtn->setMinimumSize(250, 150);
    roombtn->setMaximumSize(250, 250);

    mainL = new QHBoxLayout;
    infoMainL = new QVBoxLayout;

    QLabel* Title = new QLabel("В комнате",this);
    QLabel* nameLabel = new QLabel(name, this);
    QLabel* typeLabel = new QLabel(type, this);
    QLabel* priceLabel = new QLabel(QString::number(startPrice), this);
    QLabel* occupiedLabel = new QLabel(isOccupied ? "Занято" : "Не занято", this);

    QPushButton* infobtn = new QPushButton("Подробнее", this);
    infobtn->setMaximumSize(150, 50);
    infobtn->setMinimumSize(150, 50);
    infobtn->setStyleSheet("QPushButton { color: white; background-color: red; border-radius: 5px;}");

    infoMainL->addWidget(Title);
    infoMainL->addWidget(nameLabel);
    infoMainL->addWidget(typeLabel);
    infoMainL->addWidget(priceLabel);
    infoMainL->addWidget(occupiedLabel);
    infoMainL->addWidget(infobtn);

    mainL->addWidget(roombtn);
    mainL->addLayout(infoMainL);

    main->addLayout(mainL);
}


void roomdetails::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
}
