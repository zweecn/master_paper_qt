#include "webservicesimulation.h"
//#include "webservicestate.h"
//#include "webserviceaction.h"
#include "resource.h"
#include "workflow.h"
#include "activity.h"
#include "config.h"
#include "markovaction.h"

#include <QDebug>
#include <QFile>
#include <QIODevice>
#include <QTextCodec>
#include <QStringList>
#include <qmath.h>
#include <QSet>
#include <climits>

const QString start_filename = "data/business_start";
const int start_member_size = 3;

WebServiceSimulation::WebServiceSimulation()
{
    stateAction = NULL;
    actionState = NULL;
    posibility = NULL;
    actionUtility = NULL;
    stateUtility = NULL;
}

WebServiceSimulation::~WebServiceSimulation()
{
    clearData();
}

void WebServiceSimulation::run()
{
    createStateTransTable();
    initUtility();
    runMarkov();

    qDebug() << "Derect reward:" ;
    for (int i = 0; i < 2*(WebServiceAtomState::STATE_SIZE); i++)
    {
        for (int j = 0; j < actionList.size(); j++)
        {
            if (stateAction[i][j])
            {
                qDebug() << stateList[i].toString() << actionList[j].toString();
            }
        }
    }
    qDebug() << "State utility:" ;
    for (int i = 0; i < maxStateSize; i++)
    {
        qDebug() << stateList[i].toString() << stateUtility[stateList[i].hash()];
    }
}

bool WebServiceSimulation::runMarkov()
{
    qDebug() << "WebServiceSimulation::runMarkov() ...";
    for (int t = WorkFlow::Instance()->getActivitySize() - 2; t >= 0; t--)
    {
        qDebug() << "t =" << t;
        for (int i = actionList.size() - 1; i >= 0; i--)
        {
//            qDebug() << "i =" << i << actionList.size() << stateList.size() << maxStateSize;
            double actionUtilityTemp = 0;
            int sumP = 0;
            for (int j = 0; j < maxStateSize; j++)
            {
                sumP += posibility[actionList[i].hash()][stateList[j].hash()];
            }
            for (int j = 0; j < maxStateSize; j++)
            {
                if (actionState[actionList[i].hash()][stateList[j].hash()])
                {
                    double p = (double) posibility[actionList[i].hash()][stateList[j].hash()] / sumP;
                    actionUtilityTemp += stateUtility[stateList[j].hash()] * p;
                }
            }
            if (actionUtility[actionList[i].hash()] < actionUtilityTemp)
            {
                actionUtility[actionList[i].hash()] = actionUtilityTemp;
            }
        }

        for (int i = maxStateSize - 1; i>= 0; i--)
        {
            for (int j = 0; j < actionList.size(); j++)
            {
                if (stateAction[stateList[i].hash()][actionList[j].hash()]
                        && stateUtility[stateList[i].hash()] < actionUtility[actionList[j].hash()])
                {
                    stateUtility[stateList[i].hash()] < actionUtility[actionList[j].hash()];
                }
            }
        }
    }
    qDebug() << "WebServiceSimulation::runMarkov() finished.";
    return true;
}

bool WebServiceSimulation::initUtility()
{
    stateUtility = new double[maxStateSize];
    for (int i = 0; i < maxStateSize; i++)
    {
        stateUtility[i] = - INT_MAX;
    }
    actionUtility = new double[maxActionSize];
    for (int i = 0; i < maxActionSize; i++)
    {
        actionUtility[i] = - INT_MAX;
    }

    int i = WorkFlow::Instance()->getActivitySize() - 1;
    for (int j = 0; j < WebServiceAtomState::STATE_SIZE; j++)
    {
        WebServiceAtomState & s = stateList[i * WebServiceAtomState::STATE_SIZE + j];
        if (s.stateType == WebServiceAtomState::READY_U)
        {
            stateUtility[s.hash()] = - Config::Instance()->getPuinishmentReadyU();
        }
        else if (s.stateType == WebServiceAtomState::READY_N)
        {
            stateUtility[s.hash()] = - Config::Instance()->getPuinishmentReadyN();
        }
        else if (s.stateType == WebServiceAtomState::FAIL)
        {
            stateUtility[s.hash()] = - Config::Instance()->getPuinishmentFailed();
        }
        else if (s.stateType == WebServiceAtomState::FINISH_U)
        {
            stateUtility[s.hash()] = Config::Instance()->getRewardFinishU();
        }
        else if (s.stateType == WebServiceAtomState::FINISH_N)
        {
            stateUtility[s.hash()] = Config::Instance()->getRewardFinishN();
        }
        else if (s.stateType == WebServiceAtomState::STOP)
        {
            stateUtility[s.hash()] = - Config::Instance()->getPuinishmentStop();
        }

    }
    for (i = 0; i < WorkFlow::Instance()->getActivitySize() - 1; i++)
    {
        for (int j = 0; j < WebServiceAtomState::STATE_SIZE; j++)
        {
            WebServiceAtomState & s = stateList[i * WebServiceAtomState::STATE_SIZE + j];
            if (s.stateType == WebServiceAtomState::STOP)
            {
                stateUtility[s.hash()] = - Config::Instance()->getPuinishmentStop();
            }
        }
    }
    return true;
}

