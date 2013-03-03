#ifndef WEBSERVICEACTION_H
#define WEBSERVICEACTION_H

#include <QList>

class AtomService;

class WebServiceAction
{
public:
    WebServiceAction();
    enum
    {
        TERMINATE = 0,
        DO_NOTHING = 1,
        RETRY = 2,
        SUBSTITUTE = 3,
        RECOMPOSE = 4,

        ACTION_COUNT = 5
    };


    int type;
    int bugActivityId;

    AtomService * oldService;
    AtomService * newService;

    QList<int> replaceActivityList;
    QList<AtomService*> oldServiceList;
    QList<AtomService*> newServiceList;
};

#endif // WEBSERVICEACTION_H
