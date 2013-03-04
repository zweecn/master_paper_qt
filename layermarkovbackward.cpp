#include "layermarkovbackward.h"
#include "markovrecord.h"
#include "markov.h"
#include "climits"

#include <ctime>

uint qHash(const TAndState & key)
{
    return qHash(key.t) ^ qHash(key.state);
}

uint qHash(const StateTAndAction & key)
{
    return qHash(key.t) ^ qHash(key.state) ^ qHash(key.action);
}

uint qHash(const ToStateInfo & key)
{
    return key.time * key.price * key.posibility * qHash(key.state);
}

LayerMarkovBackward::LayerMarkovBackward(MarkovState& _state)
{
    state = _state;
    greedyState = _state;
    state.init();
    greedyState.init();
    init();
}

void LayerMarkovBackward::init(int reduceLayerSize)
{
    if (allLayerRecords.isEmpty())
    {
        generateLayerRecords();
    }
    reduceLayer(reduceLayerSize);
    extendTree(IS_EXTEND_TREE);
    initMap();
    initMarkovInfo();
}

void LayerMarkovBackward::init()
{
    time_t t1 = clock();
    generateLayerRecords();
    reduceLayer(REDUCE_LAYER_SIZE);
    extendTree(IS_EXTEND_TREE);
    initMap();
    time_t t2 = clock();
    initMarkovInfo();
    time_t t3 = clock();
    generateRecordRunTime = t2 - t1;
    initMarkovInfoRunTime = t3 - t2;
}

void LayerMarkovBackward::initGreedy()
{
    if (allLayerRecords.isEmpty())
    {
        generateLayerRecords();
    }
    reduceLayer(1);
    extendTree(IS_EXTEND_TREE);
    initMap();
    initMarkovInfo();
}

void LayerMarkovBackward::initMap()
{
    for (int t = 0; t < allLayerRecords.size(); t++) {
        addToMap(t, allLayerRecords[t]);
    }
    QSet<MarkovState> tempSet;
    tempSet.insert((allLayerRecords[0][0].stateBefore));
    t2StateMap.insert(0, tempSet);
}

void LayerMarkovBackward::generateLayerRecords()
{
    queue2.enqueue(state);
    QSet<MarkovState> stateSet;
    for (;;)
    {
        QSet<MarkovState> tempSet;
        for (int i = 0; i < queue2.size(); i++)
        {
            tempSet.insert(queue2[i]);
        }
        queue1 = queue2;
        queue2.clear();
        QList<MarkovRecord> oneLayerRecords;
        while (!queue1.isEmpty())
        {
            state = queue1.dequeue();
            if (!stateSet.contains(state))
            {
                stateSet.insert(state);
                QList<MarkovRecord> records = Markov::getRecords(state);
                addToRecords(oneLayerRecords, records);
            }
        }
        if (oneLayerRecords.isEmpty())
        {
            break;
        }
        allLayerRecords.append(oneLayerRecords);
    }
    if (allLayerRecords.size() > MarkovRecord::getMaxLayerSize())
    {
        MarkovRecord::setMaxLayerSize(allLayerRecords.size());
    }
}

void LayerMarkovBackward::extendTree(bool isExtend)
{
    if (!isExtend)
    {
        return;
    }
    for (int i = 0; i < allLayerRecords.size()-1; i++) {
        QSet<int> frontLayerStateAfterSet;
        for (int j = 0; j < allLayerRecords[i].size(); j++)
        {
            MarkovRecord & rd = allLayerRecords[i][j];
            frontLayerStateAfterSet.insert(rd.stateAfter.id);
        }
        QSet<int> nextLayerStateBeforeSet;
        for (int j = 0; j < allLayerRecords[i+1].size(); j++)
        {
            MarkovRecord & rd = allLayerRecords[i][j];
            nextLayerStateBeforeSet.insert(rd.stateBefore.id);
        }
        frontLayerStateAfterSet -= nextLayerStateBeforeSet;
        if (!frontLayerStateAfterSet.isEmpty())
        {
            for (int j = 0; j < allLayerRecords[i].size(); j++)
            {
                //                MarkovRecord  rd;
                //                rd = allLayerRecords[i][j];
                if (frontLayerStateAfterSet.contains(allLayerRecords[i][j].stateBefore.id))
                {
                    allLayerRecords[i+1].append(allLayerRecords[i][j]);
                }
            }
        }
    }
}

