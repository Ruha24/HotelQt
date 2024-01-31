#ifndef ROOMDETAILS_H
#define ROOMDETAILS_H

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>

namespace Ui {
class roomdetails;
}

class roomdetails : public QWidget
{
    Q_OBJECT

public:
    explicit roomdetails(QWidget *parent = nullptr);
    ~roomdetails();

public slots:
    void setRoomData(const QString &name, const QStringList &infoList);

    void showEvent(QShowEvent *event);
private:
    Ui::roomdetails *ui;
    QVBoxLayout* infoL;
    QHBoxLayout* mainL;
    QVBoxLayout* main;
    QVBoxLayout* infoMainL;
};

#endif // ROOMDETAILS_H
