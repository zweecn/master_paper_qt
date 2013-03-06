#ifndef WEBSERVICEATOMSTATE_H
#define WEBSERVICEATOMSTATE_H

#include <QList>
#include <QString>

class WebServiceAtomState
{
public:
    WebServiceAtomState();
    WebServiceAtomState(const WebServiceAtomState & other);
    bool operator ==(const WebServiceAtomState & other) const;
    QString toString();

    int getId();
    void setId(int _id);
    void setActivityId(int _activityId);
    void setStateType(int _stateType);

    enum
    {
        EXEC = -1,

        READY_U = 0,
        READY_N = 1,
        FAIL = 2,
        FINISH_U = 3,
        FINISH_N = 4,
        STOP = 5,

        STATE_SIZE = 6
    };

    int id;
    int activityId;
    int stateType;
};

#endif // WEBSERVICEATOMSTATE_H
