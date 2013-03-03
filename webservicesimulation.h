#ifndef WEBSERVICESIMULATION_H
#define WEBSERVICESIMULATION_H
#include "webserviceaction.h"
#include "webservicestate.h"
#include <QList>

//class WebServiceState;
//class WebServiceAction;
//class Activity;

class TActionToState
{
public:
    int actionId;
    int stateId;
    double tp;
    int dt;
    double dc;
};

//class UTG
//{
//public:
//    WSState rootState;
//    QList<WSState> leafState;
//    QList<int> actions;
//    QList<TActionToState> tas;
//};

//struct WebServiceActionResultNode
//{
//    WebServiceActionResultNode(const WebServiceState & _state)
//    {
//        memset(p, 0, sizeof(double) * WebServiceState::STATE_COUNT);
//        for (int i = 0; i < WebServiceState::STATE_COUNT; i++)
//        {
//            states[i] = _state;
//        }
//    }

//    WebServiceState states[WebServiceState::STATE_COUNT];
//    double p[WebServiceState::STATE_COUNT];
//};


class WebServiceSimulation
{
public:
    WebServiceSimulation();
    ~WebServiceSimulation();

    void run();
private:
    bool init();

    bool markov();

//    WebServiceActionResultNode terminate();
//    WebServiceActionResultNode doNothing();
//    WebServiceActionResultNode retry();
//    WebServiceActionResultNode substitute();
//    WebServiceActionResultNode recompose();

//    WebServiceState currState;
//    WebServiceAction *actions;

//    QList<WebServiceAction> markovActionList;

//    Activity *firstActivity;
//    Activity *bugActivity;

//    qint64 stateSize;
};

#endif // WEBSERVICESIMULATION_H
