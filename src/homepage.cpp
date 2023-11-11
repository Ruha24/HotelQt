#include "includes/homepage.h"
#include "ui_homepage.h"

Homepage::Homepage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Homepage)
{
    ui->setupUi(this);
    ui->full_menu_widget->hide();



}

Homepage::~Homepage()
{
    delete ui;
}

void Homepage::on_search_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
    QString search_text = ui->search_input->text();

    if(!search_text.isEmpty())
    {
        ui->label_10->setText(search_text);
    }
}

void Homepage::on_user_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
}


void Homepage::chooseImage()
{

}

void Homepage::on_pushButton_clicked()
{
    chooseImage();
}

#include <QMenu>
#include <QCalendarWidget>
#include <QDate>

void Homepage::on_pushButton_6_clicked()
{
    QDialog menuDialog(this);
    menuDialog.setWindowFlag(Qt::Popup);


    QVBoxLayout Vlayout(&menuDialog);
    QHBoxLayout HlayoutCalendar;
    QHBoxLayout HlayoutDate;

    QCalendarWidget* startDateCalendar = new QCalendarWidget();
    QCalendarWidget* endDateCalendar = new QCalendarWidget();
    applyButton = new QPushButton("Apply");

    QDateTime date = QDateTime::currentDateTime();
    QDate currentDate = QDate::currentDate();

    QString date_format = "MMMM мм";
    startDate = new QLabel("" + date.toString(date_format));
    endDate = new QLabel("" + date.addMonths(1).toString(date_format));

    startDateCalendar->setMinimumDate(currentDate);
    endDateCalendar->setSelectedDate(currentDate.addMonths(1));

    HlayoutDate.addWidget(startDate);
    HlayoutDate.addWidget(endDate);

    HlayoutCalendar.addWidget(startDateCalendar);
    HlayoutCalendar.addWidget(endDateCalendar);


    Vlayout.addLayout(&HlayoutDate);
    Vlayout.addLayout(&HlayoutCalendar);
    Vlayout.addWidget(applyButton);

    connect(applyButton, &QPushButton::clicked, this, &Homepage::onApplyButtonClicked);
    connect(startDateCalendar, &QCalendarWidget::clicked, this, &Homepage::onDateSelected);
    connect(endDateCalendar, &QCalendarWidget::clicked, this, &Homepage::onDateSelected);
    menuDialog.exec();
}

void Homepage::onDateSelected(const QDate& date)
{
    if(isStartDateSelected)
    {
        startDate->setText(date.toString("ddd") + ", " + date.toString("dd MMM"));
        isStartDateSelected = false;
    }
    else
    {
        endDate->setText(date.toString("ddd") + ", " + date.toString("dd MMM"));
        isStartDateSelected = true;
    }
}


void Homepage::on_wherebutton_clicked()
{
}



void Homepage::onApplyButtonClicked()
{
    QStringList startDateParts = startDate->text().split(" ");
    QStringList endDateParts = endDate->text().split(" ");
    QString startDateText = startDateParts[1] + " " + startDateParts[2];
    QString endDateText = endDateParts[1] + " " + endDateParts[2];
    QString buttonText = "Dates\n" + startDateText + " -> " + endDateText;
    ui->pushButton_6->setText(buttonText);
}




