#ifndef TESTUI_H
#define TESTUI_H

#include <QScrollArea>
#include <QWidget>
#include <QPushButton>
#include <QMouseEvent>

namespace Ui {
class TestUI;
}

class TestUI : public QWidget
{
    Q_OBJECT

public:
    explicit TestUI(QWidget *parent = nullptr);
    ~TestUI();

public slots:
     void labelClicked(const QString &link);

private slots:
    void scrollValCh(int val);
    void scrollToUp();


    void on_scrollUp_clicked();



    void on_label_linkActivated(const QString &link);

private:

    Ui::TestUI *ui;
};

#endif // TESTUI_H
