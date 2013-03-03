#ifndef MARKOVSTATE_H
#define MARKOVSTATE_H
#include "activity.h"
#include "webserviceflow.h"

#include <QList>

class MarkovState : public WebServiceFlow
{
public:
    MarkovState();
    bool init();

    bool isPrefixActivitiesFinished(int activityId);
    bool isFailed();
    bool isFinished();
    void setFailed(bool failed);
    MarkovState* nextSecond();
    QList<MarkovState> getNextTwoStates();
    MarkovState & operator = (const MarkovState & other);
    bool operator ==(const MarkovState & other) const;
    bool operator <(const MarkovState & other) const;

    enum
    {
        S_NORMAL = 0,
        S_FAILED = 1,
        S_SUCCEED = 2,
        S_PRICE_UP = 3,
        S_DELAYED = 4
    };

    bool finished;
    int id;
    int globalState;

    QList<MarkovState> nextStateArray;
    QList<Activity*> nextToDoActivityList;
    Activity* nextToDoActivity;
    Activity* faultActivity;

};

#endif // MARKOVSTATE_H
