#include "test.h"
#include <ctime>
#include <QDebug>

Test::Test()
{
     qDebug() << "Test::Test()";
}

void Test::runLayerMarkovBackwardTest()
{

    /*********************************************************************************
    * Gen the fault state.
    *********************************************************************************/
    qDebug() << "Test::runLayerMarkovBackwardTest()  ...";
    MarkovState state;
    state.nextToDoActivity->x = (-1.0 / state.nextToDoActivity->blindService->execTime);
    state.init();
    state.id = 0;
    qDebug() << state.toString();

    /*********************************************************************************
     * Begin Markov test.
     *********************************************************************************/
    time_t startTime = clock();
    LayerMarkovBackward bd(state);
    bd.runMarkov();
//    //		bd.printRecords();
//    //    bd.printSimpleRecords();
    double maxUtility =  bd.getMarkovReward();
    qDebug() << "Markov:" << bd.getAction().name();
    qDebug() << "Cost:" << bd.getMarkovCost() << "TimeCost:" << bd.getMarkovTimeCost();

    if (maxUtility <= - INT_MAX + 1)
    {
        qDebug() << "Max utility: MIN_VALUE";
    }
    else
    {
        qDebug() << "Max utility:" << maxUtility;
    }
    qDebug() << "Utility:" << bd.getMarkovBestUtility();
    time_t midTime = clock();
    qDebug() << "Markov runtime:" << midTime - startTime << "ms.";

    //		bd.printUtility();
    //		bd.printMap();

    /*********************************************************************************
     * End Markov test. Begin Greedy.
     *********************************************************************************/
    bd.runGreedy();
    qDebug() << "Greedy:"  << bd.getGreedyAction().toString();
    qDebug() << "Cost:" << bd.getGreedyPriceCost() << "TimeCost:" << bd.getGreedyTimeCost();
    double greedyUtility = bd.getGreedyActionReward();
    if (greedyUtility <= - INT_MAX) {
        qDebug() << "Max utility: MIN_VALUE";
    } else {
        qDebug() << "Max utility:" << greedyUtility;
    }
    time_t endTime = clock();
    qDebug() << "Greedy runtime:" << endTime - midTime << "ms.";
    /*********************************************************************************
     * End greedy.
     *********************************************************************************/
    qDebug() << "Test::runLayerMarkovBackwardTest() finished.";
}
