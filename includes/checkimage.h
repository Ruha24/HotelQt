#ifndef CHECKIMAGE_H
#define CHECKIMAGE_H

#include <QWidget>
#include <QEvent>
#include <QMouseEvent>

namespace Ui {
class CheckImage;
}

class CheckImage : public QWidget
{
    Q_OBJECT

public:
    explicit CheckImage(QWidget *parent = nullptr);
    ~CheckImage();

    void setImage(const QString &imagePath);

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
    void showEvent(QShowEvent *event) override;
private:
    QRegion createMaskRegion(const QRect &imageRect);

    Ui::CheckImage *ui;
    QWidget *darkWidget;
};

#endif // CHECKIMAGE_H
