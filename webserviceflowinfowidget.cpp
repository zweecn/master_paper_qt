#include "webserviceflowinfowidget.h"
#include "workflow.h"
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

}

void WebServiceFlowInfoWidget::createFlowInfoTable()
{
    const int columnCount = 6;
    flowInfoTable = new QTableWidget();
    flowInfoLabel = new QLabel(tr("流程信息"));
    flowInfoTable->setColumnCount(columnCount);
    flowInfoTable->setRowCount(WorkFlow::Instance()->getActivitySize());
    QStringList header;
    header << "活动编号" << "服务编号" << "价格" << "执行时间" << "可靠性" << "备注";
    flowInfoTable->setHorizontalHeaderLabels(header);
    flowInfoTable->horizontalHeader()->setStretchLastSection(true);
    flowInfoTable->setEditTriggers (QAbstractItemView::NoEditTriggers);
    flowInfoTable->setSelectionMode (QAbstractItemView::NoSelection);
    flowInfoTable->verticalHeader()->setHidden(true);
    for (int i = 0; i < columnCount; i++)
    {
        flowInfoTable->setItem(0, i, new QTableWidgetItem());
        flowInfoTable->item(0, i)->setTextAlignment(Qt::AlignCenter);
    }
//    flowInfoTable->resizeColumnsToContents();
}
