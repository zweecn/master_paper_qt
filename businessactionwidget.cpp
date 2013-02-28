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

    setWindowTitle(tr("ҵ�����̲�ȷ���¼�����"));
    resize(400, 300);

    connect(this, SIGNAL(updateActionsSignal()), this, SLOT(updateActionTable()));
}

void BusinessActionWidget::createActionTable()
{
    actionTable = new QTableWidget();
    actionTable->setColumnCount(3);
    actionTable->setRowCount(BusinessAction::ACTIONS_COUNT);
    QStringList header;
    header << "����" << "����" << "��ϸ˵��";
    actionTable->setHorizontalHeaderLabels(header);
    actionTable->horizontalHeader()->setStretchLastSection(true);
    actionTable->verticalHeader()->setHidden(true);
    actionTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    actionTable->setEditTriggers (QAbstractItemView::NoEditTriggers);
    actionTable->setSelectionMode (QAbstractItemView::SingleSelection);
    actionTable->setSortingEnabled(true);
    for (int i = 0; i < BusinessAction::ACTIONS_COUNT; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            actionTable->setItem(i, j, new QTableWidgetItem());
            actionTable->item(i, j)->setTextAlignment(Qt::AlignCenter);
        }
    }
    autoActionLabel = new QLabel(tr("�Զ�ѡ����: "));
}

void BusinessActionWidget::updateActionTable()
{
//    actionTable->item(BusinessAction::RESOURCE_REPLACE, 0)->setText(tr("��Դ�滻"));
//    actionTable->item(BusinessAction::RESOURCE_TRANSPORT, 0)->setText(tr("��Դת��"));
//    actionTable->item(BusinessAction::RESOURCE_DO_NOTHING, 0)->setText(tr("����Ϊ"));
//    actionTable->item(BusinessAction::RESOURCE_TERMINATE_NEED, 0)->setText(tr("��ֹ����"));
//    actionTable->item(BusinessAction::RESOURCE_ADD_NEW_NEED, 0)->setText(tr("����������"));

    for (int i = 0; i < BusinessAction::ACTIONS_COUNT; i++)
    {
        actionTable->item(i, 0)->setText(tr("%1").arg(actions[i].name()));
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

//    if (actions[BusinessAction::RESOURCE_REPLACE].isActive == true
//            && actions[BusinessAction::RESOURCE_REPLACE].oldResource != NULL
//            && actions[BusinessAction::RESOURCE_REPLACE].newResource != NULL)
//    {
//        QString oldResource = QString("%1").arg(actions[BusinessAction::RESOURCE_REPLACE].oldResource->id);
//        QString newResource = QString("%1").arg(actions[BusinessAction::RESOURCE_REPLACE].newResource->id);
//        actionTable->item(BusinessAction::RESOURCE_REPLACE, 2)
//                ->setText(tr("������ %1 � %2 ��, ����Դ %3 �滻Ϊ %4")
//                          .arg(actions[BusinessAction::RESOURCE_REPLACE].event->n)
//                          .arg(actions[BusinessAction::RESOURCE_REPLACE].event->a)
//                          .arg(oldResource).arg(newResource));
//    }
//    else
//    {
//        actionTable->item(BusinessAction::RESOURCE_REPLACE, 1)->setText(tr(""));
//        actionTable->item(BusinessAction::RESOURCE_REPLACE, 2)->setText(tr(""));
//    }

//    if (actions[BusinessAction::RESOURCE_TRANSPORT].reward != -INT_MAX
//            && actions[BusinessAction::RESOURCE_TRANSPORT].isActive)
//    {
//        QString activitiesString;
//        QString resourceString;
//        for (int i = 0; i < actions[BusinessAction::RESOURCE_TRANSPORT].otherFlowActivities.size(); i++)
//        {
//            activitiesString.append(QString(" %1")
//                                    .arg(actions[BusinessAction::RESOURCE_TRANSPORT]
//                                         .otherFlowActivities[i]));
//        }
//        for (int i = 0; i < actions[BusinessAction::RESOURCE_TRANSPORT].otherFlowResource.size(); i++)
//        {
//            resourceString.append(QString(" %1")
//                                  .arg(actions[BusinessAction::RESOURCE_TRANSPORT]
//                                       .otherFlowResource[i]->id));
//        }
//        actionTable->item(BusinessAction::RESOURCE_TRANSPORT, 2)
//                ->setText(tr("������ %1 �ϻ %2 �ϣ�����Դ %3 Ǩ�Ƶ����� %4 �Ļ %5 ��")
//                          .arg(actions[BusinessAction::RESOURCE_TRANSPORT].otherFlowId)
//                          .arg(activitiesString)
//                          .arg(resourceString)
//                          .arg(actions[BusinessAction::RESOURCE_TRANSPORT].event->n)
//                          .arg(actions[BusinessAction::RESOURCE_TRANSPORT].event->a));
//    }
//    else
//    {
//        actionTable->item(BusinessAction::RESOURCE_TRANSPORT, 1)->setText(tr(""));
//        actionTable->item(BusinessAction::RESOURCE_TRANSPORT, 2)->setText(tr(""));
//    }

//    if (actions[BusinessAction::RESOURCE_TERMINATE_NEED].isActive)
//    {
//        actionTable->item(BusinessAction::RESOURCE_TERMINATE_NEED, 2)
//                ->setText(tr("���� %1 ��ֹ����")
//                          .arg(actions[BusinessAction::RESOURCE_TERMINATE_NEED].event->n));
//    }
//    else
//    {
//        actionTable->item(BusinessAction::RESOURCE_TERMINATE_NEED, 1)->setText(tr(""));
//        actionTable->item(BusinessAction::RESOURCE_TERMINATE_NEED, 2)->setText(tr(""));
//    }

//    if (actions[BusinessAction::RESOURCE_DO_NOTHING].isActive)
//    {
//        actionTable->item(BusinessAction::RESOURCE_DO_NOTHING, 2)
//                ->setText(tr("����Ϊ, ���� %1 �������󽫵ò������������Դ�˷�")
//                          .arg(actions[BusinessAction::RESOURCE_DO_NOTHING].event->n));
//    }
//    else
//    {
//        actionTable->item(BusinessAction::RESOURCE_DO_NOTHING, 1)->setText(tr(""));
//        actionTable->item(BusinessAction::RESOURCE_DO_NOTHING, 2)->setText(tr(""));
//    }

    actionTable->item(BusinessAction::RESOURCE_ADD_NEW_NEED, 1)->setText(tr(""));
    actionTable->item(BusinessAction::RESOURCE_ADD_NEW_NEED, 2)->setText(tr(""));

}

void BusinessActionWidget::setBusinessAction(BusinessAction *_actions)
{
    actions = _actions;
    emit updateActionsSignal();
}

void BusinessActionWidget::setAutoBusinessAction(BusinessAction *_autoAction)
{
    autoAction = _autoAction;
    autoActionLabel->setText(tr("�Զ�ѡ����: %1").arg(autoAction->name()));
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
