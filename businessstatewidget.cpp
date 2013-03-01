#include "businessstatewidget.h"
#include "config.h"
#include "workflow.h"
#include "allmutex.h"
#include "activity.h"

#include <QtGui>

BusinessStateWidget::BusinessStateWidget(QWidget *parent) :
    QWidget(parent)
{
    createStateable();
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(stateTable);
    setLayout(mainLayout);
    connect(this, SIGNAL(updateStateTableSignal()), this, SLOT(updateStateTable()));
}


void BusinessStateWidget::createStateable()
{
    stateTable = new QTableWidget();
    stateTable->setColumnCount(Config::Instance()->getWorkflowCount()
                               * WorkFlow::Instance()->getActivitySize());
    stateTable->setRowCount(2);
    QStringList header;
    for (int i = 0; i < stateTable->columnCount(); i++)
    {
        int flowId = i / WorkFlow::Instance()->getActivitySize();
        int activityId = i % WorkFlow::Instance()->getActivitySize();
        header.append(QString("F%1.a%2").arg(flowId).arg(activityId));

    }
    stateTable->setHorizontalHeaderLabels(header);
    stateTable->horizontalHeader()->setStretchLastSection(true);
    stateTable->setEditTriggers (QAbstractItemView::NoEditTriggers);
    stateTable->setSelectionMode (QAbstractItemView::NoSelection);
    QStringList vHeader;
    vHeader << "服务编号" << "资源编号";
    stateTable->setVerticalHeaderLabels(vHeader);
    for (int i = 0; i < stateTable->rowCount(); i++)
    {
        for (int j = 0; j < stateTable->columnCount(); j++)
        {
            stateTable->setItem(i, j, new QTableWidgetItem());
            stateTable->item(i, j)->setTextAlignment(Qt::AlignCenter);
        }
    }
    stateTable->resizeColumnsToContents();
    //    stateTable->setFixedHeight(52);
}

void BusinessStateWidget::setActivities(Activity **_activities)
{
    activities = _activities;
    emit updateStateTableSignal();
}

void BusinessStateWidget::updateStateTable()
{
    stateWidgetMutex.lock();

    for (int j = 0; j < stateTable->columnCount(); j++)
    {
        int flowId = j / WorkFlow::Instance()->getActivitySize();
        int activityId = j % WorkFlow::Instance()->getActivitySize();
        stateTable->item(0, j)->setText(QString("%1").arg(activities[flowId][activityId].blindService->id));
        stateTable->item(1, j)->setText(QString("%1").arg(activities[flowId][activityId].resource->id));
    }

    stateWidgetMutex.unlock();
}
