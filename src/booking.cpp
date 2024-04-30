#include "booking.h"
#include "ui_booking.h"

#include <QApplication>
#include <QScreen>

Booking::Booking(QWidget *parent, Roomdata *data, UserData *user)
    : QWidget(parent)
    , m_room(data)
    , m_userData(user)
    , ui(new Ui::Booking)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(0);

    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    move(screenGeometry.center() - rect().center());

    ui->infolbl->setText(data->getDescription());
}

Booking::~Booking()
{
    delete ui;
}

void Booking::on_startTimebtn_clicked()
{
    QRect buttonRect = ui->startTimebtn->geometry();
    QPoint buttonCenter = buttonRect.center();
    QPoint buttonPos = ui->startTimebtn->mapTo(this, QPoint(0, 0));

    m_customCalendar = new CustomCalendar(this,
                                          buttonCenter.x() + ui->startTimebtn->width(),
                                          buttonPos.y() + ui->startTimebtn->height());

    ui->startTimebtn->setText(m_customCalendar->getStartDate());
    ui->endTimeBtn->setText(m_customCalendar->getEndDate());

    setPriceText(m_customCalendar->getDate(), m_customCalendar->getLastDate());
}

void Booking::on_endTimeBtn_clicked()
{
    QRect buttonRect = ui->endTimeBtn->geometry();
    QPoint buttonCenter = buttonRect.center();
    QPoint buttonPos = ui->endTimeBtn->mapTo(this, QPoint(0, 0));

    m_customCalendar = new CustomCalendar(this,
                                          buttonCenter.x() + ui->endTimeBtn->width(),
                                          buttonPos.y() + ui->endTimeBtn->height());

    ui->startTimebtn->setText(m_customCalendar->getStartDate());
    ui->endTimeBtn->setText(m_customCalendar->getEndDate());

    setPriceText(m_customCalendar->getDate(), m_customCalendar->getLastDate());
}

void Booking::setPriceText(const QDate &startDate, const QDate &lastDate)
{
    if (!startDate.isValid() || !lastDate.isValid() || startDate > lastDate)
        return;

    int daysDifference = startDate.daysTo(lastDate);

    int totalPrice = m_room->getStartPrice() * daysDifference;

    QString dayWord;
    if (daysDifference == 1)
        dayWord = "день";
    else if (daysDifference > 1 && daysDifference < 5)
        dayWord = "дня";
    else
        dayWord = "дней";

    m_recovery = new RecoveryData(1,
                                  m_room->getTypeRoom(),
                                  m_room->getDescription(),
                                  startDate,
                                  lastDate);

    qDebug() << "Цена поставилась";
    ui->pricelbl->setText(
        QString("Цена: %1 руб за %2 %3").arg(totalPrice).arg(daysDifference).arg(dayWord));
}

void Booking::on_recoverybtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Booking::on_pushButton_clicked()
{
    if (ui->pricelbl->text().isEmpty()) {
        QMessageBox::information(this, "Ошибка", "Выберите дату заезда/выезда");
        return;
    }

    m_userData->recoveryRoom(m_room, m_recovery, [&](bool success) {
        if (success) {
            QMessageBox::information(this, "", "Вы забронировали место");
            this->close();
        }
    });
}
