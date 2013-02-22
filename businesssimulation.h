#ifndef BUSINESSSIMULATION_H
#define BUSINESSSIMULATION_H
#include <vector>
#include <QSet>

class Activity;

class BusinessSimulation
{
public:
    BusinessSimulation();

    void run();
private:
    bool init();
    void printCurrState(int t, QSet<int> & runningActivity);
    void timePassed(int t, Activity *startActivity, QSet<int> & runningActivity, QSet<int> & finishedActivity);

    std::vector<std::vector<int> > toGraph(Activity* a);

    int workflowCount;
    Activity** activities;

};

#endif // BUSINESSSIMULATION_H
