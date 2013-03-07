#include "webserviceaction.h"

#include <QDebug>

WebServiceAction::WebServiceAction()
{
    id = -1;
    dc = dt = 0;
}

//WebServiceAction::WebServiceAction(int _id, int _type)
//{
//    id = _id;
//    type = _type;
//    dc = dt = 0;
//}

WebServiceAction::WebServiceAction(const WebServiceAction &other)
{
    id = other.id;
    activityId = other.activityId;
    type = other.type;
    dc = other.dc;
    dt = other.dt;
    replaceList = other.replaceList;
}

WebServiceAction & WebServiceAction::operator =(const WebServiceAction & other)
{
    if (this == &other)
    {
        return *this;
    }
    id = other.id;
    activityId = other.activityId;
    type = other.type;
    dc = other.dc;
    dt = other.dt;
    replaceList = other.replaceList;

    return *this;
}

bool WebServiceAction::operator ==(const WebServiceAction & other) const
{
    return (activityId == other.activityId && type == other.type && replaceList == other.replaceList);
}

int WebServiceAction::getId()
{
    return id;
}

QString WebServiceAction::name()
{
    QString res;
    if (type == NO_NEED_DO)
    {
        res += "�������";
    }
    else if ( type == TERMINATE)
    {
        res += "��ֹ";
    }
    else if ( type == DO_NOTHING)
    {
        res += "����Ϊ";
    }
    else if ( type == REPLACE)
    {
        res += "�滻";
    }
    else if ( type == RE_COMPOSE)
    {
        res += "����";
    }
    else if ( type == RETRY)
    {
        res += "����";
    }
    else
    {
        res += "�Ƕ���";
    }
    return res;
}

QString WebServiceAction::toString()
{
    QString res(QString("����: [id=%1 %2").arg(getId()).arg(name()));

    QListIterator<ReplaceNode> it(replaceList);
    while (it.hasNext())
    {
        const ReplaceNode & node = it.next();
        res += QString(" A%1:S%2->S%3 ")
                        .arg(node.activityId)
                        .arg(node.oldServiceId)
                        .arg(node.newServiceId);
    }
    res += QString(" dc=%1 dt=%2]").arg(dc).arg(dt);

    return res;
}

double WebServiceAction::getSucceedProbolity()
{

    if (type == NO_NEED_DO)
    {

    }
    else if ( type == TERMINATE)
    {

    }
    else if ( type == DO_NOTHING)
    {

    }
    else if ( type == REPLACE)
    {

    }
    else if ( type == RE_COMPOSE)
    {

    }
    else if ( type == RETRY)
    {

    }
    else
    {

    }
    return 0;
}
