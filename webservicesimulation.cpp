#include "webservicesimulation.h"
#include "resource.h"
#include "workflow.h"
#include "activity.h"
#include "config.h"
#include "markovaction.h"
#include "businesssimulation.h"
#include "criticalpath.h"
#include "allmutex.h"

#include <cassert>
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
    wsew = NULL;
    wsaw = NULL;
    wsfiw = NULL;
    bestPotentialReward = 0;
    bestProbility = 0;
    selectActionId = 0;
    sleepMSecond = 0;
    eventHistoryItem = new WebServiceEventRecordItem();
    currEvent = new WebServiceEvent();
    wsr = new WebServiceRecovery();
    wsf = &(wsr->getWebServiceFlow());
    return true;
}

bool WebServiceSimulation::clearData()
{
    if (eventHistoryItem != NULL)
        delete eventHistoryItem;
    if (currEvent != NULL)
        delete currEvent;
    if (wsr != NULL)
        delete wsr;
    return true;
}

void WebServiceSimulation::run()
{
    qDebug() << "WebServiceSimulation::run() ...";
    isStop = false;
    if (isAutoRun)
        autoRun();
    else
        manualRun();
    emit stopSignal();
    qDebug() << "WebServiceSimulation::run() finished.";
}

void WebServiceSimulation::stop()
{
    isStop = true;
}

void WebServiceSimulation::autoRun()
{
    qDebug() << "WebServiceSimulation::autoRun() ...";

    runningActivities.clear();
    finishedActivities.clear();
    bugActivities.clear();

    runningActivities.insert(0);

    serviceFlowInfoWidgetMutex.lock();
    wsfiw->setWebServiceFlow(wsf);
    serviceFlowInfoWidgetMutex.unlock();

    int t = 0;
    qDebug() << t << isFinished() ;
    while (!isStop && !isFinished())
    {
        emit normalEventSignal();
        printCurrState(t);
        // [0]
        qDebug() << "[0] Update service flow show...";
        updatePainter();
        serviceFlowInfoWidgetMutex.lock();
        wsfiw->setWebServiceFlow(wsf);
        serviceFlowInfoWidgetMutex.unlock();
        usleep(sleepMSecond);

        // [1] event
        qDebug() << "[1] Random event...";
        eventWidgetMutex.lock();
        *currEvent = WebServiceEvent::random(t, runningActivities, finishedActivities);
        wsew->setEvent(currEvent);
        qDebug() << " Event:" << currEvent->toString();
        eventWidgetMutex.unlock();
        emit badEventSignal();

        // [2] update show
        qDebug() << "[2] Update show...";
        bugActivities.insert(currEvent->a);
        updatePainter();

        // [3]
        qDebug() << "[3] Cause bad state...";
        WebServiceAtomState state;
        state.activityId = currEvent->a;
        state.stateType = currEvent->type;
        qDebug() << " State:" << state.toString();

        // [4]
        qDebug() << "[4] Find markov solution...";
        actionWidgetMutex.lock();
        markovResult = wsr->getMarkovResult(state);
        wsaw->setMarkovResult(markovResult);
        actionWidgetMutex.unlock();

        // [5]
        qDebug() << "[5] GetBestMarkovResult..." ;
//        currAction = getBestAction();
        MarkovResultItem* currItem = getBestMarkovResult();
        currAction = &(currItem->action);
        if (currItem == NULL)
        {
            qDebug() << "getBestMarkovResult() failed.";
            break;
        }
        qDebug() << " Action:" << currAction->toString();
        usleep(sleepMSecond);

        // [6]
        qDebug() << "[6] Recovery with action...";
        serviceFlowInfoWidgetMutex.lock();
        wsr->recovery(currAction);
        wsfiw->setWebServiceFlow(wsf);
        if (wsf->isFinished())
        {
            break;
        }
        serviceFlowInfoWidgetMutex.unlock();

        // [7]
        qDebug() << "[7] Show service flow after recovery...";
        bugActivities.clear();
        updatePainter();
        usleep(sleepMSecond);

        // [8]
        qDebug() << "[8] Update event history...";
        eventHistoryWidgetMutex.lock();
        eventHistoryItem->event = *currEvent;
        eventHistoryItem->result = *currItem;
        wsew->addWebServiceEventRecordItem(eventHistoryItem);
        eventHistoryWidgetMutex.unlock();

        // [9]
        qDebug() << "[9] One second passed...";
        serviceFlowInfoWidgetMutex.lock();
        timePassed();
        wsfiw->setWebServiceFlow(wsf);
        serviceFlowInfoWidgetMutex.unlock();

        // [10]
        qDebug() << "[10] One second passed, update service flow...";
        updatePainter();

        t++;
    }
    emit execFinishedSignal();
    qDebug() << "WebServiceSimulation::autoRun() finished.";
}

