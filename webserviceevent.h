#ifndef WEBSERVICEEVENT_H
#define WEBSERVICEEVENT_H
#include <QString>

class WebServiceEvent
{
public:
    WebServiceEvent();

    QString toString();
    QString name();

    static WebServiceEvent random(int currTime);

    int t;      // time
    int a;      // activity id
    int type;   // event type
};

#endif // WEBSERVICEEVENT_H
