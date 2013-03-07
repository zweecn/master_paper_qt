#ifndef WEBSERVICESIMULATION_H
#define WEBSERVICESIMULATION_H
#include "webserviceflow.h"
#include "webservicerecovery.h"
#include "webserviceevent.h"
#include "servicegraph.h"
#include "webserviceeventwidget.h"
#include "webserviceactionwidget.h"
#include "webserviceflowinfowidget.h"

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
    void manualRun();

    void setServiceGraph(ServiceGraph *_sg);
    void setWebServiceEventWidget(WebServiceEventWidget* _wsew);
    void setWebServiceActionWidget(WebServiceActionWidget* _wsaw);
    void setWebServiceFlowInfoWidget(WebServiceFlowInfoWidget* _wsfiw);
    void setAutoRun(bool _isAutoRun);
    void setSelectActionId(int _selectActionId);
    void setSleepMSecond(int _sleepMSecond);

signals:
    void normalEventSignal();
    void badEventSignal();
    void execFinishedSignal();

private:
    bool init();
    bool clearData();
    bool isFinished();

    WebServiceAction * getBestAction();
    MarkovResultItem* getBestMarkovResult();
    void timePassed();
    void printCurrState(int t);

    void updatePainter();

    void makeMarkov();

    WebServiceFlow * wsf;
    WebServiceRecovery * wsr;

    QSet<int> runningActivities;
    QSet<int> finishedActivities;
    QSet<int> bugActivities;

    // Below is the data should be lock when they are read/write
    WebServiceEvent * currEvent;
    WebServiceEventRecordItem * eventHistoryItem;
    WebServiceAction * currAction;
    QList<MarkovResultItem> markovResult;
    double bestPotentialReward;
    double bestProbility;
    int selectActionId;
    int sleepMSecond;

    // UI
    ServiceGraph *sg;
    WebServiceEventWidget* wsew;
    WebServiceActionWidget* wsaw;
    WebServiceFlowInfoWidget* wsfiw;
    bool isAutoRun;
};

#endif // WEBSERVICESIMULATION_H
