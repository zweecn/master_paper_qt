#include "test.h"
#include "webserviceevent.h"
#include "markovstate.h"
#include "markovaction.h"
#include "businesssimulation.h"
#include "criticalpath.h"
#include "webservicerecovery.h"
#include "webservicesimulation.h"
#include "webservicemainwidget.h"

#include <ctime>
#include <QDebug>
#include <QString>

Test::Test()
{
    qDebug() << "Test::Test() ...";

//    runLayerMarkovBackwardTest();
//    runMarkovTest();
//    runWebServiceSimulation();
    runWebServiceMainWidgetTest();

    qDebug() << "Test::test() finished.";
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
    qDebug() << "state=" <<state.toString();

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

void Test::runMarkovTest()
{
    WebServiceRecovery wsr;
    WebServiceAtomState s;
    s.activityId = 0;
    s.stateType = WebServiceAtomState::FAIL;
    QList<MarkovResultItem> result = wsr.getMarkovResult(s);
    qDebug() << "result.size() =" << result.size();
    for (int i = 0; i < result.size(); i++)
    {
        qDebug() << result[i].toString();
    }
    wsr.recovery(&result[1].action);

}

void Test::runWebServiceSimulation()
{
    WebServiceSimulation wsm;
    wsm.autoRun();
}

void Test::runWebServiceMainWidgetTest()
{
    WebServiceMainWidget w;
    w.show();
}
