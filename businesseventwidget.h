#ifndef BUSINESSEVENTWIDGET_H
#define BUSINESSEVENTWIDGET_H

#include <QWidget>
#include <QMutex>
#include <QList>

#include "allmutex.h"
#include "businessevent.h"

class QTableWidget;
class BusinessEvent;
class QLabel;

class BusinessEventWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BusinessEventWidget(QWidget *parent = 0);

    void setEvent(BusinessEvent *_event);
    BusinessEvent * getEvent();

signals:
    void updateEventSignal();
public slots:
    void updateEvent();
private:
    void createEventTable();
    void createHistoryEventTable();

    QTableWidget *eventTable;
    QTableWidget *historyEventTable;
    QLabel *currEventLabel;
    QLabel *historyEventLabel;
    QList<BusinessEvent> historyEventList;

    BusinessEvent *event;

};

#endif // BUSINESSEVENTWIDGET_H
