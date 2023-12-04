#include "homepage.h"
#include "ui_homepage.h"

Homepage::Homepage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Homepage)
{
    ui->setupUi(this);
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


void Homepage::on_pushButton_clicked()
{
    chooseImage();
}




void Homepage::on_home_btn_1_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void Homepage::on_shop_btn_1_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void Homepage::on_stat_btn_1_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}


void Homepage::on_setting_btn_1_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
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

