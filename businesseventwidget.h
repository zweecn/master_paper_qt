#ifndef BUSINESSEVENTWIDGET_H
#define BUSINESSEVENTWIDGET_H

#include <QWidget>

class QTableWidget;
class BusinessEvent;

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

    QTableWidget *eventTable;
    BusinessEvent *event;

};

#endif // BUSINESSEVENTWIDGET_H
