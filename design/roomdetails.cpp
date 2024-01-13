#include "roomdetails.h"
#include "ui_roomdetails.h"

roomdetails::roomdetails(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::roomdetails)
{
    ui->setupUi(this);
}

roomdetails::~roomdetails()
{
    delete ui;
}