bool WebServiceSimulation::clearData()
{
    for (int i = 0; i < maxStateSize; i++)
    {
        if (stateAction && stateAction[i])
            delete[] stateAction[i];
    }

    for (int i = 0; i < maxActionSize; i++)
    {
        if (actionState && actionState[i])
            delete[] actionState[i];
        if (posibility && posibility[i])
            delete[] posibility[i];
    }
    if (stateAction)
        delete[] stateAction;
    if (actionState)
        delete[] actionState;
    if (posibility)
        delete[] posibility;

    if (actionUtility)
        delete[] actionUtility;
    if (stateUtility)
        delete[] stateUtility;

    stateList.clear();
    actionList.clear();
    return true;
}

bool WebServiceSimulation::init()
{
    qDebug() << "WebServiceSimulation::init() begin...";
    clearData();

    for (int i = 0; i < WorkFlow::Instance()->getActivitySize(); i++)
    {
        for (int j = 0; j < WebServiceAtomState::STATE_SIZE; j++)
        {
            WebServiceAtomState s;
            s.activityId = i;
            s.stateType = j;
            stateList.append(s);
        }
    }

    maxStateSize = WorkFlow::Instance()->getActivitySize() * WebServiceAtomState::STATE_SIZE;
    maxActionSize = maxStateSize * WebServiceAction::ACTION_SIZE;
    stateAction = new int*[maxStateSize];
    for (int i = 0; i < maxStateSize; i++)
    {
        stateAction[i] = new int[maxActionSize];
        memset(stateAction[i], 0, sizeof(int) * maxActionSize);
    }
    actionState = new int*[maxActionSize];
    posibility = new int*[maxActionSize];
    for (int i = 0; i < maxActionSize; i++)
    {
        actionState[i] = new int[maxStateSize];
        memset(actionState[i], 0, sizeof(int) * maxStateSize);
        posibility[i] = new int[maxStateSize];
        memset(posibility[i], 0, sizeof(int) * maxStateSize);
    }

    qDebug() << "WebServiceSimulation::init() finised.";
    return true;
}


bool WebServiceSimulation::createStateTransTable()
{
    if (!init())
    {
        return false;
    }
    for (int i = 0; i < stateList.size(); i++)
    {
        WebServiceAtomState & s = stateList[i];
        if (s.stateType == WebServiceAtomState::READY_U)
        {
            terminate(s);
            doNothing(s);
            replace(s);
            recomposite(s);
        }
        else if (s.stateType == WebServiceAtomState::READY_N)
        {
            noNeedDo(s);
        }
        else if (s.stateType == WebServiceAtomState::FAIL)
        {
            terminate(s);
            retry(s);
            replace(s);
            recomposite(s);
        }
        else if (s.stateType == WebServiceAtomState::FINISH_U)
        {
            doNothing(s);
            terminate(s);
            recomposite(s);
        }
        else if (s.stateType == WebServiceAtomState::FINISH_N)
        {
            noNeedDo(s);
        }
    }

    return true;
}

