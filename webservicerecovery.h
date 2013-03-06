#ifndef WEBSERVICERECOVERY_H
#define WEBSERVICERECOVERY_H

#include <QList>
#include "webserviceatomstate.h"
#include "webserviceaction.h"
#include "webserviceflow.h"

class MarkovResultItem
{
public:
    MarkovResultItem(){}
    MarkovResultItem(const MarkovResultItem &other)
    {
        action = other.action;
        potentialReward = other.potentialReward;
        suffixState = other.suffixState;
        suffixPosibility = other.suffixPosibility;
    }

    MarkovResultItem& operator =(const MarkovResultItem &other)
    {
        if (this == &other)
            return *this;
        action = other.action;
        potentialReward = other.potentialReward;
        suffixState = other.suffixState;
        suffixPosibility = other.suffixPosibility;
        return *this;
    }

    WebServiceAction action;
    double potentialReward;
    double successProbility;
    QList<WebServiceAtomState> suffixState;
    QList<double> suffixPosibility;

    QString toString()
    {
        QString res = action.toString();
        res += QString(" PotentialReward:%1 SuccessProbility:%2")
                .arg(potentialReward).arg(successProbility);
        res += QString(" Suffix:[");
        for (int i = 0; i < suffixState.size(); i++)
        {
            res += (suffixState[i].toString().append(QString(",%1 ").arg(suffixPosibility[i])));
        }
        res += "]";
        return res;
    }
};

class WebServiceRecovery
{
public:
    WebServiceRecovery();
    ~WebServiceRecovery();

    QList<MarkovResultItem> getMarkovResult(WebServiceAtomState &state);
    WebServiceAction getGreedyResult(WebServiceAtomState &state);

    time_t getMarkovRuntime();
    time_t getGreedyRuntime();
    double reward(int dc, int dt, double p);

    WebServiceFlow& getWebServiceFlow();
    bool recovery(WebServiceAction * action);

    void runTest();
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
    QList<MarkovResultItem> doMarkovResult(WebServiceAtomState &state);
    QList<MarkovResultItem>& doIfLastActivityError(QList<MarkovResultItem>& res,
                                                   WebServiceAtomState &state);
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

    time_t markovRuntime;
    time_t greedyRuntime;
};

#endif // WEBSERVICERECOVERY_H
