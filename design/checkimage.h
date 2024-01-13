#ifndef CHECKIMAGE_H
#define CHECKIMAGE_H

#include <QWidget>

namespace Ui {
class CheckImage;
}

class CheckImage : public QWidget
{
    Q_OBJECT

public:
    explicit CheckImage(QWidget *parent = nullptr);
    ~CheckImage();

private:
    Ui::CheckImage *ui;
};

#endif // CHECKIMAGE_H
