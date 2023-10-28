#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QMainWindow>

namespace Ui {
class Homepage;
}

class Homepage : public QMainWindow
{
    Q_OBJECT

public:
    explicit Homepage(QWidget *parent = nullptr);
    ~Homepage();

private slots:
    void on_search_btn_clicked();

    void on_user_btn_clicked();

private:
    Ui::Homepage *ui;
};

#endif // HOMEPAGE_H
