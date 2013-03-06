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

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(markovActionLabel, 0, 0);
    mainLayout->addWidget(greedyActionLabel, 0, 1);
    mainLayout->addWidget(actionTable, 1, 0, 1, 2);
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
    header << "动作类型" << "直接收益(Greedy)" << "潜在收益(MDP)"
           << "成本消耗" << "时间延迟" << "继续概率" << "详细说明";
    actionTable->setHorizontalHeaderLabels(header);
    actionTable->horizontalHeader()->setStretchLastSection(true);
    actionTable->verticalHeader()->setHidden(true);
    actionTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    actionTable->setEditTriggers (QAbstractItemView::NoEditTriggers);
    actionTable->setSelectionMode (QAbstractItemView::SingleSelection);
    actionTable->setSortingEnabled(false);
    actionTable->resizeColumnsToContents();
    actionTable->setColumnWidth(0, 90);
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
    markovActionLabel = new QLabel(tr("MDP决策动作"));
    greedyActionLabel = new QLabel(tr("Greedy决策动作"));
    autoAction = NULL;
}

void WebServiceActionWidget::updateActionTable()
{
    actionWidgetMutex.lock();
    int markovActionId = -1;
    int greedyActionId = -1;
    for (int i = 0; i < markovResult.size(); i++)
    {
        actionTable->item(i, 0)->setText(markovResult[i].action.name());
        actionTable->item(i, 1)->setText(tr("%1").arg(markovResult[i].directReward));
        actionTable->item(i, 2)->setText(tr("%1").arg(markovResult[i].potentialReward));
        actionTable->item(i, 3)->setText(tr("%1").arg(markovResult[i].action.dc));
        actionTable->item(i, 4)->setText(tr("%1").arg(markovResult[i].action.dt));
        actionTable->item(i, 5)->setText(tr("%1").arg(markovResult[i].successProbility));
        actionTable->item(i, 6)->setText(tr("%1").arg(markovResult[i].action.toString()));
        if (markovActionId == -1 || markovResult[markovActionId].potentialReward
                < markovResult[i].potentialReward)
        {
            markovActionId = i;
        }
        if (greedyActionId == -1 || markovResult[greedyActionId].directReward
                < markovResult[i].directReward)
        {
            greedyActionId = i;
        }
    }
    if (markovActionId == -1)
    {
        markovActionLabel->setText(tr("MDP无动作"));
    }
    else
    {
        markovActionLabel->setText(tr("MDP最优动作: %1")
                                   .arg(markovResult[markovActionId].action.name()));
    }
    if (greedyActionId == -1)
    {
        greedyActionLabel->setText(tr("Greedy无动作"));
    }
    else
    {
        greedyActionLabel->setText(tr("Greedy最优动作: %1")
                                   .arg(markovResult[greedyActionId].action.name()));
    }

    actionWidgetMutex.unlock();
}


void WebServiceActionWidget::setMarkovResult(QList<MarkovResultItem>& _markovResult)
{
    markovResult = _markovResult;
    emit updateActionsSignal();
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
