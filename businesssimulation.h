#ifndef BUSINESSSIMULATION_H
#define BUSINESSSIMULATION_H
#include <vector>
#include <QSet>
#include <QWidget>
#include "intervalcoverage.h"

class Activity;
class ServiceGraph;
class BusinessAction;

class BusinessSimulation  //: public QWidget
{
//    Q_OBJECT
public:
    BusinessSimulation();
//    BusinessSimulation(QWidget *parent = 0);
    ~BusinessSimulation();

    void run();

    int resourceReplace(Activity* bug, QSet<int> & running);
    int termateDemand(Activity *bug, Activity* firstActivity);
    int doNothing(Activity *bug, Activity* firstActivity);
    int transResource(int bugFlow, Activity* bug);

private:
    bool init();
    bool initEarlyLateComplate();
    void printCurrState(int t, QSet<int> & runningActivity);
    void timePassed(Activity *startActivity, QSet<int> & runningActivity, QSet<int> & finishedActivity);
    void updatePainter(ServiceGraph & sg, QSet<int> & runningActivity, QSet<int> & finishedActivity,
                       QSet<int> &bugActivity);
    void sleepAMoment(int msec = 100);
//    void oneFlowProcess();

    std::vector<std::vector<int> > toGraph(Activity* a);
    SegMent* toSegMent(Activity* a);

    BusinessAction *actions;
    enum
    {
        REPLACE_POS = 0,
        TRANS_POS = 1,
        TERMINATE_POS = 2,
        DO_NOTHING_POS = 3,
        ADD_NEW_NEED_POS = 4,
        ACTIONS_COUNT = 5
    };
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


    // UI
    ServiceGraph* sg;
};

#endif // BUSINESSSIMULATION_H
