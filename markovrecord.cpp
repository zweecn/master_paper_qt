#include "markovrecord.h"

#include <QHash>
#include <QDebug>

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
        return false;
    }
    return true;
}

int MarkovRecord::getMaxLayerSize()
{
    return maxLayerSize;
}

void MarkovRecord::setMaxLayerSize(int _maxLayerSize)
{
    maxLayerSize = _maxLayerSize;
}
