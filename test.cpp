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
//    bd.runMarkov();
//    //		bd.printRecords();
//    //    bd.printSimpleRecords();
//    double maxUtility =  bd.getCurrActionReward();
//    qDebug() << "Markov:" << bd.getAction().name();
//    qDebug() << "Cost:" << bd.getCurrActionCost() << "TimeCost:" << bd.getCurrActionTimeCost();

//    if (maxUtility <= - INT_MAX + 1)
//    {
//        qDebug() << "Max utility: MIN_VALUE";
//    }
//    else
//    {
//        qDebug() << "Max utility:" << maxUtility;
//    }
//    qDebug() << " Utility:" << bd.getMarkovBestUtility();
//    qDebug() << "\n";

    //		bd.printUtility();
    //		bd.printMap();

//    /*********************************************************************************
//     * End Markov test. Begin Greedy.
//     *********************************************************************************/
//    bd.runGreedy();
//    System.out.print("Greedy:" + bd.getGreedyAction());
//    qDebug()("Cost: %.2f TimeCost: %.2f ", bd.getGreedyPriceCost(), bd.getGreedyTimeCost());
//    double greedyUtility = bd.getGreedyActionReward();
//    if (greedyUtility <= - Double.MAX_VALUE) {
//        qDebug() << (" Max utility: %s ", "MIN_VALUE");
//    } else {
//        qDebug() << (" Max utility: %.2f ", greedyUtility);
//    }
//    qDebug() << ("\n");
//    /*********************************************************************************
//     * End greedy.
//     *********************************************************************************/

//    long endTime=System.currentTimeMillis();
//    qDebug() << ("\nTotal RunTime: "
//                 + (endTime - startTime) + " ms. (include the runtime of print the record and print steps)");
    qDebug() << "Test::runLayerMarkovBackwardTest() finished.";
}
