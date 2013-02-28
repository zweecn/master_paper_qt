#include "businessactionwidget.h"
#include "businessaction.h"
#include "resource.h"
#include "businessevent.h"

#include <QtGui>
#include <QDebug>

BusinessActionWidget::BusinessActionWidget(QWidget *parent) :
    QWidget(parent)
{
    createActionTable();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(actionTable);
    mainLayout->addWidget(autoActionLabel);
    setLayout(mainLayout);

    setWindowTitle(tr("业务流程不确定事件动作"));
    resize(400, 300);

    connect(this, SIGNAL(updateActionsSignal()), this, SLOT(updateActionTable()));
}

void BusinessActionWidget::createActionTable()
{
    actionTable = new QTableWidget();
    actionTable->setColumnCount(3);
    actionTable->setRowCount(BusinessAction::ACTIONS_COUNT);
    QStringList header;
    header << "动作" << "收益" << "详细说明";
    actionTable->setHorizontalHeaderLabels(header);
    actionTable->horizontalHeader()->setStretchLastSection(true);
    actionTable->verticalHeader()->setHidden(true);
    actionTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    actionTable->setEditTriggers (QAbstractItemView::NoEditTriggers);
    actionTable->setSelectionMode (QAbstractItemView::SingleSelection);
    actionTable->setSortingEnabled(false);
    for (int i = 0; i < BusinessAction::ACTIONS_COUNT; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            actionTable->setItem(i, j, new QTableWidgetItem());
            actionTable->item(i, j)->setTextAlignment(Qt::AlignCenter);
        }
    }
    QList<QString> nameList = BusinessAction::nameList();
    for (int i = 0; i < nameList.size(); i++)
    {
        actionTable->item(i, 0)->setText(nameList[i]);
    }

    autoActionLabel = new QLabel(tr("无动作"));
    autoAction = NULL;
}

void BusinessActionWidget::updateActionTable()
{
//    nextStepMutex.lock();

    for (int i = 0; i < BusinessAction::ACTIONS_COUNT; i++)
    {
        if (actions[i].isActive)
        {
            actionTable->item(i, 1)->setText(tr("%1").arg(actions[i].reward));
            actionTable->item(i, 2)->setText(tr("%1").arg(actions[i].toString()));
        }
        else
        {
            actionTable->item(i, 1)->setText(tr(""));
            actionTable->item(i, 2)->setText(tr(""));
        }
    }

    actionTable->item(BusinessAction::RESOURCE_ADD_NEW_NEED, 1)->setText(tr(""));
    actionTable->item(BusinessAction::RESOURCE_ADD_NEW_NEED, 2)->setText(tr(""));

    if (autoAction != NULL)
    {
        autoActionLabel->setText(tr("自动选择动作: %1").arg(autoAction->name()));
    }
    else
    {
        autoActionLabel->setText(tr("无动作"));
    }

//    nextStepMutex.unlock();

}

void BusinessActionWidget::setBusinessAction(BusinessAction *_actions)
{
    actions = _actions;
    autoAction = NULL;
    emit updateActionsSignal();
}

void BusinessActionWidget::setAutoBusinessAction(BusinessAction *_autoAction)
{
    autoAction = _autoAction;
    emit updateActionsSignal();
}

BusinessAction * BusinessActionWidget::getBusinessAction()
{
    return actions;
}

BusinessAction * BusinessActionWidget::getAutoBusinessAction()
{
    return autoAction;
}
