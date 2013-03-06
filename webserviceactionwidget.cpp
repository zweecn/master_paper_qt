#include "webserviceactionwidget.h"
#include "resource.h"
#include "businessevent.h"
#include "allmutex.h"

#include <QtGui>
#include <QDebug>

WebServiceActionWidget::WebServiceActionWidget(QWidget *parent) :
    QWidget(parent)
{
    createActionTable();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(autoActionLabel);
    mainLayout->addWidget(actionTable);
    setLayout(mainLayout);

    setWindowTitle(tr("软件服务不确定事件动作"));
    resize(400, 300);

    connect(this, SIGNAL(updateActionsSignal()), this, SLOT(updateActionTable()));
}

void WebServiceActionWidget::createActionTable()
{
    const int columnCount = 7;
    actionTable = new QTableWidget();
    actionTable->setColumnCount(columnCount);
    actionTable->setRowCount(WebServiceAction::ACTION_SIZE);
    QStringList header;
    header << "动作类型" << "直接收益" << "潜在收益" << "成本消耗" << "时间延迟" << "继续概率" << "详细说明";
    actionTable->setHorizontalHeaderLabels(header);
    actionTable->horizontalHeader()->setStretchLastSection(true);
    actionTable->verticalHeader()->setHidden(true);
    actionTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    actionTable->setEditTriggers (QAbstractItemView::NoEditTriggers);
    actionTable->setSelectionMode (QAbstractItemView::SingleSelection);
    actionTable->setSortingEnabled(false);
    actionTable->resizeColumnsToContents();
//    actionTable->setColumnWidth(0, 150);
//    actionTable->setColumnWidth(1, 80);
//    actionTable->resizeRowsToContents();
    for (int i = 0; i < WebServiceAction::ACTION_SIZE; i++)
    {
        for (int j = 0; j < columnCount; j++)
        {
            actionTable->setItem(i, j, new QTableWidgetItem());
            actionTable->item(i, j)->setTextAlignment(Qt::AlignCenter);
        }
    }
//    QList<QString> nameList = WebServiceAction::nameList();
//    for (int i = 0; i < nameList.size(); i++)
//    {
//        actionTable->item(i, 0)->setText(nameList[i]);
//    }

    autoActionLabel = new QLabel(tr("无动作"));
    autoAction = NULL;
}

void WebServiceActionWidget::updateActionTable()
{
//    actionWidgetMutex.lock();
    for (int i = 0; i < WebServiceAction::ACTION_SIZE; i++)
    {
//        if (actions[i].isActive)
//        {
//            actionTable->item(i, 1)->setText(tr("%1").arg(actions[i].reward));
//            actionTable->item(i, 2)->setText(tr("%1").arg(actions[i].toString()));
//        }
//        else
//        {
//            actionTable->item(i, 1)->setText(tr(""));
//            actionTable->item(i, 2)->setText(tr(""));
//        }
    }


    if (autoAction != NULL)
    {
        autoActionLabel->setText(tr("自动选择动作: %1").arg(autoAction->name()));
    }
    else
    {
        autoActionLabel->setText(tr("无动作"));
    }

//    actionWidgetMutex.unlock();
}

void WebServiceActionWidget::setWebServiceAction(WebServiceAction *_actions)
{
    actions = _actions;
    autoAction = NULL;
    emit updateActionsSignal();
}

void WebServiceActionWidget::setAutoWebServiceAction(WebServiceAction *_autoAction)
{
    autoAction = _autoAction;
    emit updateActionsSignal();
}

WebServiceAction * WebServiceActionWidget::getWebServiceAction()
{
    return actions;
}

WebServiceAction * WebServiceActionWidget::getAutoWebServiceAction()
{
    return autoAction;
}

int WebServiceActionWidget::getCurrentRow()
{
    if (actionTable->currentRow() == -1
            ||actionTable->item(actionTable->currentRow(), 1)->text().isEmpty()) {
        return -1;
    }
    return actionTable->currentRow();
}
