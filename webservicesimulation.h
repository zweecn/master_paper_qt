#ifndef WEBSERVICESIMULATION_H
#define WEBSERVICESIMULATION_H
#include <QList>

#include "webserviceatomstate.h"
#include "webserviceaction.h"
#include "webserviceflow.h"

class MarkovResultItem
{
public:
    WebServiceAction action;
    double potentialReward;
    QList<WebServiceAtomState> suffixState;
    QList<double> suffixPosibility;

    QString toString()
    {
        QString res = action.toString();
        res += QString(" PotentialReward:%1").arg(potentialReward);
        res += QString(" Suffix:[");
        for (int i = 0; i < suffixState.size(); i++)
        {
            res += (suffixState[i].toString().append(QString(",%1 ").arg(suffixPosibility[i])));
        }
        res += "]";
        return res;
    }
};

class WebServiceSimulation
{
public:
    WebServiceSimulation();
    ~WebServiceSimulation();

    void getResult();

    QList<MarkovResultItem> getResult(WebServiceAtomState &state);

    void printStateAction();
    void printActionState();
    void printPosibility();
    void printResult();
    void printFirstStateResult();
private:
    bool clearData();
    bool init();
    bool initUtility();
    bool createStateTransTable();
    bool runMarkov();

    bool isActionStateHasTrue();

    void noNeedDo(WebServiceAtomState & s);
    WebServiceAction noNeedDo(int activityId);

    void recomposite(WebServiceAtomState & s);
    WebServiceAction recomposite(int activityId);

    void retry(WebServiceAtomState & s);
    WebServiceAction retry(int activityId);

    void replace(WebServiceAtomState & s);
    WebServiceAction replace(int activityId);

    void doNothing(WebServiceAtomState & s);
    WebServiceAction doNothing(int activityId);

    void terminate(WebServiceAtomState & s);
    WebServiceAction terminate(int activityId);

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
