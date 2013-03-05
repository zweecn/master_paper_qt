#include "test.h"
#include "webserviceevent.h"
#include "markovstate.h"
#include "markovaction.h"
#include "businesssimulation.h"
#include "criticalpath.h"

#include <ctime>
#include <QDebug>
#include <QString>

Test::Test()
{
    qDebug() << "Test::Test() ...";

    runLayerMarkovBackwardTest();
//    autoRun();

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

MarkovState Test::markovRecovery(MarkovState & state)
{
    state.id = 0;
    state.init();

    MarkovState stateTemp = state;
    stateTemp.init();

    LayerMarkovBackward bd(stateTemp);
    bd.runMarkov();

    qDebug() << bd.getResultString();
    qDebug() << "Below is all records.";
//    bd.printAllLayerRecords();

    return bd.getStateNew();
}

void Test::runMarkovTest1(MarkovState & state)
{
    //    MarkovRecord::clear();
    //    MarkovState markovState = state;
    //    markovState.init();
    //    int i=1;
    //    do {
    //        if (markovState.isFailed()) {
    //            markovState = markovRecovery(i, markovState, i2markovActionMap);
    //        }
    //        if (markovState == null) {
    //            System.out.println("Code 0x01");
    //            break;
    //        }
    //        if (markovState.isFailed()) {
    //            System.out.println("Code 0x02");
    //            break;
    //        }
    //        if (markovState.isFinished()) {
    //            System.out.println("Finished. Code 0x03");
    //            break;
    //        }
    //        markovState = markovState.nextSecond();
    //        i++;
    //        if (markovState == null) {
    //            System.out.println("Code 0x04");
    //            break;
    //        }
    //        markovState.init();
    //        if (!markovState.isFailed() && !markovState.isFinished()
    //                && Test.isFault(markovState.getNextToDoActivity())) {
    //            markovState = Test.makeFault(markovState);
    //            markovState.init();
    //        }
    //    } while (true);
    //    markovTime = i + 1;
}

void Test::autoRun()
{
    runningActivities.clear();
    finishedActivities.clear();
    bugActivities.clear();
    runningActivities.insert(0);

    MarkovState state;
    int t = 0;
    while (! isFinished())
    {
        qDebug() << "At t =" << t << "...";

        WebServiceEvent event = WebServiceEvent::random(t);
//        qDebug() << event.toString();
//        qDebug() << state.toString();

        QSetIterator<int> it(runningActivities);
        if (it.hasNext())
            state.activities[it.next()].x = -1;
//        qDebug() << state.toString();
        markovRecovery(state);
//        qDebug() << state.toString();
        timePassed(state);
        break;
        BusinessSimulation::sleepAMoment(2000);


        // [1] event
        //        eventWidgetMutex.lock();
        //        *currEvent = BusinessEvent::random(t, activities, workflowCount);
        //        bew->setEvent(currEvent);
        //        eventWidgetMutex.unlock();

        //        // [2] update show
        //        bugActivities[currEvent->n].insert(currEvent->a);
        //        updatePainter();
        //        bsw->setActivities(activities);

        //        // [3]
        //        actionWidgetMutex.lock();
        //        BusinessAction * action = operation(*currEvent);
        //        baw->setBusinessAction(actions);
        //        baw->setAutoBusinessAction(action);
        //        actionWidgetMutex.unlock();

        //        // [4]
        //        stateWidgetMutex.lock();
        //        recovery(action);
        //        stateWidgetMutex.unlock();

        //        sleep(1);
        //        // [5] update show
        //        updatePainter();
        //        bsw->setActivities(activities);

        //        // [6] sleep a moment & time passed & update show
        //        sleep(1);

        //        stateWidgetMutex.lock();
        //        timePassed();
        //        stateWidgetMutex.unlock();

        //        updatePainter();
        //        bsw->setActivities(activities);

        // [7] next second
        t++;
    }
}

bool Test::isFinished()
{
    return runningActivities.isEmpty();
}

void Test::timePassed(MarkovState & state)
{
    Activity *startActivity = state.activities;
    QSet<int> & runningActivity = runningActivities;
    QSet<int> & finishedActivity = finishedActivities;

    std::vector<std::vector<int> > g = BusinessSimulation::toGraph(startActivity);
    CriticalPath cp(WorkFlow::Instance()->getActivitySize(), g);
    cp.run();
    QSetIterator<int> it(runningActivity);
    while (it.hasNext())
    {
        int tmpNum = it.next();
        Activity* tmp = &startActivity[tmpNum];
        tmp->x += 1.0/cp.getLatestTime();
        if (tmp->x >= 1.0) {
            tmp->x = 1.0;
            tmp->state = Activity::FINISHED;
            finishedActivity.insert(tmpNum);
            // 加入后续的活动
            for (int x = 0; x < WorkFlow::Instance()->getActivitySize(); x++) {
                QSet<int> prefixs = WorkFlow::Instance()->getPrefixs(x);
                if ((prefixs - finishedActivity).isEmpty()) {
                    runningActivity.insert(x);
                }
            }
        }
    }
    it = finishedActivity;
    while (it.hasNext()) {
        runningActivity.remove(it.next());
    }
}
