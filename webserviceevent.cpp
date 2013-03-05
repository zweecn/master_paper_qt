#include "webserviceevent.h"
#include "workflow.h"
#include "markovstate.h"

#include <cstdlib>
#include <ctime>

WebServiceEvent::WebServiceEvent()
{
}

WebServiceEvent WebServiceEvent::random(int currTime)
{
    srand(time(NULL));
    WebServiceEvent event;
    event.t = currTime;
    event.a = rand() % WorkFlow::Instance()->getActivitySize();
    event.type = rand() % MarkovState::STATE_COUNT;

    return event;
}

QString WebServiceEvent::name()
{
    QString res;
    switch (type)
    {
    case MarkovState::S_NORMAL:
        res = "S_NORMAL";
        break;
    case MarkovState::S_FAILED:
        res = "S_FAILED";
        break;
    case MarkovState::S_SUCCEED:
        res = "S_SUCCEED";
        break;
    case MarkovState::S_PRICE_UP:
        res = "S_PRICE_UP";
        break;
    case MarkovState::S_DELAYED:
        res = "S_DELAYED";
    default:
        res = "NOT_EVENT";
    }
    return res;
}

QString WebServiceEvent::toString()
{
    QString res(QString("Event: [%1 %2 %3]").arg(t).arg(a).arg(name()));
    return res;
}
