#ifndef WEBSERVICESIMULATION_H
#define WEBSERVICESIMULATION_H
#include "webserviceflow.h"
#include "webservicerecovery.h"
#include "webserviceevent.h"

#include <QList>
#include <QThread>

class WebServiceSimulation : public  QThread
{
    Q_OBJECT

public:
    WebServiceSimulation();
    ~WebServiceSimulation();

    void run();

    void autoRun();

private:
    bool init();
    bool clearData();
    bool isFinished();

    WebServiceAction * getBestAction();
    void timePassed();
    void printCurrState(int t);

    WebServiceFlow * wsf;
    WebServiceRecovery * wsr;

    QSet<int> runningActivities;
    QSet<int> finishedActivities;
    QSet<int> bugActivities;

    // Below is the data should be lock when they are read/write
    WebServiceEvent * currEvent;
    WebServiceAction * currAction;
    QList<MarkovResultItem> markovResult;
    int selectActionId;
};

#endif // WEBSERVICESIMULATION_H
