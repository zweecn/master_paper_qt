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
    qDebug() << "WebServiceSimulation::run() ..." << runType;

    isStop = false;
    if (runType == RUNTYPE_MATLAB_MARKOV)
    {
        matlabCmd = autoMarkovRun();
    }
    else if (runType == RUNTYPE_MATLAB_GREEDY)
    {
        matlabCmd = autoGreedyRun();
    }
    else if (runType == RUNTYPE_SIM_AUTO)
    {
        autoRun();
    }
    else if (runType == RUNTYPE_SIM_MANUAL)
    {
        manualRun();
    }

    qDebug() << "WebServiceSimulation::run() finished.";
//    emit stopSignal();
}

void WebServiceSimulation::stop()
{
    isStop = true;
}

QString WebServiceSimulation::autoMarkovRun()
{
    runningActivities.clear();
    finishedActivities.clear();
    bugActivities.clear();

    runningActivities.insert(0);

    QList<WebServiceEvent> eventList;
    QList<MarkovResultItem> markovItemList;

    int t = 0;
    qDebug() << t << isFinished() ;
    while (!isStop && !isFinished())
    {
        printCurrState(t);

        // [1] event
        qDebug() << "[1] Random event...";
        *currEvent = WebServiceEvent::proRandom(t, runningActivities, finishedActivities,
                                                wsf->activities);
        qDebug() << " Event:" << currEvent->toString();

        // [2]
        qDebug() << "[2] Cause bad state...";
        WebServiceAtomState state;
        state.activityId = currEvent->a;
        state.stateType = currEvent->type;
        qDebug() << " State:" << state.toString();

        // [3]
        qDebug() << "[3] Find solutions...";
        markovResult = wsr->getMarkovResult(state);

        // [4]
        qDebug() << "[4] GetBestMarkovResult..." ;
        MarkovResultItem* currItem = getBestMarkovResult();
        currAction = &(currItem->action);
        if (currItem == NULL)
        {
            qDebug() << "getBestMarkovResult() failed.";
            break;
        }
        qDebug() << " Action:" << currAction->toString();

        // [5]
        qDebug() << "[5] Recovery with action...";
        wsr->recovery(currAction, 1);
        if (wsf->isFinished())
        {
            qDebug() << "[5.1] is finished. break.";
            break;
        }

        // [7]
        bugActivities.clear();

        // [9]
        qDebug() << "[6] One second passed...";
        timePassed();

        eventList.append(*currEvent);
        markovItemList.append(*currItem);

        t++;
    }

    QList<int> timeList;
    QList<int> rewardList;
    QList<int> costList;
    QList<int> dtimeList;
    for (int i = 0; i < eventList.size(); i++)
    {
        timeList.append(eventList[i].t);
        rewardList.append(markovItemList[i].directReward);
        costList.append(markovItemList[i].action.dc);
        dtimeList.append(markovItemList[i].action.dt);
    }
    for (int i = 1; i < eventList.size(); i++)
    {
        rewardList[i] += rewardList[i-1];
        costList[i] += costList[i-1];
        dtimeList[i] += dtimeList[i-1];
    }
    QString timeCmd = "t1 = [";
    QString rewardCmd = "r1 = [";
    QString costCmd = "c1 = [";
    QString dtimeCmd = "dt1 = [";
    for (int i = 1; i < eventList.size(); i++)
    {
        timeCmd += QString("%1 ").arg(timeList[i]);
        rewardCmd += QString("%1 ").arg(rewardList[i]);
        costCmd += QString("%1 ").arg(costList[i]);
        dtimeCmd += QString("%1 ").arg(dtimeList[i]);
    }
    timeCmd = timeCmd.trimmed() + "];";
    rewardCmd = rewardCmd.trimmed() + "];";
    costCmd = costCmd.trimmed() + "];";
    dtimeCmd = dtimeCmd.trimmed() + "];";

//    for (int i = 0; i < eventList.size(); i++)
//    {
//        qDebug() << markovItemList[i].action.name();
//    }
    qDebug() << "Finished state:" << wsf->name();
    QString cmd = QString("%1 %2 %3 %4")
            .arg(timeCmd)
            .arg(rewardCmd)
            .arg(costCmd)
            .arg(dtimeCmd);
    return cmd;
}

