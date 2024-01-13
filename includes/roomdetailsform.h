#ifndef ROOMDETAILSFORM_H
#define ROOMDETAILSFORM_H

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>

namespace Ui {
class RoomDetailsForm;
}

class RoomDetailsForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit RoomDetailsForm(QWidget *parent = nullptr);
    ~RoomDetailsForm();

    void setRoomData(const QString &name, const QString &type, double startPrice, bool isOccupied);

private:
    Ui::RoomDetailsForm *ui;
};

#endif // ROOMDETAILSFORM_H
