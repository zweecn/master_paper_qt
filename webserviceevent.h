#ifndef WEBSERVICEEVENT_H
#define WEBSERVICEEVENT_H
#include <QString>
#include <QSet>

class WebServiceEvent
{
public:
    WebServiceEvent();

    QString toString();
    QString name();

    static WebServiceEvent random(int currTime, QSet<int>& runningActivities,
                                                   QSet<int>& finishedActivities);

    int t;      // time
    int a;      // activity id
    int type;   // event type
};

#endif // WEBSERVICEEVENT_H