QString WebServiceSimulation::autoGreedyRun()
{
    runningActivities.clear();
    finishedActivities.clear();
    bugActivities.clear();

    runningActivities.insert(0);

    QList<WebServiceEvent> eventList;
    QList<MarkovResultItem> markovItemList;

    int t = 0;
    qDebug() << t << isFinished() ;
    while (!isStop && !isFinished())
    {
        printCurrState(t);

        // [1] event
        qDebug() << "[1] Random event...";
        *currEvent = WebServiceEvent::proRandom(t, runningActivities, finishedActivities,
                                                wsf->activities);
        qDebug() << " Event:" << currEvent->toString();

        // [2]
        qDebug() << "[2] Cause bad state...";
        WebServiceAtomState state;
        state.activityId = currEvent->a;
        state.stateType = currEvent->type;
        qDebug() << " State:" << state.toString();

        // [3]
        qDebug() << "[3] Find solutions...";
        markovResult = wsr->getMarkovResult(state);

        // [4]
        qDebug() << "[4] GetBestGreedyResult..." ;
        MarkovResultItem* currItem = getBestGreedyResult();
        currAction = &(currItem->action);
        if (currItem == NULL)
        {
            qDebug() << "GetBestGreedyResult() failed.";
            break;
        }
        qDebug() << " Action:" << currAction->toString();

        // [5]
        qDebug() << "[5] Recovery with action...";
        wsr->recovery(currAction, 1);
        if (wsf->isFinished())
        {
            qDebug() << "[5.1] is finished. break.";
            break;
        }

        // [7]
        bugActivities.clear();

        // [9]
        qDebug() << "[6] One second passed...";
        timePassed();

        eventList.append(*currEvent);
        markovItemList.append(*currItem);

        t++;
    }

    QList<int> timeList;
    QList<int> rewardList;
    QList<int> costList;
    QList<int> dtimeList;
    for (int i = 0; i < eventList.size(); i++)
    {
        timeList.append(eventList[i].t);
        rewardList.append(markovItemList[i].directReward);
        costList.append(markovItemList[i].action.dc);
        dtimeList.append(markovItemList[i].action.dt);
    }
    for (int i = 1; i < eventList.size(); i++)
    {
        rewardList[i] += rewardList[i-1];
        costList[i] += costList[i-1];
        dtimeList[i] += dtimeList[i-1];
    }
    QString timeCmd = "t2 = [";
    QString rewardCmd = "r2 = [";
    QString costCmd = "c2 = [";
    QString dtimeCmd = "dt2 = [";
    for (int i = 1; i < eventList.size(); i++)
    {
        timeCmd += QString("%1 ").arg(timeList[i]);
        rewardCmd += QString("%1 ").arg(rewardList[i]);
        costCmd += QString("%1 ").arg(costList[i]);
        dtimeCmd += QString("%1 ").arg(dtimeList[i]);
    }
    timeCmd = timeCmd.trimmed() + "];";
    rewardCmd = rewardCmd.trimmed() + "];";
    costCmd = costCmd.trimmed() + "];";
    dtimeCmd = dtimeCmd.trimmed() + "];";

//    for (int i = 0; i < eventList.size(); i++)
//    {
//        qDebug() << markovItemList[i].action.name();
//    }
    qDebug() << "Finished state:" << wsf->name();
    QString cmd = QString("%1 %2 %3 %4")
            .arg(timeCmd)
            .arg(rewardCmd)
            .arg(costCmd)
            .arg(dtimeCmd);
    return cmd;
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
        eventHistoryItem->validResultList = markovResult;
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
        if (selectActionId < 0 || selectActionId >= markovResult.size())
        {
            qDebug() << "assert failed. selectActionId >=0 && selectActionId < markovResult.size()";
            break;
        }
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
        eventHistoryItem->validResultList = markovResult;
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

MarkovResultItem* WebServiceSimulation::getBestGreedyResult()
{
    if (markovResult.isEmpty())
        return NULL;

    MarkovResultItem * res = NULL;
    for (int i = 0; i < markovResult.size(); i++)
    {
        if (res == NULL || res->directReward < markovResult[i].directReward)
        {
            res = &markovResult[i];
        }
    }

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

//void WebServiceSimulation::setAutoRun(bool _isAutoRun)
//{
//    isAutoRun = _isAutoRun;
//}

//void WebServiceSimulation::setMatlabRun(bool _isMatlabRun)
//{
//    isMatlabRun = _isMatlabRun;
//}

void WebServiceSimulation::setRunType(int _runType)
{
    runType = _runType;
}


void WebServiceSimulation::setSelectActionId(int _selectActionId)
{
    selectActionId = _selectActionId;
}

MarkovResultItem* WebServiceSimulation::getSelectItem()
{
    if (selectActionId < markovResult.size() && selectActionId >= 0)
    {
        return  &markovResult[selectActionId];
    }
    return NULL;
}

void WebServiceSimulation::setSleepMSecond(int _sleepMSecond)
{
    sleepMSecond = _sleepMSecond;
}

QString WebServiceSimulation::getMatlabCmd()
{
    return matlabCmd;
}

WebServiceEventWidget* WebServiceSimulation::getWebServiceEventWidget()
{
    return wsew;
}
