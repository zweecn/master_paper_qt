#ifndef WEBSERVICESIMULATION_H
#define WEBSERVICESIMULATION_H
#include <QList>

#include "webserviceatomstate.h"
#include "webserviceaction.h"
#include "webserviceflow.h"

class WebServiceSimulation
{
public:
    WebServiceSimulation();
    ~WebServiceSimulation();

    void run();

private:
    bool clearData();
    bool init();
    bool initUtility();
    bool createStateTransTable();
    bool runMarkov();

    void noNeedDo(WebServiceAtomState & s);
    WebServiceAction noNeedDo();

    void recomposite(WebServiceAtomState & s);
    WebServiceAction recomposite(int activityId);

    void retry(WebServiceAtomState & s);
    WebServiceAction retry(int activityId);

    void replace(WebServiceAtomState & s);
    WebServiceAction replace(int activityId);

    void doNothing(WebServiceAtomState & s);
    WebServiceAction doNothing();

    void terminate(WebServiceAtomState & s);
    WebServiceAction terminate();

    WebServiceAction* getAction(WebServiceAction & action);
    QList<ReplaceNode> recomposeFreeService(int activityId);
    AtomService* nextFreeService(int activityId);

    QList<WebServiceAtomState> stateList;
    QList<WebServiceAction> actionList;
    WebServiceFlow wsf;

    int ** stateAction;
    int ** actionState;
    int ** posibility;

    enum
    {
        MIN_POSIBILITY = 0,
        MAX_POSIBILITY = 100
    };

    double * actionUtility;
    double * stateUtility;

    int maxStateSize;
    int maxActionSize;
};

#endif // WEBSERVICESIMULATION_H
