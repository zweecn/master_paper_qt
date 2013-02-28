#include "businesseventwidget.h"
#include "businessevent.h"
#include <QtGui>

BusinessEventWidget::BusinessEventWidget(QWidget *parent) :
    QWidget(parent)
{
    createEventTable();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(eventTable);
    setLayout(mainLayout);

    setWindowTitle(tr("ҵ�������¼�"));
    resize(400, 300);

    connect(this, SIGNAL(updateEventSignal()), this, SLOT(updateEvent()));
}

void BusinessEventWidget::createEventTable()
{
    eventTable = new QTableWidget();
    eventTable->setColumnCount(7);
    eventTable->setRowCount(1);
    QStringList header;
    header << "ʱ��" << "���̺�" << "���" << "�¼�����" << "��ʼʱ��" << "����ʱ��" << "��ע";
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
}

void BusinessEventWidget::updateEvent()
{
    eventTable->item(0, 0)->setText(tr("%1").arg(event->t));
    QString type;
    if (event->type == BusinessEvent::NORMAIL) {
        type = "����";
    } else if (event->type == BusinessEvent::RESOUCE_NOT_USE) {
        type = "��Դ������";
    } else if (event->type == BusinessEvent::NEED_ADD) {
        type = "��������";
    } else if (event->type == BusinessEvent::NEED_REDUCE) {
        type = "�������";
    } else if (event->type == BusinessEvent::NEED_CANCEL) {
        type = "����ȡ��";
    }
    eventTable->item(0, 3)->setText(type);
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
