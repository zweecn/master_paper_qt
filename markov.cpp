#include "markov.h"
#include "markovrecord.h"
#include <QDebug>
#include <qmath.h>


Markov::Markov()
{
}

bool Markov::helper(MarkovState & state)
{
//    qDebug() << "Markov::helper(MarkovState & state)...";
    if (!MarkovRecord::hasState(state)) {
        MarkovRecord::addState(state);
    } else {
        MarkovState *tmps = MarkovRecord::getState(state);
        if (tmps == NULL) {
            qDebug() << "Markov::helper(MarkovState & state). tmps==NULL";
        } else {
            state = (*tmps);
        }
    }
    return true;
}

bool Markov::helper(MarkovAction & action)
{
//    qDebug() << "Markov::helper(MarkovAction & action)...";
    if (!MarkovRecord::hasAction(action)) {
        MarkovRecord::addAction(action);
    } else {
        MarkovAction *tmpa = MarkovRecord::getAction(action);
        if (tmpa == NULL) {
            qDebug() << "Markov::noActionRecords(MarkovState & state). tmpa==NULL";
        } else {
            action = (*tmpa);
        }
    }
    return true;
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
        helper(stateAfter);
        helper(noac);
        records.append(MarkovRecord(stateAfter, stateAfter, noac, 1, 0, timeCost));
        return records;
    }
    else
    {
        QList<MarkovState> states = stateAfter.getNextTwoStates();
        if (states.size() < 2)
        {
            return records;
        }
        MarkovState stateAfter1 = states[0];
        MarkovState stateAfter2 = states[1];
        helper(stateAfter1);
        helper(stateAfter2);
        helper(noac);
        records.append(MarkovRecord(state, stateAfter1, noac,
                                 stateAfter.nextToDoActivity->blindService->reliability, 0, timeCost));
        records.append(MarkovRecord(state, stateAfter2, noac,
                                 1 - stateAfter.nextToDoActivity->blindService->reliability, 0, timeCost));

        return records;
    }
}


QList<MarkovRecord> Markov::terminateRecords(MarkovState & state)
{
    QList<MarkovRecord> records;
    double timeCost = 0;
    if (state.globalState != MarkovState::S_NORMAL
            && state.globalState != MarkovState::S_SUCCEED
            && (state.faultActivity->x - 1) > 0)
    {
        timeCost = (state.faultActivity->x - 1) * state.faultActivity->blindService->execTime;
    }
    MarkovState stateAfter = state;
    MarkovAction terminateAction = MarkovAction::terminate(stateAfter);

    if (terminateAction.type == MarkovAction::ERROR_ACTION
            || stateAfter.isFinished()
            || MarkovRecord::hasStateAction(stateAfter, terminateAction))
    {
        return records;
    }

    Markov::helper(stateAfter);
    Markov::helper(terminateAction);

    stateAfter.globalState = MarkovState::S_FAILED;
    stateAfter.finished = true;

    MarkovRecord rd(state, stateAfter, terminateAction, 1, 0, timeCost);
    records.append(rd);
    return records;
}

////在action里面加上消耗和报酬
QList<MarkovRecord> Markov::redoRecords(MarkovState & state)
{
    QList<MarkovRecord> records;
    MarkovState stateAfter = state;
    MarkovAction redoAction = MarkovAction::redo(stateAfter);
//    qDebug() << redoAction.type << stateAfter.isFinished()
//             << MarkovRecord::hasStateAction(stateAfter, redoAction);

    if (redoAction.type == MarkovAction::ERROR_ACTION
            || stateAfter.isFinished()
            || MarkovRecord::hasStateAction(stateAfter, redoAction)) {
        return records;
    }

//    if (stateAfter.faultActivity != NULL)
//        stateAfter.faultActivity->x = 0;
//    stateAfter.globalState = MarkovState::S_NORMAL;

    QList<MarkovState> states = stateAfter.getNextTwoStates();

    if (states.size() < 2)
    {
        return records;
    }

    MarkovState stateAfter1 = states[0];
    MarkovState stateAfter2 = states[1];

    helper(stateAfter1);
    helper(stateAfter2);
    helper(redoAction);

    records.append(MarkovRecord(state, stateAfter1, redoAction,
                                state.faultActivity->blindService->reliability,
                                state.faultActivity->blindService->price,
                                fabs(state.faultActivity->blindService->execTime * state.faultActivity->x)));

    records.append(MarkovRecord(state, stateAfter2, redoAction,
                                 1 - state.faultActivity->blindService->reliability,
                                 state.faultActivity->blindService->price,
                                 fabs(state.faultActivity->blindService->execTime * state.faultActivity->x)));
    return records;
}



QList<MarkovRecord> Markov::replaceRecords(MarkovState & state)
{
    QList<MarkovRecord> records;
    MarkovState stateAfter = state;
    MarkovAction replaceAction = MarkovAction::replace(stateAfter);

    if (replaceAction.type == MarkovAction::ERROR_ACTION
            || stateAfter.isFinished()
            || MarkovRecord::hasStateAction(stateAfter, replaceAction)) {
        return records;
    }

//    qDebug() << stateAfter.toString();

//    if (stateAfter.faultActivity != NULL)
//        stateAfter.faultActivity->x = 0;
//    stateAfter.globalState = MarkovState::S_NORMAL;
//    stateAfter.finished = false;

//    qDebug() << stateAfter.faultActivity;

    QList<MarkovState> states = stateAfter.getNextTwoStates();
    if (states.size() < 2)
    {
        return records;
    }

    MarkovState stateAfter1 = states[0];
    MarkovState stateAfter2 = states[1];
    helper(stateAfter1);
    helper(stateAfter2);
    helper(replaceAction);

    // NEED FINISH
    records.append(MarkovRecord(state, stateAfter1, replaceAction,
                                replaceAction.getReplacePosibility(),
                                 replaceAction.getReplacePriceCost(),
                                replaceAction.getReplaceTimeCost()));
    records.append(MarkovRecord(state, stateAfter2, replaceAction,
                                1 - replaceAction.getReplacePosibility(),
                                replaceAction.getReplacePriceCost(),
                                replaceAction.getReplaceTimeCost()));

    return records;
}