void LayerMarkovBackward::reduceLayer(int i)
{
    if (i > 0 && i < allLayerRecords.size())
    {
        while (allLayerRecords.size() > i + 1)
        {
            allLayerRecords.removeLast();
        }
    }
}

void LayerMarkovBackward::addToRecords(QList<MarkovRecord> &destRecords, QList<MarkovRecord> &sourceRecord)
{
    if (!sourceRecord.isEmpty())
    {
        destRecords += sourceRecord;
        for (int i = 0; i < sourceRecord.size(); i++)
        {
            MarkovRecord &rd = sourceRecord[i];
            queue2.enqueue(rd.stateAfter);
        }
    }
}

void LayerMarkovBackward::addToMap(int t, QList<MarkovRecord> & records)
{
    QSet<MarkovState> tempSet;
    for (int i = 0; i < records.size(); i++)
    {
        MarkovRecord &rd = records[i];
        TAndState tsb(t, rd.stateBefore);
        if (! tState2ChildActionMap.contains(tsb)) {
            tState2ChildActionMap.insert(tsb, QList<MarkovAction>());
        }
        tState2ChildActionMap[tsb].append(rd.action);
        StateTAndAction sta(rd.stateBefore, t, rd.action);
        if (! stateTAction2ChildStateInfoMap.contains(sta)) {
            stateTAction2ChildStateInfoMap.insert(sta, QList<ToStateInfo>());
        }
        ToStateInfo info(rd.stateAfter, rd.posibility, rd.priceCost, rd.timeCost);
        stateTAction2ChildStateInfoMap[sta].append(info); //Mark

        tempSet.insert(rd.stateAfter);
        if (! t2StateMap.contains(t+1)) {
            t2StateMap.insert(t+1, tempSet);
        } else {
            t2StateMap[t+1] += tempSet;
        }
    }
}

QString LayerMarkovBackward::makeStepString(int t, MarkovAction & action, double u)
{
    QString res = QString("At t=%1 Action=%2 utility=%3").arg(t).arg(action.name()).arg(u);
    return res;
}

int LayerMarkovBackward::getTsize()
{
    return allLayerRecords.size() + 1;
}

bool LayerMarkovBackward::hasChildren(int t, MarkovState & state)
{
    TAndState ts(t, state);
    return (tState2ChildActionMap.contains(ts));
}

void LayerMarkovBackward::initMarkovInfo()
{
    utility = new double*[getTsize()];
    for (int i = 0; i < getTsize(); i++)
    {
        utility[i] = new double[MarkovRecord::getStateSize()];
    }
    step = new QString[getTsize() - 1];
    for (int i = 0; i < getTsize(); i++)
    {
        for (int j = 0; j < MarkovRecord::getStateSize(); j++)
        {
            utility[i][j] = - INT_MAX;
        }
    }
    for (int t = getTsize()-1; t >= 0; t--)
    {
        QSetIterator<MarkovState> it(t2StateMap[t]);
        while (it.hasNext())
        {
            MarkovState i;
            i = it.next();
            if (!hasChildren(t, i))
            {
                utility[t][i.id] = getNReward(t, i);
            }
            else
            {
                utility[t][i.id] = -INT_MAX;
            }
        }
    }
}

void LayerMarkovBackward::runMarkov()
{
    time_t t1 = clock();
    for (int t = getTsize()-2; t >= 0; t--)
    {
        QSetIterator<MarkovState> it(t2StateMap[t]);
        while (it.hasNext())
        {
            MarkovState i;
            i = it.next();
            if (hasChildren(t, i))
            {
                utility[t][i.id] = maxUtility(t, i);
            }
        }
    }

    if (stateNew.id != -1) {
        if (firstAction.type == MarkovAction::A_TERMINATE) {
            stateNew.nextToDoActivity->x = (-1);
        }
        stateNew.init();
    }
    runMarkovRunTime = clock() - t1;
}

