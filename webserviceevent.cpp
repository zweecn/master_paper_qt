#include "webserviceevent.h"
#include "workflow.h"
#include "webserviceatomstate.h"
#include "activity.h"

#include <cstdlib>
#include <ctime>

WebServiceEvent::WebServiceEvent()
{
    type = NOT_EVENT;
}

bool WebServiceEvent::operator ==(const WebServiceEvent & other) const
{
    return t == other.t && type == other.type && a == other.a;
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

WebServiceEvent WebServiceEvent::proRandom(int currTime, QSet<int>& runningActivities,
                                        QSet<int>& finishedActivities, Activity* firstActivity)
{
    double sp = 1;
    QSetIterator<int> it(runningActivities);
    while (it.hasNext())
    {
        int aId = it.next();
        sp *= firstActivity[aId].blindService->reliability;
    }
    srand(time(NULL));
    WebServiceEvent event;
    event.t = currTime;
    event.a = rand() % WorkFlow::Instance()->getActivitySize();
    event.type = WebServiceEvent::NOT_EVENT;
    if (rand() > sp)
    {
        if (runningActivities.contains(event.a))
        {
            event.type =  WebServiceAtomState::FAIL;
        }
        else if (finishedActivities.contains(event.a))
        {
            event.type = WebServiceAtomState::FINISH_U;
        }
        else
        {
            event.type =  WebServiceAtomState::READY_U;
        }
    }
    else
    {
        if (runningActivities.contains(event.a))
        {
            event.type =  WebServiceAtomState::FINISH_N;
        }
        else if (finishedActivities.contains(event.a))
        {
            event.type = WebServiceAtomState::FINISH_N;
        }
        else
        {
            event.type =  WebServiceAtomState::READY_N;
        }
    }
    return event;
}

QString WebServiceEvent::name()
{
    QString res;
    if (type == WebServiceAtomState::READY_N)
    {
        res += "准备(N)";
    }
    else if (type == WebServiceAtomState::READY_U)
    {
        res += "准备(U)";
    }
    else if ( type == WebServiceAtomState::FAIL)
    {
        res += "失败";
    }
    else if ( type == WebServiceAtomState::FINISH_U)
    {
        res += "完成(U)";
    }
    else if ( type == WebServiceAtomState::FINISH_N)
    {
        res += "完成(N)";
    }
    else if ( type == WebServiceAtomState::STOP)
    {
        res += "停止";
    }
    else
    {
        res += "非事件";
    }
    return res;
}

QString WebServiceEvent::toString()
{
    QString res(QString("事件: [T%1 A%2 %3]").arg(t).arg(a).arg(name()));
    return res;
}
