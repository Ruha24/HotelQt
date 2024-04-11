#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QMainWindow>
#include <QTcpSocket>

#include <QComboBox>
#include <QLineEdit>
#include <QMenu>
#include <QMessageBox>
#include <QSignalMapper>
#include <QSpacerItem>
#include <QWidgetAction>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include "CustomWidget/customcalendar.h"
#include "User/userdata.h"
#include "authorization.h"

QT_BEGIN_NAMESPACE
namespace Ui { class HomePage; }
QT_END_NAMESPACE

class HomePage : public QMainWindow
{
    Q_OBJECT

public:
    HomePage(QWidget *parent = nullptr);
    ~HomePage();

private slots:
    // Календарь
    void on_startTimebtn_clicked();
    void on_endTimeBtn_clicked();

    void setUserData(UserData *data);

    // Выбор количества людей
    void changedMinusChild();
    void changedAddChild();
    void changedMinusGrown();
    void changedAddGrown();
    void setTextGuest();

    // Ивенты кнопок
    void on_searchbtn_clicked();
    void on_bronlbl_linkActivated(const QString &link);
    void on_placelbl_linkActivated(const QString &link);
    void on_aboutlbl_linkActivated(const QString &link);
    void on_Title_linkActivated(const QString &link);
    void on_Title_2_linkActivated(const QString &link);
    void on_Title_3_linkActivated(const QString &link);
    void on_aboutlbl_2_linkActivated(const QString &link);
    void on_placelbl_3_linkActivated(const QString &link);
    void on_bronlbl_3_linkActivated(const QString &link);
    void on_bronlbl_2_linkActivated(const QString &link);
    void on_Profile_2_clicked();
    void on_Profile_clicked();

    // Авторизация
    void authorizationUser();

    void on_Title_5_linkActivated(const QString &link);

    void on_bronlbl_5_linkActivated(const QString &link);

    void on_placelbl_5_linkActivated(const QString &link);

    void on_aboutlbl_5_linkActivated(const QString &link);

    void on_Profile_3_clicked();

    void on_saveDataUser_clicked();

private:
    void connectionElement();
    void CreateElementMenu();
    void SetVisibleUser();
    void initActionUser(QComboBox *cmb);

    CustomCalendar *customCalendar;

    UserData *userData;

    QPushButton *minusButton;
    QPushButton *addButton;
    QPushButton *minusButton2;
    QPushButton *addButton2;
    QLabel *countlbl;
    QLabel *countChildlbl;
    Ui::HomePage *ui;
};
#endif // HOMEPAGE_H
