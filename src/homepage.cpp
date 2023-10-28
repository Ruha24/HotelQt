#include "includes/homepage.h"
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


