#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QWidget>

#include "authorization.h"

namespace Ui {
class Registration;
}

class Registration : public QWidget
{
    Q_OBJECT

public:
    explicit Registration(QWidget *parent = nullptr);
    ~Registration();

private slots:
    void on_buttonReg_clicked();

private:
    UserData *m_userData;

    Ui::Registration *ui;
};

#endif // REGISTRATION_H
