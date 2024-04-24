#ifndef BOOKING_H
#define BOOKING_H

#include <QMessageBox>
#include <QWidget>

#include "CustomWidget/customcalendar.h"
#include "Data/recoverydata.h"
#include "Data/roomdata.h"
#include "Data/userdata.h"

namespace Ui {
class Booking;
}

class Booking : public QWidget
{
    Q_OBJECT

public:
    Booking(QWidget *parent = nullptr, Roomdata *room = nullptr, UserData *user = nullptr);
    ~Booking();

private slots:
    void on_startTimebtn_clicked();

    void on_endTimeBtn_clicked();

    void on_recoverybtn_clicked();

    void on_pushButton_clicked();

private:
    void setPriceText(const QDate &startDate, const QDate &lastDate);

    Roomdata *m_room;

    UserData *m_userData;

    RecoveryData *m_recovery;

    CustomCalendar *m_customCalendar;

    Ui::Booking *ui;
};

#endif // BOOKING_H
