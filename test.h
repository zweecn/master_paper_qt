#ifndef TEST_H
#define TEST_H
#include "layermarkovbackward.h"

class Test
{
public:
    Test();

    void runLayerMarkovBackwardTest();

    void runMarkovTest1(MarkovState & state);
    MarkovState markovRecovery(MarkovState & state);

    void autoRun();
    bool isFinished();
    void timePassed(MarkovState & state);

private:
    QSet<int> runningActivities;
    QSet<int> finishedActivities;
    QSet<int> bugActivities;
};

#endif // TEST_H
