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
        res += "׼��(N)";
    }
    else if (stateType == READY_U)
    {
        res += "׼��(U)";
    }
    else if ( stateType == FAIL)
    {
        res += "ʧ��";
    }
    else if ( stateType == FINISH_U)
    {
        res += "���(U)";
    }
    else if ( stateType == FINISH_N)
    {
        res += "���(N)";
    }
    else if ( stateType == STOP)
    {
        res += "��ֹ";
    }
    else
    {
        res += "��״̬";
    }
    return res;
}

QString WebServiceAtomState::toString()
{
    QString res(QString("״̬: [A=%1 %2]").arg(activityId).arg(name()));
    return res;
}
