#ifndef WEBSERVICEATOMSTATE_H
#define WEBSERVICEATOMSTATE_H

#include <QList>
#include <QString>

class WebServiceAtomState
{
public:
    WebServiceAtomState();
    bool operator ==(const WebServiceAtomState & other) const;
    int hash();
    QString toString();

    enum
    {
        READY_U = 0,
        READY_N = 1,
        FAIL = 2,
        FINISH_U = 3,
        FINISH_N = 4,
        STOP = 5,

        STATE_SIZE = 6
    };

    int activityId;
    int stateType;
};

#endif // WEBSERVICEATOMSTATE_H
