#include "booking.h"
#include "ui_booking.h"

#include <QApplication>
#include <QScreen>

Booking::Booking(QWidget *parent, Roomdata *data)
    : QWidget(parent)
    , room(data)
    , ui(new Ui::Booking)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Popup);

    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    move(screenGeometry.center() - rect().center());

    ui->infolbl->setText(data->getDescription());
}

Booking::~Booking()
{
    delete ui;
}
