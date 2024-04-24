#include "customcalendar.h"

CustomCalendar::CustomCalendar(QWidget *parent, int x, int y)
    : QDialog(parent)
{
    menuDialog = new QDialog(this, Qt::Popup);
    menuDialog->move(x, y);

    Vlayout = new QVBoxLayout();
    HlayoutDate = new QHBoxLayout();
    HlayoutCalendar = new QHBoxLayout();

    startDateCalendar = new QCalendarWidget();
    endDateCalendar = new QCalendarWidget();
    applyButton = new QPushButton("Применить");

    startDate = createLabelWithDate();
    endDate = createLabelWithDate();
    startDateCalendar->setMinimumDate(QDate::currentDate());
    endDateCalendar->setSelectedDate(QDate::currentDate().addMonths(1));
    endDateCalendar->setMinimumDate(QDate::currentDate().addDays(1));

    HlayoutDate->addWidget(startDate);
    HlayoutDate->addWidget(endDate);

    HlayoutCalendar->addWidget(startDateCalendar);
    HlayoutCalendar->addWidget(endDateCalendar);

    Vlayout->addLayout(HlayoutDate);
    Vlayout->addLayout(HlayoutCalendar);
    Vlayout->addWidget(applyButton);

    connect(startDateCalendar, &QCalendarWidget::clicked, this, &CustomCalendar::onDateSelected);
    connect(endDateCalendar, &QCalendarWidget::clicked, this, &CustomCalendar::onDateSelected);
    connect(applyButton, &QPushButton::clicked, this, &CustomCalendar::onApplyButtonClicked);
    connect(startDateCalendar,
            &QCalendarWidget::clicked,
            this,
            &CustomCalendar::updateEndDateMinimumDate);

    menuDialog->setLayout(Vlayout);
    menuDialog->setStyleSheet("QDialog { border: none; }");
    menuDialog->exec();
}

CustomCalendar::~CustomCalendar()
{
    delete menuDialog;
    delete Vlayout;
    delete HlayoutDate;
    delete HlayoutCalendar;
}

void CustomCalendar::getStyleSheetForDateRangeHighlight(const QDate &start, const QDate &end)
{
    QString styleSheet = "QCalendarWidget QTableView QWidget {"
                         "background-color: yellow;"
                         "}";

    startDateCalendar->setStyleSheet(styleSheet);
    endDateCalendar->setStyleSheet(styleSheet);
}

QDate CustomCalendar::getLastDate() const
{
    return lastDate;
}

void CustomCalendar::setLastDate(const QDate &newLastDate)
{
    lastDate = newLastDate;
}

QDate CustomCalendar::getDate() const
{
    return date;
}

void CustomCalendar::setDate(const QDate &newDate)
{
    date = newDate;
}

void CustomCalendar::updateEndDateMinimumDate() {}

QLabel *CustomCalendar::createLabelWithDate()
{
    QLabel *label = new QLabel(getFormattedDate(QDate::currentDate()));
    return label;
}

QString CustomCalendar::getFormattedDate(const QDate &date)
{
    static const char *months[] = {"января",
                                   "февраля",
                                   "марта",
                                   "апреля",
                                   "мая",
                                   "июня",
                                   "июля",
                                   "августа",
                                   "сентября",
                                   "октября",
                                   "ноября",
                                   "декабря"};

    return date.toString("ddd d ") + QString::fromUtf8(months[date.month() - 1]);
}

void CustomCalendar::onDateSelected(const QDate &date)
{
    QLabel *selectedLabel = isStartDateSelected ? startDate : endDate;

    if (isStartDateSelected) {
        startDateCalendar->setSelectedDate(date);
        setDate(date);
        isStartDateSelected = false;
    } else {
        if (date >= startDateCalendar->selectedDate()) {
            setLastDate(date);
            endDateCalendar->setSelectedDate(date);
            endDate->setText(getFormattedDate(date));
            getStyleSheetForDateRangeHighlight(startDateCalendar->selectedDate(), date);
        }
        isStartDateSelected = true;
    }

    selectedLabel->setText(getFormattedDate(date));
}

QString CustomCalendar::getStartDate()
{
    QStringList startDateParts = startDate->text().split(" ");
    QString startDateText = startDateParts[1] + " " + startDateParts[2];

    return startDateText;
}

QString CustomCalendar::getEndDate()
{
    QStringList endDateParts = endDate->text().split(" ");
    QString endDateText = endDateParts[1] + " " + endDateParts[2];

    return endDateText;
}

void CustomCalendar::onApplyButtonClicked()
{
    QString startDate = getStartDate();
    QString endDate = getEndDate();

    menuDialog->accept();
}
