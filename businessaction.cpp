#include "businessaction.h"
#include "businessevent.h"
#include "resource.h"

#include <QString>

BusinessAction::BusinessAction()
{
    type = 0;
    reward = 0;

    bug = 0;
    newResource = 0;

    event = NULL;
}

BusinessAction::BusinessAction(const BusinessAction& other)
{
    type = other.type;
    reward = other.reward;
    event = other.event;

    bug = other.bug;
    oldResource = other.oldResource;
    newResource = other.newResource;

    otherFlowId = other.otherFlowId;
    otherFlowResource = other.otherFlowResource;
    otherFlowActivities = other.otherFlowActivities;

    isActive = other.isActive;
}

BusinessAction& BusinessAction::operator ==(const BusinessAction & other)
{
    if (this == &other)
        return *this;
    type = other.type;
    reward = other.reward;
    event = other.event;

    bug = other.bug;
    oldResource = other.oldResource;
    newResource = other.newResource;

    otherFlowId = other.otherFlowId;
    otherFlowResource = other.otherFlowResource;
    otherFlowActivities = other.otherFlowActivities;

    isActive = other.isActive;

    return *this;
}

bool BusinessAction::operator ==(const BusinessAction& other) const
{
    return (type == other.type
            && reward == other.reward
            && event == other.event

            && bug == other.bug
            && oldResource == other.oldResource
            && newResource == other.newResource

            && otherFlowId == other.otherFlowId
            && otherFlowResource == other.otherFlowResource
            && otherFlowActivities == other.otherFlowActivities
            && isActive == other.isActive);
}

QString BusinessAction::name()
{
    QString res;
    if (type == BusinessAction::RESOURCE_REPLACE)
    {
        res = "��Դ�滻";
    }
    else if (type == BusinessAction::RESOURCE_TRANSPORT)
    {
        res = "��Դת��";
    }
    else if (type == BusinessAction::RESOURCE_TERMINATE_NEED)
    {
        res = "��ֹ����";
    }
    else if (type == BusinessAction::RESOURCE_DO_NOTHING)
    {
        res = "����Ϊ";
    }
    else if (type == BusinessAction::RESOURCE_ADD_NEW_NEED)
    {
        res = "���� m+k ��������";
    }
    return res;
}

QString BusinessAction::toString()
{
    QString res;
    if (type == BusinessAction::RESOURCE_REPLACE
            && isActive && oldResource != NULL
            && newResource != NULL && event != NULL)
    {
        res = QString(("������ %1 � %2 ��, ����Դ %3 �滻Ϊ %4"))
                .arg(event->n)
                .arg(event->a)
                .arg(oldResource->id)
                .arg(newResource->id);
    }
    else if (type == BusinessAction::RESOURCE_TRANSPORT
            && reward != -INT_MAX && isActive && event != NULL)
    {
        QString activitiesString;
        QString resourceString;
        for (int i = 0; i < otherFlowActivities.size(); i++)
        {
            activitiesString.append(QString(" %1").arg(otherFlowActivities[i]));
        }
        for (int i = 0; i < otherFlowResource.size(); i++)
        {
            resourceString.append(QString(" %1").arg(otherFlowResource[i]->id));
        }
        res = QString(("������ %1 �ϻ %2 �ϣ�����Դ %3 Ǩ�Ƶ����� %4 �Ļ %5 ��"))
                .arg(otherFlowId)
                .arg(activitiesString)
                .arg(resourceString)
                .arg(event->n)
                .arg(event->a);
    }
    else if (type == BusinessAction::RESOURCE_TERMINATE_NEED && isActive)
    {
        res = QString(("���� %1 ��ֹ����")).arg(event->n);
    }
    else if (type == BusinessAction::RESOURCE_DO_NOTHING && isActive)
    {
        res = QString(("����Ϊ, ���� %1 �������󽫵ò������������Դ�˷�")).arg(event->n);
    }
    else
    {
        res = "";
    }
    return res;
}

QList<QString> BusinessAction::nameList()
{
    QList<QString> res;
    res << "��Դ�滻" << "��Դת��" << "����Ϊ" << "��ֹ����" << "���� m+k ��������";
    return res;
}
