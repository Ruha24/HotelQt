#ifndef INFORESERV_H
#define INFORESERV_H

#include <QWidget>
#include "Data/recoverydata.h"

namespace Ui {
class InfoReserv;
}

class InfoReserv : public QWidget
{
    Q_OBJECT

public:
    explicit InfoReserv(QWidget *parent = nullptr, RecoveryData *rec = nullptr);
    ~InfoReserv();

private:
    RecoveryData *m_recovery;

    Ui::InfoReserv *ui;
};

#endif // INFORESERV_H
