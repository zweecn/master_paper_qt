//#include "markovstate.h"
//#include "workflow.h"

//#include <QSet>
//#include <QDebug>

//MarkovState::MarkovState()
//    : WebServiceFlow()
//{
//    id = -1;
//    init();
//}

//MarkovState::MarkovState(const MarkovState &other)
//{
////    finished = other.finished;
////    id = other.id;
////    globalState = other.globalState;

////    nextStateArray = other.nextStateArray;
////    nextToDoActivityList = other.nextToDoActivityList;
////    nextToDoActivity = other.nextToDoActivity;
////    faultActivity = other.faultActivity;

////    for (int i = 0; i < WorkFlow::Instance()->getActivitySize(); i++)
////    {
////        activities[i] = other.activities[i];
////    }

//    finished = other.finished;
//    id = other.id;
//    globalState = other.globalState;

//    nextStateArray = other.nextStateArray;

//    for (int i = 0; i < WorkFlow::Instance()->getActivitySize(); i++)
//    {
//        activities[i] = other.activities[i];
//    }

//    nextToDoActivityList.clear();
//    for (int i = 0; i < other.nextToDoActivityList.size(); i++)
//    {
//        int id = other.nextToDoActivityList[i]->number;
//        Activity * aTmp = &activities[id];
//        nextToDoActivityList.append(aTmp);
//    }

//    nextToDoActivity = (other.nextToDoActivity == NULL
//                        ? NULL : &activities[other.nextToDoActivity->number]);
//    faultActivity = ( other.faultActivity == NULL
//                      ? NULL : &activities[other.faultActivity->number]);
//}

//// super clone activities
//bool MarkovState::init()
//{
////    qDebug() << "MarkovState::init() ...";
//    bool failed = false;
//    nextToDoActivityList.clear();
//    nextToDoActivity = NULL;
//    faultActivity = NULL;
//    for (int i = 0; i < WorkFlow::Instance()->getActivitySize(); i++) {
//        Activity * ac = &activities[i];
//        if (ac->x < 0) {
//            ac->state = S_FAILED;
//            failed = true;
//        } else if (ac->x < 1.0001 && ac->x > 0.9999) {
//            ac->state = S_SUCCEED;
//        } else if (ac->x >= 0 && ac->x < 1) {
//            ac->state = S_NORMAL;
//        } else if (ac->x > 1) {
//            ac->state = S_DELAYED;
//        }
//        switch (ac->state) {
//        case S_FAILED:
//        case S_DELAYED:
//            faultActivity = ac;
//            if (isPrefixActivitiesFinished(ac->number)) {
//                nextToDoActivityList.append(ac);
//            }
//            break;
//        case S_NORMAL:
//            if (isPrefixActivitiesFinished(ac->number)) {
//                nextToDoActivityList.append(ac);
//            }
//            break;
//        default:
//            break;
//        }

//    }
//    if (this->activities[WorkFlow::Instance()->getActivitySize()-1].state == S_SUCCEED) {
//        finished = true;
//    } else {
//        finished = false;
//    }

//    if (nextToDoActivityList.isEmpty()
//            && isPrefixActivitiesFinished(activities[WorkFlow::Instance()->getActivitySize()-1].number))
//    {
//        globalState = S_SUCCEED;
//    }
//    else
//    {
//        for (int i = 0; i < nextToDoActivityList.size(); i++) {
//            Activity * ac = nextToDoActivityList[i];
//            if (nextToDoActivity == NULL || ac->x * nextToDoActivity->blindService->execTime
//                    < nextToDoActivity->x * nextToDoActivity->blindService->execTime) {
//                nextToDoActivity = ac;
//            }
//        }
//        if (faultActivity == NULL) {
//            if (failed) {
//                globalState = S_FAILED;
//            }
//            if ((finished && !failed) || (activities[WorkFlow::Instance()->getActivitySize()-1].x >= 1)) {
//                globalState = S_SUCCEED;
//            }
//            if (!finished && !failed) {
//                globalState = S_NORMAL;
//            }
//        } else {
//            if (globalState  != S_SUCCEED) {
//                if (faultActivity->state != S_NORMAL) {
//                    globalState = faultActivity->state;
//                }
//            }
//        } //else
//    } //else
////    qDebug() << "MarkovState::init() finished.";
//    return true;
//}

//QString MarkovState::name()
//{
//    QString res;
//    switch (globalState)
//    {
//    case S_NORMAL:
//        res = "S_NORMAL";
//        break;
//    case S_FAILED:
//        res = "S_FAILED";
//        break;
//    case S_SUCCEED:
//        res = "S_SUCCEED";
//        break;
//    case S_PRICE_UP:
//        res = "S_PRICE_UP";
//        break;
//    case S_DELAYED:
//        res = "S_DELAYED";
//        break;
//    default:
//        res = QString("%1").arg(globalState);
//        break;
//    }
//    return res;
//}

//QString MarkovState::toString()
//{
//    QString res(QString("State:%1 %2 [").arg(id).arg(name()));
//    for (int i = 0; i < WorkFlow::Instance()->getActivitySize(); i++)
//    {
//        res.append(QString("%1:%2,%3").arg(activities[i].number)
//                   .arg((int)activities[i].blindService->id)
//                   .arg(activities[i].x));
//        res.append(" ");
//    }
//    if (finished)
//    {
//        res.append("FINISHED");
//    }
//    else
//    {
//        res.append("EXECING");
//    }
//    res = res.trimmed().append("]");
//    return res;
//}

