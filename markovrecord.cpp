#include "markovrecord.h"

#include <QHash>
#include <QDebug>
#include <cmath>

uint qHash(const StateAction & key)
{
    return qHash(key.state) ^ qHash(key.action);
}

uint qHash(const MarkovState & key)
{
    return qHash(key.finished) ^ qHash(key.globalState);
}

uint qHash(const MarkovAction & key)
{
    return qHash(key.bugActivityId) ^ qHash(key.type);
}

static QList<MarkovState> stateList;
static QList<MarkovAction> actionList;
static QSet<MarkovState> stateBeforeSet;
static QHash<StateAction, double> stateAction2PriceCostMap;
static QHash<StateAction, double> stateAction2TimeCostMap;
static int maxLayerSize;

MarkovRecord::MarkovRecord()
{
}

MarkovRecord::MarkovRecord(MarkovState & _stateBefore, MarkovState & _stateAfter, MarkovAction & _action,
                           double _posibility, double _priceCost, double _timeCost)
{
    stateBefore = _stateBefore;
    stateAfter = _stateAfter;
    action = _action;
    posibility = _posibility;
    priceCost = _priceCost;
    timeCost = _timeCost;

    init();
}

MarkovRecord & MarkovRecord::operator =(const MarkovRecord& other)
{
    if (this == &other)
    {
        return *this;
    }
    stateBefore = other.stateBefore;
    stateAfter = other.stateAfter;
    action = other.action;
    posibility = other.posibility;
    priceCost = other.priceCost;
    timeCost = other.timeCost;
    return *this;
}

bool MarkovRecord::init()
{
    addStateAction(stateBefore, action, priceCost, timeCost);
    stateBeforeSet.insert(stateBefore);
    return true;
}

void MarkovRecord::addStateAction(MarkovState & _state, MarkovAction & _action, double _price, double _time)
{
    StateAction sa(_state, _action);
    stateAction2PriceCostMap[sa] = _price;
    stateAction2TimeCostMap[sa] = _time;
}

void MarkovRecord::clear()
{
    stateList.clear();
    actionList.clear();
    stateBeforeSet.clear();
    stateAction2PriceCostMap.clear();
    stateAction2TimeCostMap.clear();
}

bool MarkovRecord::hasState(MarkovState &state)
{
    return stateList.contains(state);
}

void MarkovRecord::addState(MarkovState &state)
{
    int i = stateList.size();
    state.id = i;
    stateList.append(state);
}

MarkovState* MarkovRecord::getState(MarkovState &state)
{
    if (stateList.contains(state))
    {
        for (int i = stateList.size()-1; i>=0 ; i--)
        {
            if (state == (stateList[i]))
            {
                return &stateList[i];
            }
        }
    }
    return NULL;
}

MarkovState* MarkovRecord::getState(int i)
{
    if (i < stateList.size())
    {
        return &stateList[i];
    }
    return NULL;
}

int MarkovRecord::getStateSize()
{
    return stateList.size();
}

bool MarkovRecord::hasAction(MarkovAction & action)
{
    return actionList.contains(action);
}

void MarkovRecord::addAction(MarkovAction & action)
{
    int i = actionList.size();
    action.id = i;
    actionList.append(action);
}

MarkovAction* MarkovRecord::getAction(MarkovAction & action)
{
    if (actionList.contains(action))
    {
        for (int i = actionList.size()-1; i >= 0; i--)
        {
            if (action == (actionList[i]))
            {
                return &actionList[i];
            }
        }
    }
    return NULL;
}

MarkovAction* MarkovRecord::getAction(int i)
{
    if (i < actionList.size()) {
        return &actionList[i];
    }
    return NULL;
}

int MarkovRecord::getActionSize()
{
    return actionList.size();
}

bool MarkovRecord::hasStateBefore(MarkovState & state)
{
    return stateBeforeSet.contains(state);
}

bool MarkovRecord::hasStateAction(MarkovState & state, MarkovAction & action)
{
    StateAction sa(state, action);
    if (stateAction2PriceCostMap.contains(sa))
    {
        return true;
    }
    return false;
}

int MarkovRecord::getMaxLayerSize()
{
    return maxLayerSize;
}

void MarkovRecord::setMaxLayerSize(int _maxLayerSize)
{
    maxLayerSize = _maxLayerSize;
}

QString MarkovRecord::toString()
{
    QString res(QString("Record: [%1 %2 %3 %4 %5 %6]")
                .arg(stateBefore.toString())
                .arg(stateAfter.toString())
                .arg(action.toString())
                .arg(priceCost)
                .arg(timeCost)
                .arg(posibility));
    return res;
}

bool MarkovRecord::operator ==(const MarkovRecord & other)
{
    double e = 0.000001;
    if (stateBefore == other.stateBefore
            && stateAfter == other.stateAfter
            && action == other.action
            && fabs(priceCost - other.priceCost) < e
            && fabs(timeCost - other.timeCost) < e
            && fabs(posibility - other.posibility) < e)
    {
        return true;
    }
    return false;
}
