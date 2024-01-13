#ifndef ROOMDETAILS_H
#define ROOMDETAILS_H

#include <QWidget>

namespace Ui {
class roomdetails;
}

class roomdetails : public QWidget
{
    Q_OBJECT

public:
    explicit roomdetails(QWidget *parent = nullptr);
    ~roomdetails();

private:
    Ui::roomdetails *ui;
};

#endif // ROOMDETAILS_H
