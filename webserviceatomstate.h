#ifndef WEBSERVICEATOMSTATE_H
#define WEBSERVICEATOMSTATE_H

#include <QList>

class WebServiceAtomState
{
public:
    WebServiceAtomState();
    static QList<int> aviableActions(WebServiceAtomState & state);

    int activityId;
    int stateType;
};

#endif // WEBSERVICEATOMSTATE_H
