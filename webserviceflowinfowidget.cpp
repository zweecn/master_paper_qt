#include "webserviceflowinfowidget.h"
#include "workflow.h"
#include "allmutex.h"

#include <QtGui>

WebServiceFlowInfoWidget::WebServiceFlowInfoWidget(QWidget *parent) :
    QWidget(parent)
{
    createFlowInfoTable();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(flowInfoTable);
    setLayout(mainLayout);

    setWindowTitle(tr("流程信息"));
    resize(400, 300);

    connect(this, SIGNAL(updateFlowInfoTableSignal()), this, SLOT(updateFlowInfo()));
}

void WebServiceFlowInfoWidget::createFlowInfoTable()
{
    const int columnCount = 6;
    flowInfoTable = new QTableWidget();
    flowInfoLabel = new QLabel(tr("流程信息"));
    flowInfoTable->setColumnCount(columnCount);
    flowInfoTable->setRowCount(WorkFlow::Instance()->getActivitySize());
    QStringList header;
    header << "活动编号" << "服务编号" << "价格" << "执行时间" << "可靠性" << "已执行";
    flowInfoTable->setHorizontalHeaderLabels(header);
    flowInfoTable->horizontalHeader()->setStretchLastSection(true);
    flowInfoTable->setEditTriggers (QAbstractItemView::NoEditTriggers);
    flowInfoTable->setSelectionMode (QAbstractItemView::NoSelection);
    flowInfoTable->verticalHeader()->setHidden(true);
    for (int i = 0; i < flowInfoTable->rowCount(); i++)
    {
        for (int j = 0; j < columnCount; j++)
        {
            flowInfoTable->setItem(i, j, new QTableWidgetItem());
            flowInfoTable->item(i, j)->setTextAlignment(Qt::AlignCenter);
        }
    }
    //    flowInfoTable->resizeColumnsToContents();
}

void WebServiceFlowInfoWidget::setWebServiceFlow(WebServiceFlow *_wsf)
{
    wsf = _wsf;
    emit updateFlowInfoTableSignal();
}

void WebServiceFlowInfoWidget::updateFlowInfo()
{
    serviceFlowInfoWidgetMutex.lock();
    for (int i = 0; i < flowInfoTable->rowCount(); i++)
    {
        flowInfoTable->item(i, 0)->setText(tr("%1").arg(i));
        flowInfoTable->item(i, 1)->setText(tr("%1").arg(wsf->activities[i].blindService->id));
        flowInfoTable->item(i, 2)->setText(tr("%1").arg(wsf->activities[i].blindService->price));
        flowInfoTable->item(i, 3)->setText(tr("%1").arg(wsf->activities[i].blindService->execTime));
        flowInfoTable->item(i, 4)->setText(tr("%1 %").arg((double)wsf->activities[i].blindService->reliability));
        flowInfoTable->item(i, 5)->setText(tr("%1 %").arg(wsf->activities[i].x * 100));
    }
    serviceFlowInfoWidgetMutex.unlock();
}
