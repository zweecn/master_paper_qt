#include "businesseventwidget.h"
#include "businessevent.h"
#include <QtGui>

BusinessEventWidget::BusinessEventWidget(QWidget *parent) :
    QWidget(parent)
{
    createEventTable();
    createHistoryEventTable();

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(currEventLabel, 0, 0);
    mainLayout->addWidget(eventTable, 1, 0);
    mainLayout->addWidget(historyEventLabel, 2, 0);
    mainLayout->addWidget(historyEventTable, 3, 0);
    setLayout(mainLayout);

    setWindowTitle(tr("业务流程事件"));
    resize(400, 300);

    connect(this, SIGNAL(updateEventSignal()), this, SLOT(updateEvent()));
    connect(this, SIGNAL(updateHistoryEventSignal()), this, SLOT(updateEventHistory()));
}

void BusinessEventWidget::createEventTable()
{
    eventTable = new QTableWidget();
    eventTable->setColumnCount(7);
    eventTable->setRowCount(1);
    QStringList header;
    header << "时间编号" << "流程编号" << "活动编号" << "不确定性事件类型" << "开始时间" << "结束时间" << "决策动作";
    eventTable->setHorizontalHeaderLabels(header);
    eventTable->horizontalHeader()->setStretchLastSection(true);
    eventTable->setEditTriggers (QAbstractItemView::NoEditTriggers);
    eventTable->setSelectionMode (QAbstractItemView::NoSelection);
    eventTable->verticalHeader()->setHidden(true);
    for (int i = 0; i < 7; i++)
    {
        eventTable->setItem(0, i, new QTableWidgetItem());
        eventTable->item(0, i)->setTextAlignment(Qt::AlignCenter);
    }
    eventTable->resizeColumnsToContents();
    eventTable->setFixedHeight(52);
    currEventLabel = new QLabel(tr("当前"));
}

void BusinessEventWidget::createHistoryEventTable()
{
    historyEventTable = new QTableWidget();
    historyEventTable->setColumnCount(7);
    historyEventTable->setRowCount(1);
    QStringList header;
    header << "时间编号" << "流程编号" << "活动编号" << "不确定性事件类型" << "开始时间" << "结束时间" << "决策动作";
    historyEventTable->setHorizontalHeaderLabels(header);
    historyEventTable->horizontalHeader()->setStretchLastSection(true);
    historyEventTable->setEditTriggers (QAbstractItemView::NoEditTriggers);
    historyEventTable->setSelectionMode (QAbstractItemView::NoSelection);
    historyEventTable->verticalHeader()->setHidden(true);
    for (int i = 0; i < 7; i++)
    {
        historyEventTable->setItem(0, i, new QTableWidgetItem());
        historyEventTable->item(0, i)->setTextAlignment(Qt::AlignCenter);
    }
    historyEventTable->resizeColumnsToContents();
    historyEventLabel = new QLabel(tr("历史"));
}

void BusinessEventWidget::updateEvent()
{
    qDebug() << "BusinessEventWidget::updateEvent() ..." << event->t ;
    eventWidgetMutex.lock();
    eventTable->item(0, 0)->setText(tr("%1").arg(event->t));
    eventTable->item(0, 3)->setText(event->name());
    if (event->type != BusinessEvent::NORMAIL) {
        eventTable->item(0, 1)->setText(tr("%1").arg(event->n));
        eventTable->item(0, 2)->setText(tr("%1").arg(event->a));
        eventTable->item(0, 4)->setText(tr("%1").arg(event->t1));
        eventTable->item(0, 5)->setText(tr("%1").arg(event->t2));
    } else {
        eventTable->item(0, 1)->setText(tr(""));
        eventTable->item(0, 2)->setText(tr(""));
        eventTable->item(0, 4)->setText(tr(""));
        eventTable->item(0, 5)->setText(tr(""));
    }
    eventWidgetMutex.unlock();
    qDebug() << "BusinessEventWidget::updateEvent() finished.";
}

void BusinessEventWidget::updateEventHistory()
{
    qDebug() << "BusinessEventWidget::updateEventHistory() ...";
    eventHistoryWidgetMutex.lock();
    if (!historyEventList.isEmpty())
    {
        historyEventTable->setRowCount(historyEventList.size());
        for (int i = 0; i < historyEventTable->rowCount(); i++) {
            for (int j = 0; j < historyEventTable->columnCount(); j++) {
                if (historyEventTable->item(i, j) == (QTableWidgetItem*)NULL) {
                    historyEventTable->setItem(i, j, new QTableWidgetItem());
                    historyEventTable->item(i, j)->setTextAlignment(Qt::AlignCenter);
                }
            }
            historyEventTable->item(i, 0)->setText(tr("%1").arg(historyEventList[i].event.t));
            historyEventTable->item(i, 1)->setText(tr("%1").arg(historyEventList[i].event.n));
            historyEventTable->item(i, 2)->setText(tr("%1").arg(historyEventList[i].event.a));
            historyEventTable->item(i, 3)->setText(historyEventList[i].event.name());
            historyEventTable->item(i, 4)->setText(tr("%1").arg(historyEventList[i].event.t1));
            historyEventTable->item(i, 5)->setText(tr("%1").arg(historyEventList[i].event.t2));
            historyEventTable->item(i, 6)->setText(tr("%1").arg(historyEventList[i].action.name()));
        }
    }
    eventHistoryWidgetMutex.unlock();
    qDebug() << "BusinessEventWidget::updateEventHistory() finished.";
}

void BusinessEventWidget::setEvent(BusinessEvent *_event)
{
    event = _event;
    emit updateEventSignal();
}

 BusinessEvent * BusinessEventWidget::getEvent()
 {
     return event;
 }
void BusinessEventWidget::addBusinessEventRecordItem(BusinessEventRecordItem* _historyEventItem)
{
    bool flag = false;
    for (int i = 0; i < historyEventList.size(); i++) {
        if (historyEventList[i] == *_historyEventItem) {
            flag = true;
            break;
        }
    }
    if (!flag) {
        historyEventList.append(*_historyEventItem);
    }

    emit updateHistoryEventSignal();
}