void WebServiceSimulation::manualRun()
{
    qDebug() << "WebServiceSimulation::manualRun() ...";

    runningActivities.clear();
    finishedActivities.clear();
    bugActivities.clear();

    runningActivities.insert(0);

    serviceFlowInfoWidgetMutex.lock();
    wsfiw->setWebServiceFlow(wsf);
    serviceFlowInfoWidgetMutex.unlock();

    int t = 0;
    qDebug() << t << isFinished() ;
    while (!isStop && !isFinished())
    {
        emit normalEventSignal();
        printCurrState(t);
        // [0]
        qDebug() << "[0] Update service flow show...";
        updatePainter();
        serviceFlowInfoWidgetMutex.lock();
        wsfiw->setWebServiceFlow(wsf);
        serviceFlowInfoWidgetMutex.unlock();
        usleep(sleepMSecond);

        // [1] event
        qDebug() << "[1] Random event...";
        eventWidgetMutex.lock();
        *currEvent = WebServiceEvent::random(t, runningActivities, finishedActivities);
        wsew->setEvent(currEvent);
        qDebug() << " Event:" << currEvent->toString();
        eventWidgetMutex.unlock();
        emit badEventSignal();

        // [2] update show
        qDebug() << "[2] Update show...";
        bugActivities.insert(currEvent->a);
        updatePainter();

        // [3] [4]
//        makeMarkov();
        qDebug() << "[3] Cause bad state...";
        WebServiceAtomState state;
        state.activityId = currEvent->a;
        state.stateType = currEvent->type;
        qDebug() << " State:" << state.toString();

        qDebug() << "[4] Find markov solution...";
        actionWidgetReadWriteMutex.lockForWrite();
        markovResult = wsr->getMarkovResult(state);
        wsaw->setMarkovResult(markovResult);
        if (isStop)
        {
            qDebug()<< "User stop WebServiceSimulation.";
            actionWidgetReadWriteMutex.unlock();
            break;
        }
        nextStepCond.wait(&actionWidgetReadWriteMutex);
        actionWidgetReadWriteMutex.unlock();

        // [5]
        qDebug() << "[5] GetBestMarkovResult..." ;
        assert(selectActionId >=0 && selectActionId < markovResult.size());
        currAction = &markovResult[selectActionId].action;
        MarkovResultItem resItem = markovResult[selectActionId];
        if (currAction == NULL)
        {
            qDebug() << "GetBestAction failed.";
            break;
        }
        qDebug() << " Action:" << currAction->toString();
        usleep(sleepMSecond);

        // [6]
        qDebug() << "[6] Recovery with action...";
        serviceFlowInfoWidgetMutex.lock();
        wsr->recovery(currAction);
        wsfiw->setWebServiceFlow(wsf);
        serviceFlowInfoWidgetMutex.unlock();

        // [7]
        qDebug() << "[7] Show service flow after recovery...";
        bugActivities.clear();
        updatePainter();
        usleep(sleepMSecond);

        // [8]
        qDebug() << "[8] Update event history...";
        eventHistoryWidgetMutex.lock();
        eventHistoryItem->event = *currEvent;
        eventHistoryItem->result = resItem;
        wsew->addWebServiceEventRecordItem(eventHistoryItem);
        eventHistoryWidgetMutex.unlock();

        // [9]
        qDebug() << "[9] One second passed...";
        serviceFlowInfoWidgetMutex.lock();
        timePassed();
        wsfiw->setWebServiceFlow(wsf);
        serviceFlowInfoWidgetMutex.unlock();

        // [10]
        qDebug() << "[10] One second passed, update service flow...";
        updatePainter();

        t++;
    }
    emit execFinishedSignal();
    qDebug() << "WebServiceSimulation::manualRun() finished.";
}

void WebServiceSimulation::makeMarkov()
{
    qDebug() << "[3] Cause bad state...";
    WebServiceAtomState state;
    state.activityId = currEvent->a;
    state.stateType = currEvent->type;
    qDebug() << " State:" << state.toString();

    qDebug() << "[4] Find markov solution...";
//    actionWidgetMutex.lock();

    actionWidgetReadWriteMutex.lockForWrite();
    markovResult = wsr->getMarkovResult(state);
    wsaw->setMarkovResult(markovResult);
    nextStepCond.wait(&actionWidgetReadWriteMutex);
    actionWidgetReadWriteMutex.unlock();

//    actionWidgetMutex.unlock();
}

bool WebServiceSimulation::isFinished()
{
    if (runningActivities.isEmpty() || wsf->isFinished())
    {
        qDebug() << "WebServiceSimulation::isFinished() ... true";
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
    qDebug() << "At t =" << t << ", running: " << runningActivities
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
    bestPotentialReward = res->potentialReward;
    bestProbility = res->successProbility;
    return &(res->action);
}

MarkovResultItem* WebServiceSimulation::getBestMarkovResult()
{
    if (markovResult.isEmpty())
        return NULL;

    MarkovResultItem * res = NULL;
    for (int i = 0; i < markovResult.size(); i++)
    {
        if (res == NULL || res->potentialReward < markovResult[i].potentialReward)
        {
            res = &markovResult[i];
        }
    }
    bestPotentialReward = res->potentialReward;
    bestProbility = res->successProbility;
    return res;
}

void WebServiceSimulation::setServiceGraph(ServiceGraph *_sg)
{
    sg = _sg;
}

// 更新显示
void WebServiceSimulation::updatePainter()
{
    qDebug() << "void WebServiceSimulation::updatePainter() ...";
    if (sg == NULL)
    {
        return;
    }
    QSet<int> & runningActivity = runningActivities;
    QSet<int> & finishedActivity = finishedActivities;
    QSet<int> & bugActivity = bugActivities;

    serviceGraphMutex.lock();
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
    serviceGraphMutex.unlock();
    qDebug() << "void WebServiceSimulation::updatePainter() finished.";
}

void WebServiceSimulation::setWebServiceEventWidget(WebServiceEventWidget* _wsew)
{
    wsew = _wsew;
}

void WebServiceSimulation::setWebServiceActionWidget(WebServiceActionWidget* _wsaw)
{
    wsaw = _wsaw;
}

void WebServiceSimulation::setWebServiceFlowInfoWidget(WebServiceFlowInfoWidget* _wsfiw)
{
    wsfiw = _wsfiw;
}

void WebServiceSimulation::setAutoRun(bool _isAutoRun)
{
    isAutoRun = _isAutoRun;
}

void WebServiceSimulation::setSelectActionId(int _selectActionId)
{
    selectActionId = _selectActionId;
}

void WebServiceSimulation::setSleepMSecond(int _sleepMSecond)
{
    sleepMSecond = _sleepMSecond;
}
