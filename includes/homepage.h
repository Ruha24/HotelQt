#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDateEdit>
#include <QLabel>

// Test
#include <QMenu>
#include <QCalendarWidget>
#include <QDate>
#include <QVBoxLayout>
#include <QHBoxLayout>

namespace Ui {
class Homepage;
}

class Homepage : public QMainWindow
{
    Q_OBJECT

public:
    explicit Homepage(QWidget *parent = nullptr);
    ~Homepage();

private slots:
    void on_search_btn_clicked();

    void on_user_btn_clicked();

    // Function choose data
    void customDialog();
    void onApplyButtonClicked();
    void onDateSelected(const QDate& date);
    void on_startDateBtn_clicked();
    void on_endDateBtn_clicked();


    // transition on link
    void links(const QString& line);


    // trigger on button
    void on_home_btn_1_toggled(bool checked);
    void on_shop_btn_1_toggled(bool checked);
    void on_stat_btn_1_toggled(bool checked);
    void on_setting_btn_1_toggled(bool checked);

    void on_searchBtn_clicked();

    void on_pushButton_4_clicked();

    void on_label_14_linkActivated(const QString &link);

    void on_Hoteln1_clicked();

    void on_pushButton_5_clicked();

    void on_label_10_linkActivated(const QString &link);

    void on_label_4_linkActivated(const QString &link);

    void on_label_12_linkActivated(const QString &link);

    void on_label_11_linkActivated(const QString &link);

private:

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

    Ui::Homepage *ui;
};

#endif // HOMEPAGE_H
