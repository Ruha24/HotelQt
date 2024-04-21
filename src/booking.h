#ifndef BOOKING_H
#define BOOKING_H

#include <QWidget>

#include "Data/roomdata.h"

namespace Ui {
class Booking;
}

class Booking : public QWidget
{
    Q_OBJECT

public:
    explicit Booking(QWidget *parent = nullptr, Roomdata *room = nullptr);
    ~Booking();

private:
    Roomdata *room;

    Ui::Booking *ui;
};

#endif // BOOKING_H
