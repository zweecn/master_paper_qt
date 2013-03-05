#include "webserviceaction.h"

WebServiceAction::WebServiceAction()
{
    id = -1;
    dc = dt = 0;
}

WebServiceAction::WebServiceAction(int _id, int _type)
{
    id = _id;
    type = _type;
    dc = dt = 0;
}

WebServiceAction::WebServiceAction(const WebServiceAction &other)
{
    id = other.id;
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
    type = other.type;
    dc = other.dc;
    dt = other.dt;
    replaceList = other.replaceList;

    return *this;
}

bool WebServiceAction::operator ==(const WebServiceAction & other) const
{
    return (type == other.type && replaceList == other.replaceList);
}

int WebServiceAction::hash()
{
    return id;
}

QString WebServiceAction::toString()
{
    QString res(QString("Action: [%1 ").arg(id));
    if (type == NO_NEED_DO)
    {
        res += "NO_NEED_DO";
    }
    else if ( type == TERMINATE)
    {
        res += "TERMINATE";
    }
    else if ( type == DO_NOTHING)
    {
        res += "DO_NOTHING";
    }
    else if ( type == REPLACE)
    {
        res += "REPLACE";
    }
    else if ( type == RE_COMPOSE)
    {
        res += "RE_COMPOSE";
    }
    else if ( type == RETRY)
    {
        res += "RETRY";
    }
    else
    {
        res += "NOT_ACTION";
    }

    res += QString(" dc=%1 dt=%2]").arg(dc).arg(dt);

    return res;
}
