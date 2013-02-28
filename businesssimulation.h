#ifndef BUSINESSSIMULATION_H
#define BUSINESSSIMULATION_H
#include <vector>
#include <QSet>
#include <QWidget>
#include "intervalcoverage.h"

class Activity;
class ServiceGraph;
class BusinessAction;
class BusinessEvent;
class BusinessEventWidget;
class BusinessActionWidget;

class BusinessSimulation
{
public:
    BusinessSimulation();
    ~BusinessSimulation();

    void run();

    ServiceGraph* getServiceGraph();
    void setServiceGraph(ServiceGraph* _sg);
    BusinessEventWidget* getBusinessEventWidget();
    void setBusinessEventWidget(BusinessEventWidget * _bew);
    BusinessActionWidget* getBusinessActionWidget();
    void setBusinessActionWidget(BusinessActionWidget* _baw);

    BusinessAction* operation(BusinessEvent & event);
    bool recovery(BusinessAction *action);

    int resourceReplace(BusinessEvent & event);
    int termateDemand(BusinessEvent & event);
    int doNothing(BusinessEvent & event);
    bool transResource(BusinessEvent & event);

    int getWorkflowCount();

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

    BusinessAction *actions;

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

    // UI
    ServiceGraph* sg;
    BusinessEventWidget* bew;
    BusinessActionWidget* baw;
};

#endif // BUSINESSSIMULATION_H
