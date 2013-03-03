#include "markov.h"
#include "markovrecord.h"
#include <QDebug>

Markov::Markov()
{
}

QList<MarkovRecord> Markov::noActionRecords(MarkovState & state)
{
    QList<MarkovRecord> records;
    MarkovState stateAfter = state;
    double timeCost = 0;
    if (state.globalState != MarkovState::S_NORMAL
            && state.globalState != MarkovState::S_SUCCEED
            && (state.faultActivity->x > 1.0))
    {
        timeCost = (state.faultActivity->x - 1.0)
                * state.faultActivity->blindService->execTime;
    }
    MarkovAction noac = MarkovAction::noAction(stateAfter);
    //stateAfter.init();
    if (noac.type == MarkovAction::ERROR_ACTION || stateAfter.globalState == MarkovState::S_SUCCEED
            || stateAfter.isFinished() || MarkovRecord::hasStateAction(stateAfter, noac))
    {
        return records;
    }

    if (stateAfter.globalState == MarkovState::S_FAILED)
    {
        if (!MarkovRecord::hasState(stateAfter)) {
            MarkovRecord::addState(stateAfter);
        } else {
            MarkovState *tmps = MarkovRecord::getState(stateAfter);
            if (tmps == NULL) {
                qDebug() << "MarkovAction::noActionRecords(MarkovState & state). tmpa==NULL";
            } else {
                stateAfter = (*tmps);
            }
        }
        if (!MarkovRecord::hasAction(noac)) {
            MarkovRecord::addAction(noac);
        } else {
            MarkovAction *tmpa = MarkovRecord::getAction(noac);
            if (tmpa == NULL) {
                qDebug() << "MarkovAction::noActionRecords(MarkovState & state). tmps==NULL";
            } else {
                noac = (*tmpa);
            }
        }
        records.append(MarkovRecord(stateAfter, stateAfter, noac, 1, 0, timeCost));
        return records;
    }
    else
    {
        QList<MarkovState> states = stateAfter.getNextTwoStates();
        MarkovState stateAfter1 = states[0];
        MarkovState stateAfter2 = states[1];
        if (!MarkovRecord::hasState(stateAfter1)) {
            MarkovRecord::addState(stateAfter1);
        } else {
            MarkovState *tmps = MarkovRecord::getState(stateAfter1);
            if (tmps == NULL) {
                qDebug() << "MarkovAction::noActionRecords(MarkovState & state). tmpa==NULL";
            } else {
                stateAfter1 = (*tmps);
            }
        }
        if (!MarkovRecord::hasState(stateAfter2)) {
            MarkovRecord::addState(stateAfter2);
        } else {
            MarkovState *tmps = MarkovRecord::getState(stateAfter2);
            if (tmps == NULL) {
                qDebug() << "MarkovAction::noActionRecords(MarkovState & state). tmpa==NULL";
            } else {
                stateAfter2 = (*tmps);
            }
        }

        if (!MarkovRecord::hasAction(noac)) {
            MarkovRecord::addAction(noac);
        } else {
            MarkovAction *tmpa = MarkovRecord::getAction(noac);
            if (tmpa == NULL) {
                qDebug() << "MarkovAction::noActionRecords(MarkovState & state). tmps==NULL";
            } else {
                noac = (*tmpa);
            }
        }
        records.append(MarkovRecord(state, stateAfter1, noac,
                                 stateAfter.nextToDoActivity->blindService->reliability, 0, timeCost));
        records.append(MarkovRecord(state, stateAfter2, noac,
                                 1 - stateAfter.nextToDoActivity->blindService->reliability, 0, timeCost));

        return records;
    }
}


//List<MarkovRecord> terminateRecords(MarkovState state) {
//    double timeCost = 0;
//    if (state.getGlobalState() != Markov.S_NORMAL && state.getGlobalState() != Markov.S_SUCCEED
//            && (state.getFaultActivity().getX()-1) > 0) {
//        timeCost = (state.getFaultActivity().getX()-1)
//                * state.getFaultActivity().getBlindService().getQos().getExecTime();
//    }
//    MarkovState stateAfter = state.clone();
//    BaseAction terminateAction = (BaseAction) Markov.terminate(stateAfter);
//    if (stateAfter == NULL || stateAfter.isFinished() || MarkovRecord.hasStateAction(stateAfter, terminateAction)) {
//        return NULL;
//    }

//    List<MarkovRecord> records = new ArrayList<MarkovRecord>();
//    if (MarkovRecord.hasState(stateAfter)) {
//        stateAfter = MarkovRecord.getState(stateAfter);
//    } else {
//        MarkovRecord.addState(stateAfter);
//    }
//    if (!MarkovRecord.hasAction(terminateAction)) {
//        MarkovRecord.addAction(terminateAction);
//    } else {
//        terminateAction = (BaseAction) MarkovRecord.getAction(terminateAction);
//    }
//    records.add(new MarkovRecord(state, stateAfter, terminateAction, 1, 0, timeCost));
//    return records;
//}

