#ifndef BUSINESSSIMULATION_H
#define BUSINESSSIMULATION_H
#include <vector>
#include <QSet>
#include <QWidget>

class Activity;
class ServiceGraph;

class BusinessSimulation // : public QWidget
{
//    Q_OBJECT
public:
    BusinessSimulation();
    ~BusinessSimulation();

    void run();
private:
    bool init();
    void printCurrState(int t, QSet<int> & runningActivity);
    void timePassed(int t, Activity *startActivity, QSet<int> & runningActivity, QSet<int> & finishedActivity);
    void updatePainter(QSet<int> & runningActivity, QSet<int> & finishedActivity);

    std::vector<std::vector<int> > toGraph(Activity* a);

    int workflowCount;
    Activity** activities;
    ServiceGraph* sg;
};

#endif // BUSINESSSIMULATION_H
