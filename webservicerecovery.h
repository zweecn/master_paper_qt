#ifndef WEBSERVICERECOVERY_H
#define WEBSERVICERECOVERY_H

#include <QList>
#include <cmath>

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
        directReward = other.directReward;
        suffixState = other.suffixState;
        suffixPosibility = other.suffixPosibility;
        successProbility = other.successProbility;
    }

    MarkovResultItem& operator =(const MarkovResultItem &other)
    {
        if (this == &other)
            return *this;
        action = other.action;
        potentialReward = other.potentialReward;
        directReward = other.directReward;
        suffixState = other.suffixState;
        suffixPosibility = other.suffixPosibility;
        successProbility = other.successProbility;
        return *this;
    }

    bool operator ==(const MarkovResultItem &other) const
    {
        double e = 0.00001;
        if (!(action == other.action))
            return false;
        if (fabs(directReward - other.directReward) > e
                || fabs(potentialReward - other.potentialReward) > e
                || fabs(successProbility - other.successProbility) > e)
            return false;
        if (!(suffixState == other.suffixState))
            return false;
        if (suffixPosibility.size() != other.suffixPosibility.size())
            return false;
        for (int i = 0; i < suffixPosibility.size(); i++)
        {
            if (fabs(suffixPosibility[i] - other.suffixPosibility[i]) > e)
                return false;
        }
        return true;
    }

    WebServiceAction action;
    double directReward;
    double potentialReward;
    double successProbility;
    QList<WebServiceAtomState> suffixState;
    QList<double> suffixPosibility;

    QString toString()
    {
        QString res(QString("<p>%1</p>").arg(action.toString()));
        res += QString("<p>潜在收益:%1   成功概率:%2</p>")
                .arg(potentialReward).arg(successProbility);
        res += QString("<p>后续可能:</p><p>");
        for (int i = 0; i < suffixState.size(); i++)
        {
            res += (suffixState[i].toString().append(QString(",p=%1 ").arg(suffixPosibility[i])));
        }
        res += "</p>";
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
    bool recovery(WebServiceAction * action, int x);

    void runTest();
    void printStateAction();
    void printActionState();
    void printPosibility();
    void printResult();
    void printFirstStateResult();

    enum
    {
        MIN_POSIBILITY = 0,
        MAX_POSIBILITY = 100
    };

private:
    bool clearData();
    bool init();
    bool initUtility();
    bool createStateTransTable();
    bool runMarkov();
    QList<MarkovResultItem> doMarkovResult(WebServiceAtomState &state);

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
    QList<MarkovResultItem> markovResult;
    WebServiceFlow wsf;

    int ** stateAction;
    int ** actionState;
    int ** posibility;

    double * actionUtility;
    double * stateUtility;

    int maxStateSize;
    int maxActionSize;

    time_t markovRuntime;
    time_t greedyRuntime;
};

#endif // WEBSERVICERECOVERY_H
