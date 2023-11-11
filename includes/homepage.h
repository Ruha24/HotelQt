#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDateEdit>
#include <QLabel>

// Test


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

    void chooseImage();

    void on_pushButton_clicked();

    // Function choose data
    void on_pushButton_6_clicked();
    void onApplyButtonClicked();
    void onDateSelected(const QDate& date);
    void on_wherebutton_clicked();

private:


    bool isStartDateSelected;

    QPushButton* applyButton;
    QLabel* startDate;
    QLabel* endDate;
    Ui::Homepage *ui;
};

#endif // HOMEPAGE_H
