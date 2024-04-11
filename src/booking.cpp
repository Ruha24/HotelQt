#include "booking.h"
#include "ui_booking.h"

Booking::Booking(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Booking)
{
    ui->setupUi(this);
}

Booking::~Booking()
{
    delete ui;
}
