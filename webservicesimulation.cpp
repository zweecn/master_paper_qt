#include "webservicesimulation.h"
#include "resource.h"
#include "workflow.h"
#include "activity.h"
#include "config.h"
#include "markovaction.h"
#include "businesssimulation.h"
#include "criticalpath.h"

#include <QDebug>
#include <QFile>
#include <QIODevice>
#include <QTextCodec>
#include <QStringList>
#include <qmath.h>
#include <QSet>
#include <climits>
#include <iostream>

WebServiceSimulation::WebServiceSimulation() : QThread()
{
    init();
}

WebServiceSimulation::~WebServiceSimulation()
{
    clearData();
}


bool WebServiceSimulation::init()
{
    sg = NULL;
    currEvent = new WebServiceEvent();
    wsr = new WebServiceRecovery();
    wsf = &(wsr->getWebServiceFlow());
    return true;
}

bool WebServiceSimulation::clearData()
{
    delete currEvent;
    delete wsr;
    return true;
}

void WebServiceSimulation::run()
{
    autoRun();
}

void WebServiceSimulation::autoRun()
{
    qDebug() << "WebServiceSimulation::autoRun() ...";

    runningActivities.clear();
    finishedActivities.clear();
    bugActivities.clear();

    runningActivities.insert(0);

    int t = 0;
    qDebug() << t << isFinished() ;
    while (!isFinished())
    {
//        emit normalEventSignal();
//        qDebug() << "At t =" << t << "...";
        printCurrState(t);
        updatePainter();
        // [1] event
//        eventWidgetMutex.lock();
        qDebug() << "Random event...";
        *currEvent = WebServiceEvent::random(t, runningActivities, finishedActivities);
        qDebug() << "event=" << currEvent->toString();
//        bew->setEvent(currEvent);
//        eventWidgetMutex.unlock();

        // [2] update show
        bugActivities.insert(currEvent->a);
        updatePainter();
//        bsw->setActivities(activities);

        // [3]
        WebServiceAtomState state;
        state.activityId = currEvent->a;
        state.stateType = currEvent->type;
        qDebug() << "state=" << state.toString();
        qDebug() << "getMarkovresult...";
        markovResult = wsr->getMarkovResult(state);

        qDebug() << "getBestAction...";
        currAction = getBestAction();

        if (currAction == NULL)
            break;
        qDebug() << currEvent->toString() << currAction->toString();
        qDebug() << "recovery...";
        wsr->recovery(currAction);
        bugActivities.clear();
        updatePainter();

//        actionWidgetMutex.lock();
//        BusinessAction * action = operation(*currEvent);
//        baw->setBusinessAction(actions);
//        baw->setAutoBusinessAction(action);
//        actionWidgetMutex.unlock();

        // [4]
//        stateWidgetMutex.lock();
//        recovery(action);
//        stateWidgetMutex.unlock();

//        sleep(1);
        // [5] update show
//        updatePainter();
//        bsw->setActivities(activities);

//        printCurrState(t);

        // [6] sleep a moment & time passed & update show
//        sleep(1);
//        bugActivities.clear();

//        stateWidgetMutex.lock();
        qDebug() << "timePassed...";
        timePassed();
        printCurrState(t);
//        stateWidgetMutex.unlock();

//        updatePainter();
//        bsw->setActivities(activities);
        // [7] next second
        t++;
    }
    qDebug() << "WebServiceSimulation::autoRun() finished.";
}

bool WebServiceSimulation::isFinished()
{
    if (runningActivities.isEmpty() || wsf->isFinished())
    {
        return true;
    }
    return false;
}

void WebServiceSimulation::timePassed()
{
    Activity *startActivity = wsf->activities;
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

void WebServiceSimulation::printCurrState(int t)
{
    qDebug() << "At " << t << ", running: " << runningActivities
             << wsf->toString();
}

WebServiceAction* WebServiceSimulation::getBestAction()
{
//    qDebug() << "WebServiceSimulation::getBestAction() ...";
    if (markovResult.isEmpty())
        return NULL;

    MarkovResultItem * res = NULL;
    for (int i = 0; i < markovResult.size(); i++)
    {
//        qDebug() << markovResult[i].toString();
        if (res == NULL || res->potentialReward < markovResult[i].potentialReward)
        {
            res = &markovResult[i];
        }
    }
//    qDebug() << "WebServiceSimulation::getBestAction() finished.";
    return &(res->action);
}

void WebServiceSimulation::setServiceGraph(ServiceGraph *_sg)
{
    sg = _sg;
}

// 更新显示
void WebServiceSimulation::updatePainter()
{
    if (sg == NULL)
    {
        return;
    }
    QSet<int> & runningActivity = runningActivities;
    QSet<int> & finishedActivity = finishedActivities;
    QSet<int> & bugActivity = bugActivities;

//    serviceGraphMutex.lock();
    QList<QColor> colors = sg->getColors();
    for (int i = 0; i < WorkFlow::Instance()->getActivitySize(); i++) {
        if (finishedActivity.contains(i)) {
            colors[i].setRgb(0, 0, 255);
        } else if (runningActivity.contains(i)) {
            colors[i].setRgb(255, 255, 0);
        } else if (bugActivity.contains(i)) {
            colors[i].setRgb(255, 0, 0);
        } else {
            colors[i].setRgb(0, 255, 0);
        }
    }
    sg->setColors(colors);
//    serviceGraphMutex.unlock();
}
