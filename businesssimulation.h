#ifndef BUSINESSSIMULATION_H
#define BUSINESSSIMULATION_H
#include <vector>
#include <QSet>
#include <QWidget>
#include <QThread>
#include <QWaitCondition>

#include "intervalcoverage.h"

class Activity;
class ServiceGraph;
class BusinessAction;
class BusinessEvent;
class BusinessEventWidget;
class BusinessActionWidget;
class BusinessStateWidget;

class BusinessSimulation : public  QThread
{
    Q_OBJECT
public:
    BusinessSimulation();
    ~BusinessSimulation();

    void run();
    void autoRun();
    void manualRun();

    ServiceGraph* getServiceGraph();
    void setServiceGraph(ServiceGraph* _sg);
    BusinessEventWidget* getBusinessEventWidget();
    void setBusinessEventWidget(BusinessEventWidget * _bew);
    BusinessActionWidget* getBusinessActionWidget();
    void setBusinessActionWidget(BusinessActionWidget* _baw);
    void setBusinessStateWidget(BusinessStateWidget* _bsw);

    void setAutoRun(bool _isAutoRun);
    void setSelectActionId(int _selectActionId);
    BusinessAction* operation(BusinessEvent & event);
    bool recovery(BusinessAction *action);

    int resourceReplace(BusinessEvent & event);
    int termateDemand(BusinessEvent & event);
    int doNothing(BusinessEvent & event);
    bool transResource(BusinessEvent & event);

    int getWorkflowCount();

signals:
    void normalEventSignal();
    void badEventSignal();

private:
    bool init();
    bool initEarlyLateComplate();
    bool isFinished();

    void clearData();
    void printCurrState(int t, int flowId);

    void timePassed(int flowId);
    void timePassed();
    void updatePainter(int flowId, ServiceGraph & sg);
    void updatePainter();

    void sleepAMoment(int msec = 1000);

    std::vector<std::vector<int> > toGraph(Activity* a);
    SegMent* toSegMent(Activity* a);


    int workflowCount;
    Activity** activities;
    int *workflowState;
    enum
    {
        WORKFLOW_FINISHED = 1,
        WORKFLOW_FAILED = 2,
        WORKFLOW_RUNNING = 3,
        WORKFLOW_READY = 4
    };


    QSet<int> *runningActivities;
    QSet<int> *finishedActivities;
    QSet<int> *bugActivities;

    // Below is the data should be lock when they are read/write
    BusinessEvent* currEvent;
    BusinessAction *actions;
    int selectActionId;

    // UI

    ServiceGraph* sg;
    BusinessEventWidget* bew;
    BusinessActionWidget* baw;
    BusinessStateWidget* bsw;
    bool isAutoRun;
};

#endif // BUSINESSSIMULATION_H