void WebServiceSimulation::noNeedDo(WebServiceAtomState & s)
{
    WebServiceAction action = noNeedDo();
    WebServiceAction *pAction = getAction(action);
    if (pAction == NULL)
    {
        action.id = actionList.size();
        actionList.append(action);
        pAction = &action;
    }
    stateAction[s.hash()][pAction->hash()] = 1;

    QSet<int> suffix = WorkFlow::Instance()->getSuffixs(s.activityId);
    QSetIterator<int> it(suffix);
    while (it.hasNext())
    {
        int suffixId = it.next();
        WebServiceAtomState suffixState;
        suffixState.activityId = suffixId;
        if (s.stateType == WebServiceAtomState::READY_N)
        {
            suffixState.stateType = WebServiceAtomState::FINISH_N;
            actionState[pAction->hash()][suffixState.hash()] = 1;
            posibility[pAction->hash()][suffixState.hash()]
                    = wsf.activities[s.activityId].blindService->reliability;

            suffixState.stateType = WebServiceAtomState::FINISH_U;
            actionState[pAction->hash()][suffixState.hash()] = 1;
            posibility[pAction->hash()][suffixState.hash()]
                    = MAX_POSIBILITY - wsf.activities[s.activityId].blindService->reliability;

            suffixState.stateType = WebServiceAtomState::FAIL;
            actionState[pAction->hash()][suffixState.hash()] = 1;
            posibility[pAction->hash()][suffixState.hash()]
                    = MAX_POSIBILITY - wsf.activities[s.activityId].blindService->reliability;
        }
        else if (s.stateType == WebServiceAtomState::FINISH_N)
        {
            suffixState.stateType = WebServiceAtomState::READY_U;
            actionState[pAction->hash()][suffixState.hash()] = 1;
            posibility[pAction->hash()][suffixState.hash()] = MAX_POSIBILITY;

            suffixState.stateType = WebServiceAtomState::READY_N;
            actionState[pAction->hash()][suffixState.hash()] = 1;
            posibility[pAction->hash()][suffixState.hash()] = MAX_POSIBILITY;
        }
    }
}

WebServiceAction WebServiceSimulation::noNeedDo()
{
    WebServiceAction action;
    action.type = WebServiceAction::NO_NEED_DO;
    action.dc = 0;
    action.dt = 0;
    return action;
}

void WebServiceSimulation::recomposite(WebServiceAtomState & s)
{
    WebServiceAction action = recomposite(s.activityId);
    WebServiceAction *pAction = getAction(action);
    if (pAction == NULL)
    {
        action.id = actionList.size();
        actionList.append(action);
        pAction = &action;
    }
    stateAction[s.hash()][pAction->hash()] = 1;

    QSet<int> suffix = WorkFlow::Instance()->getSuffixs(s.activityId);
    QSetIterator<int> it(suffix);
    while (it.hasNext())
    {
        int suffixId = it.next();
        WebServiceAtomState suffixState;
        suffixState.activityId = suffixId;
        if (s.stateType == WebServiceAtomState::READY_U
                || s.stateType == WebServiceAtomState::FAIL)
        {
            suffixState.stateType = WebServiceAtomState::FINISH_N;
            actionState[pAction->hash()][suffixState.hash()] = 1;
            posibility[pAction->hash()][suffixState.hash()]
                    = WorkFlow::Instance()
                    ->all_service[action.replaceList.first().newServiceId].reliability;

            suffixState.stateType = WebServiceAtomState::FINISH_U;
            actionState[pAction->hash()][suffixState.hash()] = 1;
            posibility[pAction->hash()][suffixState.hash()]
                    = MAX_POSIBILITY - WorkFlow::Instance()
                    ->all_service[action.replaceList.first().newServiceId].reliability;

            suffixState.stateType = WebServiceAtomState::FAIL;
            actionState[pAction->hash()][suffixState.hash()] = 1;
            posibility[pAction->hash()][suffixState.hash()]
                    = MAX_POSIBILITY - WorkFlow::Instance()
                    ->all_service[action.replaceList.first().newServiceId].reliability;
        }
        else if (s.stateType == WebServiceAtomState::FINISH_U)
        {
            suffixState.stateType = WebServiceAtomState::READY_N;
            actionState[pAction->hash()][suffixState.hash()] = 1;
            posibility[pAction->hash()][suffixState.hash()] = MAX_POSIBILITY;

            suffixState.stateType = WebServiceAtomState::READY_U;
            actionState[pAction->hash()][suffixState.hash()] = 1;
            posibility[pAction->hash()][suffixState.hash()] = MAX_POSIBILITY;
        }
    }
}

WebServiceAction WebServiceSimulation::recomposite(int activityId)
{
    WebServiceAction action;
    QList<ReplaceNode> oldNewServiceList = recomposeFreeService(activityId);
    QListIterator<ReplaceNode> it(oldNewServiceList);
    action.dc = action.dt = 0;
    while (it.hasNext())
    {
        const ReplaceNode & node = it.next();
        action.dc = WorkFlow::Instance()->all_service[node.newServiceId].price
                - wsf.activities[node.activityId].blindService->price;
        action.dt = WorkFlow::Instance()->all_service[node.newServiceId].execTime
                - wsf.activities[node.activityId].blindService->execTime;
    }
    action.type = WebServiceAction::RE_COMPOSE;
    action.replaceList = oldNewServiceList;
    return action;
}

