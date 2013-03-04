#include "markovaction.h"
#include "workflow.h"
#include "markovstate.h"
#include "markovrecord.h"

MarkovAction::MarkovAction()
{
}

QString MarkovAction::name()
{
    QString res;
    switch (type)
    {
    case ERROR_ACTION:
        res = "ERROR_ACTION";
        break;
    case A_NO_ACTION:
        res = "A_NO_ACTION";
        break;
    case A_TERMINATE:
        res = "A_TERMINATE";
        break;
    case A_RE_DO:
        res = "A_RE_DO";
        break;
    case A_REPLACE:
        res = "A_REPLACE";
        break;
    case A_RE_COMPOSITE:
        res = "A_RE_COMPOSITE";
        break;
    default:
        break;
    }
    return res;
}

AtomService* MarkovAction::nextFreeService(int activityId)
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

// Lack of old service
QList<RecomposeNode> MarkovAction::recomposeFreeService(int activityId)
{
    QList<RecomposeNode> res;
    QSet<int> suffix = WorkFlow::Instance()->getSuffixs(activityId);
    suffix.insert(activityId);
    QSetIterator<int> it(suffix);
    while (it.hasNext())
    {
        int id = it.next();
        RecomposeNode node;
        node.activityId = id;
        node.newService = nextFreeService(id);
        if (node.newService != NULL)
        {
            res.append(node);
        }
    }
    return res;
}

MarkovAction & MarkovAction::operator =(const MarkovAction & other)
{
    if (this == &other)
    {
        return *this;
    }

    type = other.type;
    bugActivityId = other.bugActivityId;

    oldService = other.oldService;
    newService = other.newService;

    oldNewServiceList.clear();
    for (int i = 0; i < other.oldNewServiceList.size(); i++)
    {
        RecomposeNode node;
        node.activityId = other.oldNewServiceList[i].activityId;
        node.oldService = other.oldNewServiceList[i].oldService;
        node.newService = other.oldNewServiceList[i].newService;
        oldNewServiceList.append(node);
    }

    //    replaceActivityList = other.replaceActivityList;
    //    oldServiceList = other.oldServiceList;
    //    newServiceList = other.newServiceList;

    return *this;
}

bool MarkovAction::operator ==(const MarkovAction & other) const
{
    if (type != other.type)
    {
        return false;
    }
    if (bugActivityId != other.bugActivityId)
    {
        return false;
    }
    if (oldService != other.oldService)
    {
        return false;
    }
    if (newService != other.newService)
    {
        return false;
    }
    if (oldNewServiceList.size() != other.oldNewServiceList.size())
    {
        return false;
    }
    for (int i = 0; i < other.oldNewServiceList.size(); i++)
    {
        if (oldNewServiceList[i].activityId != other.oldNewServiceList[i].activityId)
        {
            return false;
        }
    }
    return true;
}

bool MarkovAction::operator <(const MarkovAction & other) const
{
    for (int i = 0; i < other.oldNewServiceList.size(); i++)
    {
        if (!(oldNewServiceList[i].activityId < other.oldNewServiceList[i].activityId))
        {
            return false;
        }
    }
    return ((type < other.type)
            && (bugActivityId < other.bugActivityId)
            && (oldService < other.oldService)
            && (newService < other.newService));
}

MarkovAction MarkovAction::noAction(MarkovState & state)
{
    MarkovAction action;
    action.bugActivityId = state.nextToDoActivity->number;
    if (state.isFinished())
    {
        action.type = ERROR_ACTION;
    }
    else
    {
        action.type = A_NO_ACTION;
    }
    return action;
}

MarkovAction MarkovAction::terminate(MarkovState & state)
{
    MarkovAction action;
    action.bugActivityId = state.nextToDoActivity->number;
    action.type = A_TERMINATE;
    action.oldService = state.nextToDoActivity->blindService;
    state.setFailed(true);
    return action;
}

MarkovAction MarkovAction::redo(MarkovState & state)
{
    MarkovAction action;
    action.bugActivityId = state.faultActivity->number;
    action.type = A_RE_DO;
    action.newService = action.oldService = state.faultActivity->blindService;
    state.faultActivity->x = 0;
    state.init();
    return action;
}

MarkovAction MarkovAction::replace(MarkovState & state)
{
    MarkovAction action;
    AtomService* newFreeService = nextFreeService(state.faultActivity->number);
    if (newFreeService == NULL)
    {
        action.type = ERROR_ACTION;
        return action;
    }

    action.oldService = state.faultActivity->blindService;
    action.newService = state.faultActivity->blindService = newFreeService;
    newFreeService->free = false;
    state.faultActivity->x = 0;
    state.init();
    return action;
}

MarkovAction MarkovAction::reComposite(MarkovState & state)
{
    MarkovAction action;
    action.oldNewServiceList = recomposeFreeService(state.faultActivity->number);
    for (int i = 0; i < action.oldNewServiceList.size(); i++)
    {
        action.oldNewServiceList[i].oldService = state
                .activities[action.oldNewServiceList[i].activityId].blindService;
        action.oldNewServiceList[i].newService->free = false;
    }
    state.faultActivity->x = 0;
    return action;
}

double MarkovAction::getReplacePosibility()
{
    return newService->reliability;
}

// Bugs
double MarkovAction::getReplacePriceCost()
{
    return newService->price;
}

double MarkovAction::getReplaceTimeCost()
{
    return newService->execTime - oldService->execTime;
}

double MarkovAction::getReComposePosibility()
{
    double res = 1;
    for (int i = 0; i < oldNewServiceList.size(); i++)
    {
        res *= oldNewServiceList[i].newService->reliability;
    }
    return res;
}

// Bugs
double MarkovAction::getReComposePriceCost()
{
    double res = 0;
    for (int i = 0; i < oldNewServiceList.size(); i++)
    {
        res += oldNewServiceList[i].newService->price - oldNewServiceList[i].oldService->price;
    }
    return res;
}

// Bugs
double MarkovAction::getReComposeTimeCost()
{
    double res = 0;
    for (int i = 0; i < oldNewServiceList.size(); i++)
    {
        res += oldNewServiceList[i].newService->execTime - oldNewServiceList[i].oldService->execTime;
    }
    return res;
}

