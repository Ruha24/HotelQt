#ifndef ROOMDETAILS_H
#define ROOMDETAILS_H

#include <QWidget>
#include <QHBoxLayout>

namespace Ui {
class roomdetails;
}

class roomdetails : public QWidget
{
    Q_OBJECT

public:
    explicit roomdetails(QWidget *parent = nullptr);
    ~roomdetails();

    void setRoomData(const QString &name, const QString &type, double startPrice, bool isOccupied);

    void showEvent(QShowEvent *event);
private:
    Ui::roomdetails *ui;
    QVBoxLayout* infoL;
    QHBoxLayout* mainL;
    QVBoxLayout* main;
    QVBoxLayout* infoMainL;
};

#endif // ROOMDETAILS_H
