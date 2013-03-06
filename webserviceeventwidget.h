#ifndef WEBSERVICEEVENTWIDGET_H
#define WEBSERVICEEVENTWIDGET_H

#include <QWidget>
#include <QWidget>
#include <QMutex>
#include <QList>

#include "allmutex.h"
#include "webserviceevent.h"

class QTableWidget;
class WebServiceEvent;
class QLabel;

class WebServiceEventWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WebServiceEventWidget(QWidget *parent = 0);

    void setEvent(WebServiceEvent *_event);
    WebServiceEvent * getEvent();

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

    QList<WebServiceEvent> historyEventList;

    WebServiceEvent *event;

};

#endif // WEBSERVICEEVENTWIDGET_H