double LayerMarkovBackward::maxUtility(int t, MarkovState & i)
{
    TAndState ts(t, i);
    double u = - INT_MAX;
    QList<MarkovAction> & aList = tState2ChildActionMap[ts];
    for (int k = 0; k < aList.size(); k++)
    {
        MarkovAction & a = aList[k];
        StateTAndAction sta(i, t, a);
        double reward = getTReward(sta, stateTAction2ChildStateInfoMap[sta]);

        QList<ToStateInfo> & tsiList = stateTAction2ChildStateInfoMap[sta];
        for (int j = 0; j < tsiList.size(); j++)
        {
            ToStateInfo &tsi = tsiList[j];
            reward +=  WEAKEN * tsi.getPosibility() * utility[t+1][tsi.state.id];
        }
        if (u < reward) {
            u = reward;
            step[t] = makeStepString(t, a, u);
            actionCost = stateTAction2ChildStateInfoMap[sta][0].getPrice();
            actionTimeCost = stateTAction2ChildStateInfoMap[sta][0].getTime();
            //				posibility = stateTAction2ChildStateInfoMap.get(sta).get(0).getPosibility();
            firstAction = a;
            stateNew = stateTAction2ChildStateInfoMap[sta][0].getState();
        }
    }
    return u;
}


/*
 * This is the reward of terminate state (Leaf of the UTG tree).
 * */
double LayerMarkovBackward::getNReward(int t, MarkovState & state)
{
    if (state.globalState == MarkovState::S_FAILED)
    {
        return (- PUNISHMENT_FAILED);
    }
    return 0;
}

/*
 * This is the reward after do a action.
 * */
double LayerMarkovBackward::getTReward(StateTAndAction & sta, QList<ToStateInfo> & tsi)
{
    double res = 0;
    if (!tsi.isEmpty() && tsi[0].state.globalState == MarkovState::S_FAILED)
    {
        res = - (tsi[0].getPosibility()) * PUNISHMENT_FAILED / getTsize()
                - tsi[0].getPrice()
                - tsi[0].getTime() * PUNISHMENT_PER_SECOND;
    }
    else if (!tsi.isEmpty() && tsi[0].state.globalState != MarkovState::S_FAILED)
    {
        res = - (1-tsi[0].getPosibility()) * PUNISHMENT_FAILED / getTsize()
                - tsi[0].getPrice()
                - tsi[0].getTime() * PUNISHMENT_PER_SECOND;
    }
    return  res; //- (1-tsi.get(0).getPosibility()) * Configs.FAILED_PUNISHMENT
}

// END MARKOV

double LayerMarkovBackward::getMarkovBestUtility()
{
    return utility[0][0];
}

double LayerMarkovBackward::getCurrActionCost()
{
    return actionCost;
}

double LayerMarkovBackward::getCurrActionTimeCost()
{
    return actionTimeCost;
}

double LayerMarkovBackward::getCurrActionReward()
{
    double res = //- (1-posibility) * Configs.PUNISHMENT_FAILED
            - actionCost
            - actionTimeCost * PUNISHMENT_PER_SECOND;
    if (getAction().type == MarkovAction::A_TERMINATE)
    {
        res -= PUNISHMENT_FAILED;
    }
    return res;
}

MarkovAction & LayerMarkovBackward::getAction()
{
    return firstAction;
}

MarkovState & LayerMarkovBackward::getStateNew()
{
    return stateNew;
}

//NEXT IS GREEDY

void LayerMarkovBackward::runGreedy()
{
    initGreedy();
    runMarkov();
    greedyReward = getCurrActionReward();
    greedyPriceCost = getCurrActionCost();
    greedyTimeCost = getCurrActionTimeCost();
    greedyAction = getAction();
    greedyStateNew = getStateNew();
}

MarkovAction & LayerMarkovBackward::getGreedyAction()
{
    return greedyAction;
}

double LayerMarkovBackward::getGreedyPriceCost()
{
    return greedyPriceCost;
}

double LayerMarkovBackward::getGreedyTimeCost()
{
    return greedyTimeCost;
}

double LayerMarkovBackward::getGreedyPosibility()
{
    return greedyPosibility;
}

MarkovState & LayerMarkovBackward::getGreedyStateNew()
{
    return greedyStateNew;
}

double LayerMarkovBackward::getGreedyActionReward()
{
    double temp = 0;
    if (greedyAction.type == MarkovAction::A_TERMINATE)
    {
        temp = PUNISHMENT_FAILED;
    }
    return greedyReward - temp;
}
