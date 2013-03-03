#include "layermarkovbackward.h"
#include "markovrecord.h"

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
//    init();
}

//void LayerMarkovBackward::init(int reduceLayerSize)
//{
//    if (allLayerRecords.isEmpty())
//    {
//        generateLayerRecords();
//    }
//    reduceLayer(reduceLayerSize);
//    extendTree(IS_EXTEND_TREE);
//    initMap();
//    initMarkovInfo();
//}

//void LayerMarkovBackward::init() {
//    time_t t1 = clock();
//    generateLayerRecords();
//    reduceLayer(REDUCE_LAYER_SIZE);
//    extendTree(IS_EXTEND_TREE);
//    initMap();
//    time_t t2 = clock();
//    initMarkovInfo();
//    time_t t3 = clock();
//    generateRecordRunTime = t2 - t1;
//    initMarkovInfoRunTime = t3 - t2;
//}

//void LayerMarkovBackward::initGreedy()
//{
//    if (allLayerRecords.isEmpty())
//    {
//        generateLayerRecords();
//    }
//    reduceLayer(1);
//    extendTree(IS_EXTEND_TREE);
//    initMap();
//    initMarkovInfo();
//}

//void LayerMarkovBackward::initMap()
//{
//    for (int t = 0; t < allLayerRecords.size(); t++) {
//        addToMap(t, allLayerRecords[t]);
//    }
//    QSet<MarkovState> tempSet;
//    tempSet.insert((allLayerRecords[0][0].getStateBefore()));
//    t2StateMap.insert(0, tempSet);
//}

//void LayerMarkovBackward::generateLayerRecords()
//{
//    queue2.enqueue(state);
//    QSet<MarkovState> stateSet;
//    for (;;)
//    {
//        QSet<MarkovState> tempSet;
//        foreach (s, queue2)
//        {
//            tempSet.insert(s);
//        }
//        queue1 = queue2;
//        queue2.clear();
//        QList<MarkovRecord> oneLayerRecords;
//        while (!queue1.isEmpty())
//        {
//            state = queue1.dequeue();
//            if (!stateSet.contains(state))
//            {
//                stateSet.insert(state);
//                QList<MarkovRecord> records = Markov::getRecords(state);
//                addToRecords(oneLayerRecords, records);
//            }
//        }
//        if (oneLayerRecords.isEmpty())
//        {
//            break;
//        }
//        allLayerRecords.append(oneLayerRecords);
//    }
//    if (allLayerRecords.size() > MarkovRecord.getMaxLayerSize())
//    {
//        MarkovRecord.setMaxLayerSize(allLayerRecords.size());
//    }
//}

//void extendTree(bool isExtend)
//{
//    if (!isExtend) {
//        return;
//    }
//    for (int i = 0; i < allLayerRecords.size()-1; i++) {
//        QSet<int> frontLayerStateAfterSet;
//        foreach (rd , allLayerRecords[i]) {
//            frontLayerStateAfterSet.insert(rd.getStateAfter().getId());
//        }
//        QSet<int> nextLayerStateBeforeSet;
//        foreach (rd , allLayerRecords[i+1]) {
//            nextLayerStateBeforeSet.add(rd.getStateBefore().getId());
//        }
//        frontLayerStateAfterSet.removeAll(nextLayerStateBeforeSet);
//        if (!frontLayerStateAfterSet.isEmpty()) {
//            foreach (rd , allLayerRecords[i]) {
//                if (frontLayerStateAfterSet.contains(rd.getStateBefore().getId())) {
//                    allLayerRecords.get(i+1).add(rd);
//                }
//            }
//        }
//    }
//}

//void reduceLayer(int i)
//{
//    if (i > 0 && i < allLayerRecords.size()) {
//        while (allLayerRecords.size() > i + 1) {
//            allLayerRecords.remove(allLayerRecords.size()-1);
//        }
//    }
//}

//void addToRecords(List<MarkovRecord>destRecords, List<MarkovRecord> sourceRecord){
//    if (sourceRecord != null && !sourceRecord.isEmpty() && destRecords != null) {
//        destRecords.addAll(sourceRecord);
//        Set<MarkovAction> actionSetTemp = new HashSet<MarkovAction>();
//        foreach (rd , sourceRecord) {
//            queue2.offer(rd.getStateAfter());
//            actionSetTemp.add(rd.getAction());
//        }
//    }
//}

//void addToMap(int t, List<MarkovRecord> records) {
//    Set<MarkovState> tempSet = new HashSet<MarkovState>();
//    foreach (rd , records) {
//        TAndState tsb = new TAndState(t, rd.getStateBefore());
//        if (tState2ChildActionMap.get(tsb) == null) {
//            tState2ChildActionMap.put(tsb, new ArrayList<MarkovAction>());
//        }
//        tState2ChildActionMap.get(tsb).add(rd.getAction());
//        StateTAndAction sta = new StateTAndAction(rd.getStateBefore(), t, rd.getAction());
//        if (stateTAction2ChildStateInfoMap.get(sta) == null) {
//            stateTAction2ChildStateInfoMap.put(sta, new ArrayList<LayerMarkovBackward.ToStateInfo>());
//        }
//        ToStateInfo info = new ToStateInfo(rd.getStateAfter(), rd.getPosibility(), rd.getPriceCost(), rd.getTimeCost());
//        stateTAction2ChildStateInfoMap.get(sta).add(info); //Mark

//        tempSet.add(rd.getStateAfter());
//        if (t2StateMap.get(t+1) == null) {
//            t2StateMap.put(t+1, tempSet);
//        } else {
//            t2StateMap.get(t+1).addAll(tempSet);
//        }
//    }
//}


//String makeStepString(int t, MarkovAction action, double u) {
//    String res = "At t=" + String.format("%2d", t) + " Action=" + action + " utility=" + String.format("%.2f", u);
//    return res;
//}

//int getTsize() {
//    return allLayerRecords.size() + 1;
//}

//bool hasChildren(int t, MarkovState state) {
//    TAndState ts = new TAndState(t, state);
//    return (tState2ChildActionMap.get(ts) != null);
//}

