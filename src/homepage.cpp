#include "includes/homepage.h"
#include "ui_homepage.h"

Homepage::Homepage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Homepage)
{
    ui->setupUi(this);
    ui->full_menu_widget->hide();
    customDialog();
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

    }
}

void Homepage::on_user_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
}


void Homepage::customDialog()
{
    menuDialog.setWindowFlag(Qt::Popup);

    startDateCalendar = new QCalendarWidget();
    endDateCalendar = new QCalendarWidget();
    applyButton = new QPushButton("Apply");

    QDateTime date = QDateTime::currentDateTime();
    QDate currentDate = QDate::currentDate();

    QString date_format = "MMMM мм";


    startDate = new QLabel(date.toString("ddd") + ", " + date.toString("dd MMM"));
    endDate = new QLabel(date.toString("ddd") + ", " + date.toString("dd MMM"));

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
    menuDialog.setLayout(&Vlayout);
}

void Homepage::on_endDateBtn_clicked()
{
    menuDialog.exec();
}


void Homepage::on_startDateBtn_clicked()
{
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

void Homepage::on_home_btn_1_toggled(bool checked)
{
    ui->stackedWidget->setCurrentIndex(0);
}

void Homepage::on_shop_btn_1_toggled(bool checked)
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Homepage::on_stat_btn_1_toggled(bool checked)
{
    ui->stackedWidget->setCurrentIndex(3);
}

void Homepage::on_setting_btn_1_toggled(bool checked)
{
    ui->stackedWidget->setCurrentIndex(4);
}


void Homepage::onApplyButtonClicked()
{
    QStringList startDateParts = startDate->text().split(" ");
    QStringList endDateParts = endDate->text().split(" ");

    QString startDateText = startDateParts[1] + " " + startDateParts[2];
    QString endDateText = endDateParts[1] + " " + endDateParts[2];

    ui->startDateBtn->setText("Заезд\n" + startDateText);

    ui->endDateBtn->setText("Выезд\n" + endDateText);

    menuDialog.accept();
}

void Homepage::on_searchBtn_clicked()
{

}

void Homepage::on_pushButton_4_clicked()
{

}

void Homepage::links(const QString &link)
{
    if(link == "#stand1") ui->stackedWidget->setCurrentIndex(2);

}

void Homepage::on_label_14_linkActivated(const QString &link)
{
    links(link);
}


void Homepage::on_label_10_linkActivated(const QString &link)
{
    links(link);
}

void Homepage::on_label_4_linkActivated(const QString &link)
{
    links(link);
}

void Homepage::on_label_12_linkActivated(const QString &link)
{
    links(link);
}

void Homepage::on_label_11_linkActivated(const QString &link)
{
    links(link);
}


void Homepage::on_Hoteln1_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}



void Homepage::on_pushButton_5_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}




