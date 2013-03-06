#include "webserviceeventwidget.h"
#include <QtGui>

WebServiceEventWidget::WebServiceEventWidget(QWidget *parent) :
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
}

void WebServiceEventWidget::createEventTable()
{
    const int columnCount = 3;
    eventTable = new QTableWidget();
    eventTable->setColumnCount(columnCount);
    eventTable->setRowCount(1);
    QStringList header;
    header << "时间编号" << "活动编号" << "不确定性事件类型";
    eventTable->setHorizontalHeaderLabels(header);
    eventTable->horizontalHeader()->setStretchLastSection(true);
    eventTable->setEditTriggers (QAbstractItemView::NoEditTriggers);
    eventTable->setSelectionMode (QAbstractItemView::NoSelection);
    eventTable->verticalHeader()->setHidden(true);
    for (int i = 0; i < columnCount; i++)
    {
        eventTable->setItem(0, i, new QTableWidgetItem());
        eventTable->item(0, i)->setTextAlignment(Qt::AlignCenter);
    }
    eventTable->resizeColumnsToContents();
    eventTable->setFixedHeight(52);
    currEventLabel = new QLabel(tr("当前"));
}

void WebServiceEventWidget::createHistoryEventTable()
{
    const int columnCount = 8;
    historyEventTable = new QTableWidget();
    historyEventTable->setColumnCount(columnCount);
    historyEventTable->setRowCount(1);
    QStringList header;
    header << "时间编号" << "活动编号" << "不确定性事件类型" << "已采取动作" << "潜在收益"
           << "成本消耗" << "时间延迟" << "成功概率";
    historyEventTable->setHorizontalHeaderLabels(header);
    historyEventTable->horizontalHeader()->setStretchLastSection(true);
    historyEventTable->setEditTriggers (QAbstractItemView::NoEditTriggers);
    historyEventTable->setSelectionMode (QAbstractItemView::NoSelection);
    historyEventTable->verticalHeader()->setHidden(true);
    for (int i = 0; i < columnCount; i++)
    {
        historyEventTable->setItem(0, i, new QTableWidgetItem());
        historyEventTable->item(0, i)->setTextAlignment(Qt::AlignCenter);
    }
    historyEventTable->resizeColumnsToContents();
    historyEventLabel = new QLabel(tr("历史"));
}

void WebServiceEventWidget::updateEvent()
{
    eventWidgetMutex.lock();
    qDebug() << "WebServiceEventWidget::updateEvent() ..." << event->t ;
    eventTable->item(0, 0)->setText(tr("%1").arg(event->t));
    eventTable->item(0, 1)->setText(tr("%1").arg(event->a));
    eventTable->item(0, 2)->setText(event->name());
    qDebug() << "WebServiceEventWidget::updateEvent() finished.";
    eventWidgetMutex.unlock();
}

void WebServiceEventWidget::updateHistoryEvent()
{
    eventHistoryWidgetMutex.lock();
//    historyEventTable->setRowCount(historyEventList.size());
//    for (int i = 0; i < historyEventTable->rowCount(); i++) {
//        for (int j = 0; j < historyEventTable->columnCount(); j++) {
//            if (historyEventTable->item(i, j) == (QTableWidgetItem*)NULL) {
//                historyEventTable->setItem(i, j, new QTableWidgetItem());
//                historyEventTable->item(i, j)->setTextAlignment(Qt::AlignCenter);
//            }
//        }
//        historyEventTable->item(i, 0)->setText(tr("%1").arg(historyEventList[i].t));
//        historyEventTable->item(i, 1)->setText(tr("%1").arg(historyEventList[i].a));
//        historyEventTable->item(i, 2)->setText(historyEventList[i].name());
//        historyEventTable->item(i, 3)->setText(tr(""));
//        historyEventTable->item(i, 4)->setText(tr(""));
//        historyEventTable->item(i, 5)->setText(tr(""));
//        historyEventTable->item(i, 6)->setText(tr(""));
//        historyEventTable->item(i, 7)->setText(tr(""));
//    }
    eventHistoryWidgetMutex.unlock();
}

void WebServiceEventWidget::setEvent(WebServiceEvent *_event)
{
    event = _event;
    emit updateEventSignal();
}

WebServiceEvent * WebServiceEventWidget::getEvent()
{
    return event;
}

void WebServiceEventWidget::addWebServiceEventRecordItem(WebServiceEventRecordItem* _eventHistoryItem)
{
    bool flag = false;
    for (int i = 0; i < historyEventList.size(); i++) {
        if (historyEventList[i] == (*_eventHistoryItem)) {
            flag = true;
            break;
        }
    }
    if (!flag) {
        historyEventList.append(*_eventHistoryItem);
    }
    emit updateHistoryEventSignal();
}
