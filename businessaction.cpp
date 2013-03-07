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
        res = "资源替换";
    }
    else if (type == BusinessAction::RESOURCE_TRANSPORT)
    {
        res = "资源转移";
    }
    else if (type == BusinessAction::RESOURCE_TERMINATE_NEED)
    {
        res = "终止需求";
    }
    else if (type == BusinessAction::RESOURCE_DO_NOTHING)
    {
        res = "不作为";
    }
    else if (type == BusinessAction::RESOURCE_ADD_NEW_NEED)
    {
        res = "引入 m+k 个新需求";
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
        res = QString(("在流程 %1 活动 %2 上, 把资源 %3 替换为 %4"))
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
        res = QString(("从流程 %1 上活动 %2 上，把资源 %3 迁移到流程 %4 的活动 %5 上"))
                .arg(otherFlowId)
                .arg(activitiesString)
                .arg(resourceString)
                .arg(event->n)
                .arg(event->a);
    }
    else if (type == BusinessAction::RESOURCE_TERMINATE_NEED && isActive)
    {
        res = QString(("流程 %1 终止需求")).arg(event->n);
    }
    else if (type == BusinessAction::RESOURCE_DO_NOTHING && isActive)
    {
        res = QString(("不作为, 流程 %1 的新需求将得不到满足或者资源浪费")).arg(event->n);
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
    res << "资源替换" << "资源转移" << "不作为" << "终止需求" << "引入 m+k 个新需求";
    return res;
}
