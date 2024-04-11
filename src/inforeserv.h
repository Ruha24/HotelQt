#ifndef INFORESERV_H
#define INFORESERV_H

#include <QWidget>

namespace Ui {
class InfoReserv;
}

class InfoReserv : public QWidget
{
    Q_OBJECT

public:
    explicit InfoReserv(QWidget *parent = nullptr);
    ~InfoReserv();

private:
    Ui::InfoReserv *ui;
};

#endif // INFORESERV_H
