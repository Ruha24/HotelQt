#include "checkimage.h"
#include "ui_checkimage.h"

CheckImage::CheckImage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CheckImage)
{
    ui->setupUi(this);
}

CheckImage::~CheckImage()
{
    delete ui;
}
