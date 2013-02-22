#ifndef BUSINESSSIMULATION_H
#define BUSINESSSIMULATION_H
#include <vector>
#include <QSet>
#include <QWidget>

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
private:
    bool init();
    void printCurrState(int t, QSet<int> & runningActivity);
    void timePassed(Activity *startActivity, QSet<int> & runningActivity, QSet<int> & finishedActivity);
    void updatePainter(ServiceGraph & sg, QSet<int> & runningActivity, QSet<int> & finishedActivity);
    void sleepAMoment();
//    void oneFlowProcess();

    std::vector<std::vector<int> > toGraph(Activity* a);

    int workflowCount;
    Activity** activities;
    ServiceGraph* sg;
};

#endif // BUSINESSSIMULATION_H
