#include "includes/testui.h"
#include "ui_testui.h"
#include <QScrollBar>

#include <QSpacerItem>

TestUI::TestUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestUI)
{
    ui->setupUi(this);
    connect(ui->scrollArea->verticalScrollBar(), &QScrollBar::valueChanged, this, &TestUI::scrollValCh);
}

void TestUI::labelClicked(const QString &link)
{
    if(link == "#")
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
}

TestUI::~TestUI()
{
    delete ui;
}

void TestUI::scrollValCh(int val)
{
    qDebug() << val;
    qDebug() <<  0.5 * ui->scrollUp->height();
    if(val > 250)
        ui->scrollUp->show();
    else
        ui->scrollUp->hide();
}

void TestUI::scrollToUp()
{
    ui->scrollUp->hide();
    ui->scrollArea->verticalScrollBar()->setValue(0);
}

void TestUI::on_scrollUp_clicked()
{
    scrollToUp();
}

void TestUI::on_label_linkActivated(const QString &link)
{
    labelClicked(link);
}

