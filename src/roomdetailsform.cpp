#include "includes/roomdetailsform.h"
#include "ui_roomdetailsform.h"

#include <QPushButton>
#include <QVBoxLayout>

RoomDetailsForm::RoomDetailsForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RoomDetailsForm)
{
    ui->setupUi(this);

}

RoomDetailsForm::~RoomDetailsForm()
{
    delete ui;
}
#include <QGroupBox>
void RoomDetailsForm::setRoomData(const QString &name, const QString &type, double startPrice, bool isOccupied)
{
    // главный layout
    QHBoxLayout* mainL = new QHBoxLayout;

    // картинка
    QPushButton* roombtn = new QPushButton;

    QVBoxLayout* infoMainL = new QVBoxLayout;

    // текстовая информация
    QVBoxLayout* infoL = new QVBoxLayout;

    QLabel* nameLabel = new QLabel(name);
    QLabel* typeLabel = new QLabel(type);
    QLabel* priceLabel = new QLabel(QString::number(startPrice));
    QLabel* occupiedLabel = new QLabel(isOccupied ? "Occupied" : "Not Occupied");

    infoL->addWidget(nameLabel);
    infoL->addWidget(typeLabel);
    infoL->addWidget(priceLabel);
    infoL->addWidget(occupiedLabel);

    // кнопка "Подробнее"
    QPushButton* infobtn = new QPushButton("Подробнее");

    infoMainL->addLayout(infoL);
    infoMainL->addWidget(infobtn);

    mainL->addWidget(roombtn);
    mainL->addLayout(infoMainL);

    this->setLayout(mainL);
}



