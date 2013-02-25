#ifndef BUSINESSSIMULATION_H
#define BUSINESSSIMULATION_H
#include <vector>
#include <QSet>
#include <QWidget>
#include "intervalcoverage.h"

class Activity;
class ServiceGraph;

class BusinessSimulation  //: public QWidget
{
//    Q_OBJECT
public:
    BusinessSimulation();
//    BusinessSimulation(QWidget *parent = 0);
    ~BusinessSimulation();

    void run();

    int resourceReplace(Activity* bug, QSet<int> & running);
    int termateDemand(Activity* firstActivity);
    int doNothing(Activity* firstActivity);
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

    int workflowCount;
    Activity** activities;
    int *workflowState;
    enum {WORKFLOW_FINISHED = 1,
          WORKFLOW_FAILED = 2,
          WORKFLOW_RUNNING = 3,
          WORKFLOW_READY = 4};

    // UI
    ServiceGraph* sg;
};

#endif // BUSINESSSIMULATION_H
