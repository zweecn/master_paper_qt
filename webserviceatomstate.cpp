#include "webserviceatomstate.h"
#include "webservicestate.h"
#include "webserviceaction.h"
#include "workflow.h"

WebServiceAtomState::WebServiceAtomState()
{
}

bool WebServiceAtomState::operator ==(const WebServiceAtomState & other) const
{
    return (activityId == other.activityId && stateType == other.stateType);
}

int WebServiceAtomState::hash()
{
    int res = activityId * STATE_SIZE + stateType;
    return res;
}

QString WebServiceAtomState::toString()
{
    QString res(QString("State [%1 ").arg(activityId));
    if (stateType == READY_N)
    {
        res += "READY_N";
    }
    else if (stateType == READY_U)
    {
        res += "READY_U";
    }
    else if ( stateType == FAIL)
    {
        res += "FAIL";
    }
    else if ( stateType == FINISH_U)
    {
        res += "FINISH_U";
    }
    else if ( stateType == FINISH_N)
    {
        res += "FINISH_N";
    }
    else if ( stateType == STOP)
    {
        res += "STOP";
    }
    else
    {
        res += "NOT_STATE";
    }
    res += "]";
    return res;
}
