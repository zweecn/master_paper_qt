#include "webserviceevent.h"
#include "workflow.h"
#include "webserviceatomstate.h"

#include <cstdlib>
#include <ctime>

WebServiceEvent::WebServiceEvent()
{
}

WebServiceEvent WebServiceEvent::random(int currTime, QSet<int>& runningActivities,
                                        QSet<int>& finishedActivities)
{
    srand(time(NULL));
    WebServiceEvent event;
    event.t = currTime;
    event.a = rand() % WorkFlow::Instance()->getActivitySize();
    if (runningActivities.contains(event.a))
    {
        event.type =  WebServiceAtomState::FAIL;
    }
    else if (finishedActivities.contains(event.a))
    {
        if (rand() %2)
            event.type = WebServiceAtomState::FINISH_N;
        else
            event.type = WebServiceAtomState::FINISH_U;
    }
    else
    {
        if (rand() %2)
            event.type =  WebServiceAtomState::READY_N;
        else
            event.type =  WebServiceAtomState::READY_U;
    }
    return event;
}

QString WebServiceEvent::name()
{
    QString res;
    if (type == WebServiceAtomState::READY_N)
    {
        res += "READY_N";
    }
    else if (type == WebServiceAtomState::READY_U)
    {
        res += "READY_U";
    }
    else if ( type == WebServiceAtomState::FAIL)
    {
        res += "FAIL";
    }
    else if ( type == WebServiceAtomState::FINISH_U)
    {
        res += "FINISH_U";
    }
    else if ( type == WebServiceAtomState::FINISH_N)
    {
        res += "FINISH_N";
    }
    else if ( type == WebServiceAtomState::STOP)
    {
        res += "STOP";
    }
    else
    {
        res += "NOT_STATE";
    }
    return res;
}

QString WebServiceEvent::toString()
{
    QString res(QString("Event: [T%1 A%2 %3]").arg(t).arg(a).arg(name()));
    return res;
}
