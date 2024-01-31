#include "includes/checkimage.h"
#include "ui_checkimage.h"
#include <QGraphicsOpacityEffect>

CheckImage::CheckImage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CheckImage)
{
    ui->setupUi(this);
    installEventFilter(this);

    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    ui->Imagelbl->setAlignment(Qt::AlignCenter);
}

CheckImage::~CheckImage()
{
    delete ui;
}

void CheckImage::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);

    QList<QScreen*> screens = QGuiApplication::screens();
    if (!screens.isEmpty()) {
        setFixedSize(screens.first()->availableGeometry().size());
        move(screens.first()->availableGeometry().center() - rect().center());
    }
}

void CheckImage::setImage(const QString &imagePath)
{
    QPixmap pixmap(imagePath);
    ui->Imagelbl->setPixmap(pixmap);
    update();
}

bool CheckImage::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);

        if (mouseEvent) {
            QImage sizeImage = ui->Imagelbl->pixmap().toImage();
            QRect imageRect((ui->Imagelbl->width() - sizeImage.width()) / 2,
                            (ui->Imagelbl->height() - sizeImage.height()) / 2,
                            sizeImage.width(),
                            sizeImage.height());

            if (!imageRect.contains(mouseEvent->pos()))
                close();
        }
    }
    return QWidget::eventFilter(obj, event);
}
