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
#include <QtNetwork>

#include "CustomWidget/customcalendar.h"
#include "Data/recoverydata.h"
#include "Data/userdata.h"
#include "authorization.h"
#include "booking.h"
#include "inforeserv.h"

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

    void on_savePasswordbtn_clicked();

    void on_aboutlbl_6_linkActivated(const QString &link);

    void on_placelbl_6_linkActivated(const QString &link);

    void on_bronlbl_6_linkActivated(const QString &link);

    void on_Title_6_linkActivated(const QString &link);

    void registerRoom(const Roomdata &room);

    void on_aboutlbl_4_linkActivated(const QString &link);

    void on_placelbl_4_linkActivated(const QString &link);

    void on_bronlbl_4_linkActivated(const QString &link);

    void on_Title_4_linkActivated(const QString &link);

private:
    void connectionElement();
    void CreateElementMenu();
    void SetVisibleUser();
    void initActionUser(QComboBox *cmb);
    void setRecovery();
    void showDetailedInfo(const RecoveryData &recovery);
    void cancelRecovery(const RecoveryData &recovery);
    void cancelAdminRecovery(const RecoveryData &recovery, UserData *user, QWidget *recoveryWidget);
    void getUserData();
    void setInformationRoom();
    void initAction();
    void getAllUsers();

    QWidget *createUserWidget(UserData *user);
    void createRecoveryWidget(UserData *user, QWidget *recoveryWidget);

    CustomCalendar *customCalendar;

    UserData *userData;

    QList<RecoveryData> recList;

    int countPlacesRoom = 2;

    bool isVisible = false;

    Roomdata *room = new Roomdata();
    QPushButton *minusButton;
    QPushButton *addButton;
    QPushButton *minusButton2;
    QPushButton *addButton2;
    QLabel *countlbl;
    QLabel *countChildlbl;
    Ui::HomePage *ui;
};
#endif // HOMEPAGE_H
