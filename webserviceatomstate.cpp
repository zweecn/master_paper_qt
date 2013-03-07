#include "webserviceatomstate.h"
#include "webservicestate.h"
#include "webserviceaction.h"
#include "workflow.h"

WebServiceAtomState::WebServiceAtomState()
{
    id = 0;
}
WebServiceAtomState::WebServiceAtomState(const WebServiceAtomState & other)
{
    id = other.id;
    activityId = other.activityId;
    stateType = other.stateType;
}

bool WebServiceAtomState::operator ==(const WebServiceAtomState & other) const
{
    return (activityId == other.activityId && stateType == other.stateType);
}

int WebServiceAtomState::getId()
{
    id = activityId * STATE_SIZE + stateType;
    return id;
}

void WebServiceAtomState::setId(int _id)
{
    id = _id;
}

void WebServiceAtomState::setActivityId(int _activityId)
{
    activityId = _activityId;
}

void WebServiceAtomState::setStateType(int _stateType)
{
    stateType = _stateType;
}

QString WebServiceAtomState::name()
{
    QString res;
    if (stateType == READY_N)
    {
        res += "准备(N)";
    }
    else if (stateType == READY_U)
    {
        res += "准备(U)";
    }
    else if ( stateType == FAIL)
    {
        res += "失败";
    }
    else if ( stateType == FINISH_U)
    {
        res += "完成(U)";
    }
    else if ( stateType == FINISH_N)
    {
        res += "完成(N)";
    }
    else if ( stateType == STOP)
    {
        res += "终止";
    }
    else
    {
        res += "非状态";
    }
    return res;
}

QString WebServiceAtomState::toString()
{
    QString res(QString("状态: [A=%1 %2]").arg(activityId).arg(name()));
    return res;
}
