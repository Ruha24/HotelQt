#include "inforeserv.h"
#include "ui_inforeserv.h"

#include <QApplication>
#include <QScreen>

InfoReserv::InfoReserv(QWidget *parent, RecoveryData *rec)
    : QWidget(parent)
    , m_recovery(rec)
    , ui(new Ui::InfoReserv)
{
    setWindowFlags(Qt::Popup);
    ui->setupUi(this);

    ui->label_3->setText(m_recovery->getDescription());

    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    move(screenGeometry.center() - rect().center());

    QPixmap pixmap(rec->getImage());

    if (!pixmap.isNull()) {
        ui->label->setPixmap(pixmap);
    }

    ui->label->setScaledContents(true);
}

InfoReserv::~InfoReserv()
{
    delete ui;
}