QList<MarkovRecord> Markov::reCompositeRecords(MarkovState & state)
{
    QList<MarkovRecord> records;
    MarkovState stateAfter = state;
    MarkovAction reCompositeAction = MarkovAction::reComposite(stateAfter);

    if (reCompositeAction.type == MarkovAction::ERROR_ACTION
            || stateAfter.isFinished()
            || MarkovRecord::hasStateAction(stateAfter, reCompositeAction)) {
        return records;
    }

    QList<MarkovState> states = stateAfter.getNextTwoStates();
    if (states.size() < 2)
    {
        return records;
    }

    MarkovState stateAfter1 = states[0];
    MarkovState stateAfter2 = states[1];
    helper(stateAfter1);
    helper(stateAfter2);
    helper(reCompositeAction);

    records.append(MarkovRecord(state, stateAfter1, reCompositeAction,
                                reCompositeAction.getReComposePosibility(),
                                reCompositeAction.getReComposePriceCost(),
                                reCompositeAction.getReComposeTimeCost()));
    records.append(MarkovRecord(state, stateAfter2, reCompositeAction,
                                1 - reCompositeAction.getReComposePosibility(),
                                reCompositeAction.getReComposePriceCost(),
                                reCompositeAction.getReComposeTimeCost()));

//    qDebug() << records.size();
    return records;
}

QList<MarkovRecord> Markov::getRecords(MarkovState & state)
{
//    qDebug() << "Markov::getRecords() ... " << state.toString();
    QList<MarkovRecord> resultRecords;
    if (MarkovRecord::hasStateBefore(state))
    {
        qDebug() << "if (MarkovRecord::hasStateBefore(state)) ";
        return resultRecords;
    }

    if (state.globalState == MarkovState::S_NORMAL ) {
        QList<MarkovRecord> tempRecords = Markov::noActionRecords(state);
        if (!tempRecords.isEmpty()) {
            resultRecords += tempRecords;
        }
    } else if (state.globalState == MarkovState::S_DELAYED) {
        QList<MarkovRecord> tempRecords = Markov::noActionRecords(state);
        if (!tempRecords.isEmpty()) {
            resultRecords += tempRecords;
        }
        tempRecords = Markov::terminateRecords(state);
        if (!tempRecords.isEmpty()) {
            resultRecords += tempRecords;
        }
        tempRecords = Markov::replaceRecords(state);
        if (!tempRecords.isEmpty()) {
            resultRecords += tempRecords;
        }
        tempRecords = Markov::reCompositeRecords(state);
        if (!tempRecords.isEmpty()) {
            resultRecords += tempRecords;
        }
    } else if (state.globalState == MarkovState::S_PRICE_UP) {
        QList<MarkovRecord> tempRecords = Markov::noActionRecords(state);
        if (!tempRecords.isEmpty()) {
            resultRecords += tempRecords;
        }
        tempRecords = Markov::terminateRecords(state);
        if (!tempRecords.isEmpty()) {
            resultRecords += tempRecords;
        }
        tempRecords = Markov::replaceRecords(state);
        if (!tempRecords.isEmpty()) {
            resultRecords += tempRecords;
        }
        tempRecords = Markov::reCompositeRecords(state);
        if (!tempRecords.isEmpty()) {
            resultRecords += tempRecords;
        }

    } else if (state.globalState == MarkovState::S_SUCCEED) {
        QList<MarkovRecord> tempRecords = Markov::noActionRecords(state);
        if (!tempRecords.isEmpty()) {
            resultRecords += tempRecords;
        }
    } else if (state.globalState == MarkovState::S_FAILED) {
//        qDebug() << "Begin." << state.toString();
        QList<MarkovRecord> tempRecords = Markov::terminateRecords(state);
//        qDebug() << "terminateRecords tempRecords:" << tempRecords.size()
//                    << state.toString();
        if (!tempRecords.isEmpty()) {
            resultRecords += tempRecords;
        }
        tempRecords = Markov::redoRecords(state);
//        qDebug() << "redoRecords tempRecords:" << tempRecords.size()
//                    << state.toString();
        if (!tempRecords.isEmpty()) {
            resultRecords += tempRecords;
        }
        tempRecords = Markov::replaceRecords(state);
//        qDebug() << "replaceRecords tempRecords:" << tempRecords.size()
//                    << state.toString();
        if (!tempRecords.isEmpty()) {
            resultRecords += tempRecords;
        }
        tempRecords = Markov::reCompositeRecords(state);
//        qDebug() << "reCompositeRecords tempRecords:" << tempRecords.size()
//                    << state.toString();
        if (!tempRecords.isEmpty()) {
            resultRecords += tempRecords;
        }
    }

//    qDebug() << "Markov::getRecords() finished. resultRecords.size():" << resultRecords.size();
    return resultRecords;
}
