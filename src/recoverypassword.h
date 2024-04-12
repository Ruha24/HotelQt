#ifndef RECOVERYPASSWORD_H
#define RECOVERYPASSWORD_H

#include <QWidget>
#include "User/userdata.h"

namespace Ui {
class RecoveryPassword;
}

class RecoveryPassword : public QWidget
{
    Q_OBJECT

public:
    explicit RecoveryPassword(QWidget *parent = nullptr, UserData *userData = nullptr);
    ~RecoveryPassword();

private:
    UserData *m_userData;

    Ui::RecoveryPassword *ui;
};

#endif // RECOVERYPASSWORD_H
