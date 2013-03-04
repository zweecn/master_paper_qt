#ifndef LAYERMARKOVBACKWARD_H
#define LAYERMARKOVBACKWARD_H
#include "markovstate.h"
#include "markovrecord.h"

#include <QQueue>
#include <QString>
#include <QList>
#include <QMap>
#include <QHash>
#include <QSet>
#include <qmath.h>

class TAndState
{
public:
    TAndState(int t, MarkovState & _state)
    {
        t = t;
        state = _state;
    }
    int getT() const
    {
        return t;
    }
    MarkovState & getState()
    {
        return state;
    }
    bool operator ==(const TAndState & other) const
    {
        return (t == other.t && state == other.state);
    }
    bool operator <(const TAndState & other) const
    {
        return (t < other.t && state < other.state);
    }

    int t;
    MarkovState state;
};

class StateTAndAction
{
public:
    StateTAndAction(MarkovState & _state, int _t, MarkovAction & _action)
    {
        state = _state;
        t = _t;
        action = _action;
    }
    MarkovState & getState()
    {
        return state;
    }
    int  getT() const
    {
        return t;
    }
    MarkovAction & getAction()
    {
        return action;
    }
    bool operator ==(const StateTAndAction & other) const
    {
        return (t == other.t && state == other.state && action == other.action);
    }
    bool operator <(const StateTAndAction & other) const
    {
        return (t < other.t && state < other.state && action < other.action);
    }


    MarkovState state;
    int t;
    MarkovAction action;
};

class ToStateInfo
{
public:
    ToStateInfo(MarkovState & _state, double _posibility, double _price, double _time)
    {
        state = _state;
        posibility = _posibility;
        price = _price;
        time = _time;
    }

    MarkovState & getState()
    {
        return state;
    }
    double getPosibility() const
    {
        return posibility;
    }
    double getPrice() const
    {
        return price;
    }
    double getTime() const
    {
        return time;
    }
    bool operator ==(const ToStateInfo & other) const
    {
        double e = 0.0000001;
        return (fabs(time - other.time) < e
                && fabs(price - other.price) < e
                && fabs(posibility - other.posibility) < e
                && state == other.state);
    }
    bool operator <(const ToStateInfo & other) const
    {
        return (time < other.time
                && price < other.price
                && posibility < other.posibility
                && state < other.state);
    }

    MarkovState state;
    double posibility;
    double price;
    double time;
};

extern inline uint qHash(const TAndState & key);
extern inline uint qHash(const StateTAndAction & key);
extern inline uint qHash(const ToStateInfo & key);

class LayerMarkovBackward
{
public:
    LayerMarkovBackward(MarkovState& _state);

    void init(int reduceLayerSize);
    void init();
    void initGreedy();
    void initMap();
    void generateLayerRecords();
    void extendTree(bool isExtend);
    void reduceLayer(int i);
    void addToRecords(QList<MarkovRecord> &destRecords, QList<MarkovRecord> &sourceRecord);
    void addToMap(int t, QList<MarkovRecord> & records);
    QString makeStepString(int t, MarkovAction & action, double u);
    int getTsize();
    bool hasChildren(int t, MarkovState & state);

    void initMarkovInfo();
    void runMarkov();
    double maxUtility(int t, MarkovState & i);
    double getNReward(int t, MarkovState & state);
    double getTReward(StateTAndAction & sta, QList<ToStateInfo> & tsi);

    double getMarkovBestUtility();
    double getCurrActionCost();
    double getCurrActionTimeCost();
    double getCurrActionReward();
    MarkovAction & getAction();
    MarkovState & getStateNew();


    // Next is greedy

    double getGreedyActionReward();
    MarkovState & getGreedyStateNew();
    double getGreedyPosibility();
    double getGreedyTimeCost();
    double getGreedyPriceCost();
    MarkovAction & getGreedyAction();
    void runGreedy();

    MarkovState state;
    MarkovState greedyState;

    QQueue<MarkovState> queue1;
    QQueue<MarkovState> queue2;
    QList<QList<MarkovRecord> > allLayerRecords;

    QHash<int, QSet<MarkovState> > t2StateMap;
    QHash<TAndState, QList<MarkovAction> > tState2ChildActionMap;
    QHash<StateTAndAction, QList<ToStateInfo> > stateTAction2ChildStateInfoMap;

    enum
    {
        IS_EXTEND_TREE = 1,
        REDUCE_LAYER_SIZE = 2,
        WEAKEN = 1,
        PUNISHMENT_FAILED = 100,
        PUNISHMENT_PER_SECOND = 1

    };
private:
    double** utility;
    QString* step;
    double actionCost;
    double actionTimeCost;

    MarkovAction firstAction;
    MarkovState stateNew;

    long generateRecordRunTime;
    long initMarkovInfoRunTime;
    long runMarkovRunTime;

    // Next is greedy
    MarkovAction greedyAction;
    double greedyPriceCost;
    double greedyTimeCost;
    double greedyPosibility;
    double greedyReward;
    MarkovState greedyStateNew;
};

#endif // LAYERMARKOVBACKWARD_H