//bool MarkovState::isPrefixActivitiesFinished(int activityId)
//{
//    QSet<int> prefixActivityNumbers = WorkFlow::Instance()->getPrefixs(activityId);
//    if (!prefixActivityNumbers.isEmpty()) {
//        QSetIterator<int> it(prefixActivityNumbers);
//        while (it.hasNext()) {
//            int id = it.next();
//            if (activities[id].state != S_SUCCEED) {
//                return false;
//            }
//        }
//    }
//    return true;
//}

//MarkovState* MarkovState::nextSecond()
//{
//    if (this->isFinished()) {
//        return NULL;
//    }
//    if (this->isFailed()) {
//        return NULL;
//    }
//    if (nextToDoActivityList.size() == 1) { //Seq
//        nextToDoActivity->x += (1.0 / nextToDoActivity->blindService->execTime);
//    } else { //Ban
//        for (int i = 0; i < nextToDoActivityList.size(); i++) {
//            nextToDoActivityList[i]->x += (1.0 / nextToDoActivityList[i]->blindService->execTime);
//        }
//    }
//    init();
//    return this;
//}

//QList<MarkovState> MarkovState::getNextTwoStates()
//{
//    nextStateArray.clear();
//    nextStateArray.append(*this);
//    if (isFailed())
//    {
//        return nextStateArray;
//    }
//    else
//    {
//        nextStateArray.append(*this);
//        if (nextToDoActivityList.size() == 1)
//        { //Seq
//            nextStateArray[0].activities[nextToDoActivity->number].x = 1;
//            nextStateArray[1].activities[nextToDoActivity->number].x = -1;
//        }
//        else
//        { //Ban
//            double timeCostTemp = (1 - nextToDoActivity->x) *
//                    nextToDoActivity->blindService->execTime;
//            for (int i = 0; i < nextToDoActivityList.size(); i++)
//            {
//                Activity * ac = nextToDoActivityList[i];
//                double xTemp = (timeCostTemp + ac->x * ac->blindService->execTime)
//                        / ac->blindService->execTime;

//                if (xTemp >= 1) {
//                    nextStateArray[0].activities[ac->number].x = 1;
//                } else {
//                    nextStateArray[0].activities[ac->number].x = xTemp;
//                }
//            }
//            nextStateArray[0].activities[nextToDoActivity->number].x = 1;
//            nextStateArray[1].activities[nextToDoActivity->number].x = -1;
//        }
//        nextStateArray[0].init();
//        nextStateArray[1].init();

//        return nextStateArray;
//    }
//}


//MarkovState & MarkovState::operator =(const MarkovState & other)
//{
//    if (this == &other)
//    {
//        return *this;
//    }
//    finished = other.finished;
//    id = other.id;
//    globalState = other.globalState;

//    nextStateArray = other.nextStateArray;

//    for (int i = 0; i < WorkFlow::Instance()->getActivitySize(); i++)
//    {
//        activities[i] = other.activities[i];
//    }

//    nextToDoActivityList.clear();
//    for (int i = 0; i < other.nextToDoActivityList.size(); i++)
//    {
//        int id = other.nextToDoActivityList[i]->number;
//        Activity * aTmp = &activities[id];
//        nextToDoActivityList.append(aTmp);
//    }

//    nextToDoActivity = (other.nextToDoActivity == NULL
//                        ? NULL : &activities[other.nextToDoActivity->number]);
//    faultActivity = ( other.faultActivity == NULL
//                      ? NULL : &activities[other.faultActivity->number]);
//    return *this;
//}

//bool MarkovState::operator ==(const MarkovState & other) const
//{
//    if (finished != other.finished
//            || globalState != other.globalState)
//    {
//        return false;
//    }

//    if (faultActivity != NULL && other.faultActivity != NULL)
//    {
//        if (!(*faultActivity == *(other.faultActivity)))
//        {
//            return false;
//        }
//    }
//    if (faultActivity != NULL && other.faultActivity == NULL)
//    {
//        return false;
//    }
//    if (faultActivity == NULL && other.faultActivity != NULL)
//    {
//        return false;
//    }

//    for (int i = 0; i < WorkFlow::Instance()->getActivitySize(); i++)
//    {
//        if (!(activities[i] == other.activities[i]))
//        {
//            return false;
//        }
//    }
//    return true;
//}

//bool MarkovState::operator <(const MarkovState & other) const
//{
//    if (finished >= other.finished
//            || globalState >= other.globalState)
//    {
//        return false;
//    }
//    if (faultActivity != NULL && other.faultActivity != NULL)
//    {
//        if (!(*faultActivity < *(other.faultActivity)))
//        {
//            return false;
//        }
//    }
//    if (faultActivity != NULL && other.faultActivity == NULL)
//    {
//        return false;
//    }
//    if (faultActivity == NULL && other.faultActivity != NULL)
//    {
//        return true;
//    }

//    for (int i = 0; i < WorkFlow::Instance()->getActivitySize(); i++)
//    {
//        if (!(activities[i] < other.activities[i]))
//        {
//            return false;
//        }
//    }
//    return true;
//}

//bool MarkovState::isFailed()
//{
//    return globalState == S_FAILED;
//}

//void MarkovState::setFailed(bool failed)
//{
//    if (failed == true)
//        globalState = S_FAILED;
//}

//bool MarkovState::isFinished()
//{
//    return finished;
//}