void WebServiceSimulation::retry(WebServiceAtomState & s)
{
    WebServiceAction action = retry(s.activityId);
    WebServiceAction *pAction = getAction(action);
    if (pAction == NULL)
    {
        action.id = actionList.size();
        actionList.append(action);
        pAction = &action;
    }
    stateAction[s.hash()][pAction->hash()] = 1;

    QSet<int> suffix = WorkFlow::Instance()->getSuffixs(s.activityId);
    QSetIterator<int> it(suffix);
    while (it.hasNext())
    {
        int suffixId = it.next();
        WebServiceAtomState suffixState;
        suffixState.activityId = suffixId;
        if (s.stateType == WebServiceAtomState::FAIL)
        {
            suffixState.stateType = WebServiceAtomState::FINISH_N;
            actionState[pAction->hash()][suffixState.hash()] = 1;
            posibility[pAction->hash()][suffixState.hash()]
                    = wsf.activities[s.activityId].blindService->reliability;

            suffixState.stateType = WebServiceAtomState::FINISH_U;
            actionState[pAction->hash()][suffixState.hash()] = 1;
            posibility[pAction->hash()][suffixState.hash()]
                    = MAX_POSIBILITY - wsf.activities[s.activityId].blindService->reliability;
            suffixState.stateType = WebServiceAtomState::FAIL;
            actionState[pAction->hash()][suffixState.hash()] = 1;
            posibility[pAction->hash()][suffixState.hash()]
                    = MAX_POSIBILITY - wsf.activities[s.activityId].blindService->reliability;
        }
    }
}

WebServiceAction WebServiceSimulation::retry(int activityId)
{
    WebServiceAction action;
    action.type = WebServiceAction::RETRY;
    action.dc = wsf.activities[activityId].blindService->price;
    action.dt = wsf.activities[activityId].blindService->execTime;
    return action;
}

void WebServiceSimulation::replace(WebServiceAtomState & s)
{
    WebServiceAction action = replace(s.activityId);
    WebServiceAction *pAction = getAction(action);
    if (pAction == NULL)
    {
        action.id = actionList.size();
        actionList.append(action);
        pAction = &action;
    }
    stateAction[s.hash()][pAction->hash()] = 1;

    QSet<int> suffix = WorkFlow::Instance()->getSuffixs(s.activityId);
    QSetIterator<int> it(suffix);
    while (it.hasNext())
    {
        int suffixId = it.next();
        WebServiceAtomState suffixState;
        suffixState.activityId = suffixId;
        if (s.stateType == WebServiceAtomState::READY_U
                || s.stateType == WebServiceAtomState::FAIL)
        {
            suffixState.stateType = WebServiceAtomState::FINISH_N;
            actionState[pAction->hash()][suffixState.hash()] = 1;
            posibility[pAction->hash()][suffixState.hash()]
                    = WorkFlow::Instance()
                    ->all_service[action.replaceList.first().newServiceId].reliability;

            suffixState.stateType = WebServiceAtomState::FINISH_U;
            actionState[pAction->hash()][suffixState.hash()] = 1;
            posibility[pAction->hash()][suffixState.hash()]
                    = MAX_POSIBILITY - WorkFlow::Instance()
                    ->all_service[action.replaceList.first().newServiceId].reliability;

            suffixState.stateType = WebServiceAtomState::FAIL;
            actionState[pAction->hash()][suffixState.hash()] = 1;
            posibility[pAction->hash()][suffixState.hash()]
                    = MAX_POSIBILITY - WorkFlow::Instance()
                    ->all_service[action.replaceList.first().newServiceId].reliability;
        }
    }
}

WebServiceAction WebServiceSimulation::replace(int activityId)
{
    WebServiceAction action;
    AtomService* newFreeService = nextFreeService(activityId);
    if (newFreeService == NULL)
    {
        action.type = WebServiceAction::REPLACE;
        action.dc = wsf.activities[activityId].blindService->price;
        action.dt = wsf.activities[activityId].blindService->execTime;
        return action;
    }

    action.type = WebServiceAction::REPLACE;
    ReplaceNode node;
    node.activityId = activityId;
    node.oldServiceId = wsf.activities[activityId].blindService->id;
    node.newServiceId = newFreeService->id;
    action.replaceList.append(node);
    action.dc = newFreeService->price - wsf.activities[activityId].blindService->price;
    action.dt = newFreeService->execTime - wsf.activities[activityId].blindService->execTime;

    return action;
}

