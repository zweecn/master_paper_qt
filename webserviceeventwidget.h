#ifndef WEBSERVICEEVENTWIDGET_H
#define WEBSERVICEEVENTWIDGET_H

#include <QWidget>
#include <QWidget>
#include <QMutex>
#include <QList>

#include "allmutex.h"
#include "webserviceevent.h"
#include "webserviceeventrecorditem.h"

class QTableWidget;
class WebServiceEvent;
class QLabel;

class WebServiceEventWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WebServiceEventWidget(QWidget *parent = 0);

    void setEvent(WebServiceEvent *_event);
    void addWebServiceEventRecordItem(WebServiceEventRecordItem* _eventHistoryItem);
    WebServiceEvent * getEvent();

    QList<WebServiceEventRecordItem> & getHistoryEventList();

signals:
    void updateEventSignal();
    void updateHistoryEventSignal();
public slots:
    void updateEvent();
    void updateHistoryEvent();
private:
    void createEventTable();
    void createHistoryEventTable();

    QTableWidget *eventTable;
    QTableWidget *historyEventTable;
    QLabel *currEventLabel;
    QLabel *historyEventLabel;

//    QList<WebServiceEvent> historyEventList;
    QList<WebServiceEventRecordItem> historyEventList;
    WebServiceEvent *event;
    WebServiceEventRecordItem *eventHistoryItem;
};

#endif // WEBSERVICEEVENTWIDGET_H