////在action里面加上消耗和报酬
//List<MarkovRecord> redoRecords(MarkovState state) {
//    MarkovState stateAfter = state.clone();
//    BaseAction redoAction = (BaseAction) Markov.redo(stateAfter);
//    if (stateAfter == NULL || stateAfter.isFinished() || MarkovRecord.hasStateAction(stateAfter, redoAction)) {
//        return NULL;
//    }
//    List<MarkovRecord> records = new ArrayList<MarkovRecord>();
//    MarkovState[] states = stateAfter.getNextTwoStates();

//    MarkovState stateAfter1 = states[0];
//    MarkovState stateAfter2 = states[1];

//    if (!MarkovRecord.hasState(stateAfter1)) {
//        MarkovRecord.addState(stateAfter1);
//    } else {
//        stateAfter1 = MarkovRecord.getState(stateAfter1);
//    }
//    if (!MarkovRecord.hasState(stateAfter2)) {
//        MarkovRecord.addState(stateAfter2);
//    } else {
//        stateAfter2 = MarkovRecord.getState(stateAfter2);
//    }
//    if (!MarkovRecord.hasAction(redoAction)) {
//        MarkovRecord.addAction(redoAction);
//    } else {
//        redoAction = (BaseAction) MarkovRecord.getAction(redoAction);
//    }

//    records.add(new MarkovRecord(state, stateAfter1, redoAction,
//                                 state.getFaultActivity().getBlindService().getQos().getReliability(),
//                                 state.getFaultActivity().getBlindService().getQos().getPrice(),
//                                 Math.abs(state.getFaultActivity().getBlindService().getQos().getExecTime()*state.getFaultActivity().getX())));
//    records.add(new MarkovRecord(state, stateAfter2, redoAction,
//                                 1 - state.getFaultActivity().getBlindService().getQos().getReliability(),
//                                 state.getFaultActivity().getBlindService().getQos().getPrice(),
//                                 Math.abs(state.getFaultActivity().getBlindService().getQos().getExecTime()*state.getFaultActivity().getX())));
//    return records;
//}



//List<MarkovRecord> replaceRecords(MarkovState state) {
//    MarkovState stateAfter = state.clone();

//    ReplaceAction replaceAction = (ReplaceAction) Markov.replace(stateAfter);

//    if (replaceAction == NULL || stateAfter == NULL || stateAfter.isFinished() || MarkovRecord.hasStateAction(stateAfter, replaceAction)) {
//        return NULL;
//    }

//    List<MarkovRecord> records = new ArrayList<MarkovRecord>();

//    MarkovState[] states = stateAfter.getNextTwoStates();
//    if (states == NULL || states.length < 2) {
//        return NULL;
//    }

//    MarkovState stateAfter1 = states[0];
//    MarkovState stateAfter2 = states[1];
//    if (!MarkovRecord.hasState(stateAfter1)) {
//        MarkovRecord.addState(stateAfter1);
//    } else {
//        stateAfter1 = MarkovRecord.getState(stateAfter1);
//    }
//    if (!MarkovRecord.hasState(stateAfter2)) {
//        MarkovRecord.addState(stateAfter2);
//    } else {
//        stateAfter2 = MarkovRecord.getState(stateAfter2);
//    }
//    if (!MarkovRecord.hasAction(replaceAction)) {
//        MarkovRecord.addAction(replaceAction);
//    } else {
//        replaceAction = (ReplaceAction) MarkovRecord.getAction(replaceAction);
//    }



//    records.add(new MarkovRecord(state, stateAfter1, replaceAction, replaceAction.getPosibility(),
//                                 replaceAction.getPriceCost(), replaceAction.getTimeCost()));
//    records.add(new MarkovRecord(state, stateAfter2, replaceAction, 1 - replaceAction.getPosibility(),
//                                 replaceAction.getPriceCost(), replaceAction.getTimeCost()));


//    return records;


//}

//List<MarkovRecord> reCompositeRecords(MarkovState state) {
//    MarkovState stateAfter = state.clone();
//    if (reCompositeAction == NULL || stateAfter.isFinished() || MarkovRecord.hasStateAction(stateAfter, reCompositeAction)) {
//        return NULL;
//    }

//    List<MarkovRecord> records = new ArrayList<MarkovRecord>();

//    MarkovState[] states = stateAfter.getNextTwoStates();

//    if (states == NULL || states.length < 2) {
//        return NULL;
//    }
//    ReCompositorImpl reCompositor = ((ReCompositorImpl) reCompositeAction.getReCompositor());