void WebServiceSimulation::doNothing(WebServiceAtomState & s)
{
    WebServiceAction action = doNothing();
    WebServiceAction *pAction = getAction(action);
    if (pAction == NULL)
    {
        action.id = actionList.size();
        actionList.append(action);
        pAction = &action;
    }
    stateAction[s.hash()][pAction->hash()] = 1;

    QSet<int> suffix = WorkFlow::Instance()->getSuffixs(s.activityId);
    QSetIterator<int> it(suffix);
    while (it.hasNext())
    {
        int suffixId = it.next();
        WebServiceAtomState suffixState;
        suffixState.activityId = suffixId;
        if (s.stateType == WebServiceAtomState::READY_U)
        {
            suffixState.stateType = WebServiceAtomState::FINISH_N;
            actionState[pAction->hash()][suffixState.hash()] = 1;
            posibility[pAction->hash()][suffixState.hash()]
                    = wsf.activities[s.activityId].blindService->reliability;

            suffixState.stateType = WebServiceAtomState::FINISH_U;
            actionState[pAction->hash()][suffixState.hash()] = 1;
            posibility[pAction->hash()][suffixState.hash()]
                    = MAX_POSIBILITY - wsf.activities[s.activityId].blindService->reliability;

            suffixState.stateType = WebServiceAtomState::FAIL;
            actionState[pAction->hash()][suffixState.hash()] = 1;
            posibility[pAction->hash()][suffixState.hash()]
                    = MAX_POSIBILITY - wsf.activities[s.activityId].blindService->reliability;
        }
        else if (s.stateType == WebServiceAtomState::FINISH_U)
        {
            suffixState.stateType = WebServiceAtomState::READY_U;
            actionState[pAction->hash()][suffixState.hash()] = 1;
            posibility[pAction->hash()][suffixState.hash()] = MAX_POSIBILITY;
        }
    }
}

WebServiceAction WebServiceSimulation::doNothing()
{
    WebServiceAction action;
    action.type = WebServiceAction::DO_NOTHING;
    action.dc = 0;
    action.dt = 0;
    return action;
}

void WebServiceSimulation::terminate(WebServiceAtomState & s)
{
    WebServiceAction action = terminate();
    WebServiceAction *pAction = getAction(action);
    if (pAction == NULL)
    {
        action.id = actionList.size();
        actionList.append(action);
        pAction = &action;
    }
    stateAction[s.hash()][pAction->hash()] = 1;

    QSet<int> suffix = WorkFlow::Instance()->getSuffixs(s.activityId);
    QSetIterator<int> it(suffix);
    while (it.hasNext())
    {
        int suffixId = it.next();
        WebServiceAtomState suffixState;
        suffixState.activityId = suffixId;
        suffixState.stateType = WebServiceAtomState::STOP; //Add here
        actionState[pAction->hash()][suffixState.hash()] = 1;
        posibility[pAction->hash()][suffixState.hash()] = MAX_POSIBILITY;
    }
}

WebServiceAction WebServiceSimulation::terminate()
{
    WebServiceAction action;
    action.type = WebServiceAction::TERMINATE;
    action.dc = Config::Instance()->getPuinishmentFailed();
    action.dt = 0;
    return action;
}

WebServiceAction* WebServiceSimulation::getAction(WebServiceAction & action)
{
    for (int i = 0; i < actionList.size(); i++)
    {
        if (actionList[i].hash() == action.hash())
        {
            return &actionList[i];
        }
    }
    return NULL;
}

QList<ReplaceNode> WebServiceSimulation::recomposeFreeService(int activityId)
{
    QList<ReplaceNode> res;
    QSet<int> suffix = WorkFlow::Instance()->getSuffixs(activityId);
    suffix.insert(activityId);
    QSetIterator<int> it(suffix);
    while (it.hasNext())
    {
        int id = it.next();
        ReplaceNode node;
        node.activityId = id;
        AtomService *newService = nextFreeService(id);
        if (newService != NULL)
        {
            node.oldServiceId = wsf.activities[activityId].blindService->id;
            node.newServiceId = newService->id;
            res.append(node);
        }
    }
    return res;
}

AtomService* WebServiceSimulation::nextFreeService(int activityId)
{
    QList<AtomService> & all_service = WorkFlow::Instance()->all_service;
    for (int i = 0; i < all_service.size(); i++)
    {
        if (all_service[i].activityNumber == activityId && all_service[i].free)
        {
            return &all_service[i];
        }
    }

    return NULL;
}

