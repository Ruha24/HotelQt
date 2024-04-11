#include "inforeserv.h"
#include "ui_inforeserv.h"

InfoReserv::InfoReserv(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InfoReserv)
{
    ui->setupUi(this);
}

InfoReserv::~InfoReserv()
{
    delete ui;
}
