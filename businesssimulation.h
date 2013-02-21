#ifndef BUSINESSSIMULATION_H
#define BUSINESSSIMULATION_H
#include <vector>

class Activity;

class BusinessSimulation
{
public:
    BusinessSimulation();

    void run();
private:
    bool init();
    std::vector<std::vector<int> > toGraph(Activity* a);

    int workflowCount;
    Activity** activities;

};

#endif // BUSINESSSIMULATION_H
