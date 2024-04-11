#ifndef CUSTOMCALENDAR_H
#define CUSTOMCALENDAR_H

#include <QCalendarWidget>
#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class CustomCalendar : public QDialog
{
    Q_OBJECT

public:
    CustomCalendar(QWidget *parent = nullptr, int x = 0, int y = 0);
    ~CustomCalendar();

    QString getEndDate();
    QString getStartDate();

private slots:
    void onDateSelected(const QDate &date);
    void onApplyButtonClicked();

signals:
    void datesSelected(const QString &startDate, const QString &endDate);

private:
    QLabel *createLabelWithDate();
    QString getFormattedDate(const QDate &date);
    void updateEndDateMinimumDate();
    void getStyleSheetForDateRangeHighlight(const QDate &start, const QDate &end);

    QDialog *menuDialog;
    bool isStartDateSelected;
    QCalendarWidget *startDateCalendar;
    QCalendarWidget *endDateCalendar;
    QLabel *startDate;
    QLabel *endDate;
    QPushButton *applyButton;
    QVBoxLayout *Vlayout;
    QHBoxLayout *HlayoutCalendar;
    QHBoxLayout *HlayoutDate;
};

#endif // CUSTOMCALENDAR_H
