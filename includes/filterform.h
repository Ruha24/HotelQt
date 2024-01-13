#ifndef FILTERFORM_H
#define FILTERFORM_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
namespace Ui {
class FilterForm;
}

class FilterForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit FilterForm(QWidget *parent = nullptr);
    ~FilterForm();

private slots:
    void on_acceptbtn_clicked();

signals:
    void filterAccepted(const QString& type, bool isOccupied);


private:
    void getTypeRoom();
    Ui::FilterForm *ui;
};

#endif // FILTERFORM_H
