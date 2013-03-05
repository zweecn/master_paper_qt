#ifndef MARKOVRECORD_H
#define MARKOVRECORD_H
#include "markovstate.h"
#include "markovaction.h"

#include <QMap>
#include <QHash>

class StateAction
{
public:
    StateAction(MarkovState & _state, MarkovAction & _action)
    {
        state = _state;
        action = _action;
    }
    bool operator ==(const StateAction & other) const
    {
        return (state == other.state && action == other.action);
    }

    MarkovState state;
    MarkovAction action;
};


extern uint qHash(const StateAction & key);
extern uint qHash(const MarkovState & key);
extern uint qHash(const MarkovAction & key);

class MarkovRecord
{
public:
    MarkovRecord();
    MarkovRecord(MarkovState & _stateBefore, MarkovState & _stateAfter, MarkovAction & _action,
                 double _posibility, double _priceCost, double _timeCost);

    MarkovRecord & operator =(const MarkovRecord& other);
    bool operator ==(const MarkovRecord & other);
    bool init();
    QString toString();

    MarkovState stateBefore;
    MarkovState stateAfter;
    MarkovAction action;
    double posibility;
    double priceCost;
    double timeCost;

    static void addStateAction(MarkovState & _state, MarkovAction & _action, double _price, double _time);
    static bool hasState(MarkovState &state);
    static void addState(MarkovState &state);
    static MarkovState* getState(MarkovState &state);
    static MarkovState* getState(int i);
    static int getStateSize();
    static bool hasAction(MarkovAction & action);
    static void addAction(MarkovAction & action);
    static MarkovAction* getAction(MarkovAction & action);
    static MarkovAction* getAction(int i);
    static int getActionSize();
    static bool hasStateBefore(MarkovState & state);
    static bool hasStateAction(MarkovState & state, MarkovAction & action);
    static int getMaxLayerSize();
    static void setMaxLayerSize(int _maxLayerSize);
    static void clear();

};

#endif // MARKOVRECORD_H
