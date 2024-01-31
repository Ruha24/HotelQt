#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDateEdit>
#include <QLabel>
#include <QMessageBox>

// Test
#include <QMenu>
#include <QCalendarWidget>
#include <QDate>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QHttpMultiPart>

#include <QPrinter>
#include <QDateTime>
#include <QPainter>
#include <QFileDialog>

#include "includes/checkimage.h"

#include "includes/filterform.h"
#include "includes/roomdetails.h"

namespace Ui {
class Homepage;
}

class Homepage : public QMainWindow
{
    Q_OBJECT

public:
    explicit Homepage(QWidget *parent = nullptr, QString name = "");
    ~Homepage();

private slots:
    void on_search_btn_clicked();

    void connectionAll();

    void on_user_btn_clicked();

    // query
    void handleServerResponse(QNetworkReply *reply,
                              const QString &successStatus,
                              const std::function<void(QJsonObject)> &successCallback);

    // User
    void HideElement();
    void getRole(QString &name);

    void getInformation(const QString &name);

    void setImage(QPushButton* button);
    void onAnyButtonToggled();
    void getRoomInfo(const QString &text, const QString &type, bool isOccupied);

    void setRoomDataForPage(roomdetails *roomDetail, const QString &image, const QString &title);

    // Function choose data
    void customDialog();
    QLabel *createLabelWithDate();
    void updateDateButtons();
    void onDateSelected(const QDate &date);
    QString getFormattedDate(const QDate &date);
    void on_startDateBtn_clicked();
    void on_endDateBtn_clicked();
    void onApplyButtonClicked();

    QString createButtonStyleSheet(const QString &imagePath);

    // transition on link
    void links(const QString &line);

    void setPageIndex(int index);
    void setPages(int num);

    void setInfoRoom();

    // trigger on button
    void on_home_btn_1_toggled(bool checked);
    void on_shop_btn_1_toggled(bool checked);
    void on_stat_btn_1_toggled(bool checked);
    void on_setting_btn_1_toggled(bool checked);

    void on_searchBtn_clicked();
    void getRoomInformation(const QString &searchText);

    void on_pushButton_4_clicked();

    void on_label_14_linkActivated(const QString &link);

    void on_Hoteln1_clicked();

    void on_pushButton_5_clicked();

    void on_label_10_linkActivated(const QString &link);

    void on_label_4_linkActivated(const QString &link);

    void on_label_12_linkActivated(const QString &link);

    void on_label_11_linkActivated(const QString &link);


    void on_label_35_linkActivated(const QString &link);

    void on_label_37_linkActivated(const QString &link);

    void on_label_38_linkActivated(const QString &link);

    void on_label_36_linkActivated(const QString &link);

    void on_room1_1btn_clicked();

    void on_filterbtn_clicked();

public slots:
    void handleFilterAccepted(const QString& type, bool isOccupied);

private:
    QNetworkAccessManager networkManager;
    QNetworkRequest commonRequest;
    QJsonObject commonJsonObject;

    bool isStartDateSelected;
    QDialog menuDialog;
    QCalendarWidget* startDateCalendar;
    QCalendarWidget* endDateCalendar;
    QLabel* startDate;
    QLabel* endDate;
    QPushButton* applyButton;
    QVBoxLayout Vlayout;
    QHBoxLayout HlayoutCalendar;
    QHBoxLayout HlayoutDate;

    CheckImage* chcImage;

    int numPage;

    QJsonArray roomsArray;
    QString Type;
    bool IsOccupied;
    QString UserRole;

    const QString prefix = ":/image/image/";

    Ui::Homepage *ui;
};

#endif // HOMEPAGE_H
