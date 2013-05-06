#ifndef WEBSERVICEEVENT_H
#define WEBSERVICEEVENT_H
#include <QString>
#include <QSet>


class Activity;

class WebServiceEvent
{
public:
    WebServiceEvent();

    bool operator ==(const WebServiceEvent & other) const;

    QString toString();
    QString name();

    static WebServiceEvent random(int currTime, QSet<int>& runningActivities,
                                                   QSet<int>& finishedActivities);
    static WebServiceEvent proRandom(int currTime,
                                     QSet<int>& runningActivities,
                                     QSet<int>& finishedActivities,
                                     Activity* firstActivity);

    int t;      // time
    int a;      // activity id
    int type;   // event type

    enum
    {
        NOT_EVENT = -1
    };
};

#endif // WEBSERVICEEVENT_H