//    MarkovState stateAfter1 = states[0];
//    MarkovState stateAfter2 = states[1];
//    if (!MarkovRecord.hasState(stateAfter1)) {
//        MarkovRecord.addState(stateAfter1);
//    } else {
//        stateAfter1 = MarkovRecord.getState(stateAfter1);
//    }
//    if (!MarkovRecord.hasState(stateAfter2)) {
//        MarkovRecord.addState(stateAfter2);
//    } else {
//        stateAfter2 = MarkovRecord.getState(stateAfter2);
//    }
//    if (!MarkovRecord.hasAction(reCompositeAction)) {
//        MarkovRecord.addAction(reCompositeAction);
//    } else {
//        reCompositeAction = (ReCompositeAction) MarkovRecord.getAction(reCompositeAction);
//    }


//    records.add(new MarkovRecord(state, stateAfter1, reCompositeAction, reCompositor.getPosibility(),
//                                 reCompositor.getPriceCost(), reCompositor.getTimeCost()));
//    records.add(new MarkovRecord(state, stateAfter2, reCompositeAction, 1 - reCompositor.getPosibility(),
//                                 reCompositor.getPriceCost(), reCompositor.getTimeCost()));

//    return records;
//}

//List<MarkovRecord> getRecords(MarkovState state) {

//    if (MarkovRecord.hasStateBefore(state)) {
//        return NULL;
//    }
//    List<MarkovRecord> resultRecords = new ArrayList<MarkovRecord>();

//    if (state.getGlobalState() == Markov.S_NORMAL ) {
//        List<MarkovRecord> tempRecords = Markov.noActionRecords(state);

//        if (tempRecords != NULL && !tempRecords.isEmpty()) {
//            resultRecords.addAll(tempRecords);
//        }
//    } else if (state.getGlobalState() == Markov.S_DELAYED) {

//        List<MarkovRecord> tempRecords = Markov.noActionRecords(state);
//        if (tempRecords != NULL && !tempRecords.isEmpty()) {
//            resultRecords.addAll(tempRecords);
//        }
//        tempRecords = Markov.terminateRecords(state);
//        if (tempRecords != NULL && !tempRecords.isEmpty()) {
//            resultRecords.addAll(tempRecords);
//        }
//        tempRecords = Markov.replaceRecords(state);
//        if (tempRecords != NULL && !tempRecords.isEmpty()) {
//            resultRecords.addAll(tempRecords);
//        }
//        tempRecords = Markov.reCompositeRecords(state);
//        if (tempRecords != NULL && !tempRecords.isEmpty()) {
//            resultRecords.addAll(tempRecords);
//        }
//    } else if (state.getGlobalState() == Markov.S_PRICE_UP) {
//        List<MarkovRecord> tempRecords = Markov.noActionRecords(state);
//        if (tempRecords != NULL && !tempRecords.isEmpty()) {
//            resultRecords.addAll(tempRecords);
//        }
//        tempRecords = Markov.terminateRecords(state);
//        if (tempRecords != NULL && !tempRecords.isEmpty()) {
//            resultRecords.addAll(tempRecords);
//        }
//        //System.out.println("state1=" + state);
//        tempRecords = Markov.replaceRecords(state);
//        //System.out.println("state2=" + state.getFaultActivity());
//        if (tempRecords != NULL && !tempRecords.isEmpty()) {
//            resultRecords.addAll(tempRecords);
//        }
//        tempRecords = Markov.reCompositeRecords(state);
//        if (tempRecords != NULL && !tempRecords.isEmpty()) {
//            resultRecords.addAll(tempRecords);
//        }
//    } else if (state.getGlobalState() == Markov.S_SUCCEED) {
//        List<MarkovRecord> tempRecords = Markov.noActionRecords(state);
//        if (tempRecords != NULL && !tempRecords.isEmpty()) {
//            resultRecords.addAll(tempRecords);
//        }
//    } else if (state.getGlobalState() == Markov.S_FAILED) {
//        List<MarkovRecord> tempRecords = Markov.terminateRecords(state);
//        if (tempRecords != NULL && !tempRecords.isEmpty()) {
//            resultRecords.addAll(tempRecords);
//        }
//        tempRecords = Markov.redoRecords(state);
//        //	System.out.println("Redo:" + tempRecords);
//        if (tempRecords != NULL && !tempRecords.isEmpty()) {
//            resultRecords.addAll(tempRecords);
//        }
//        //System.out.println("state=" + state);
//        tempRecords = Markov.replaceRecords(state);
//        //	System.out.println("Replace:" + tempRecords);
//        if (tempRecords != NULL && !tempRecords.isEmpty()) {
//            resultRecords.addAll(tempRecords);
//        }
//        tempRecords = Markov.reCompositeRecords(state);
//        //	System.out.println("reComposite:" + tempRecords);
//        if (tempRecords != NULL && !tempRecords.isEmpty()) {
//            resultRecords.addAll(tempRecords);
//        }
//        //System.out.println("Line 469");
//    }

//    return resultRecords;
//}
