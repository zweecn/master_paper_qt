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
    eventWidgetMutex.lock();
    qDebug() << "BusinessEventWidget::updateEvent() ..." << event->t ;
    eventTable->item(0, 0)->setText(tr("%1").arg(event->t));
//    QString type;
//    if (event->type == BusinessEvent::NORMAIL) {
//        type = "正常";
//    } else if (event->type == BusinessEvent::RESOUCE_NOT_USE) {
//        type = "资源不可用";
//    } else if (event->type == BusinessEvent::NEED_ADD) {
//        type = "需求增加";
//    } else if (event->type == BusinessEvent::NEED_REDUCE) {
//        type = "需求减少";
//    } else if (event->type == BusinessEvent::NEED_CANCEL) {
//        type = "需求取消";
//    }
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

    bool flag = false;
    for (int i = 0; i < historyEventList.size(); i++) {
        if (historyEventList[i] == *event) {
            flag = true;
            break;
        }
    }
    if (!flag) {
        historyEventList.append(*event);
    }

    historyEventTable->setRowCount(historyEventList.size());
    for (int i = 0; i < historyEventTable->rowCount(); i++) {
        for (int j = 0; j < historyEventTable->columnCount(); j++) {
            if (historyEventTable->item(i, j) == (QTableWidgetItem*)NULL) {
                historyEventTable->setItem(i, j, new QTableWidgetItem());
                historyEventTable->item(i, j)->setTextAlignment(Qt::AlignCenter);
            }
        }
        historyEventTable->item(i, 0)->setText(tr("%1").arg(historyEventList[i].t));
        historyEventTable->item(i, 1)->setText(tr("%1").arg(historyEventList[i].n));
        historyEventTable->item(i, 2)->setText(tr("%1").arg(historyEventList[i].a));
        historyEventTable->item(i, 3)->setText(historyEventList[i].name());
        historyEventTable->item(i, 4)->setText(tr("%1").arg(historyEventList[i].t1));
        historyEventTable->item(i, 5)->setText(tr("%1").arg(historyEventList[i].t2));
    }
    qDebug() << "BusinessEventWidget::updateEvent() finished.";
    eventWidgetMutex.unlock();
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
