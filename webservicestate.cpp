#include "webservicestate.h"
#include "activity.h"

#include "config.h"
#include "workflow.h"

WebServiceState::WebServiceState()
{
    init();
}

WebServiceState::~WebServiceState()
{
//    if (firstActivity != NULL)
//    {
//        delete[] firstActivity;
//    }
}

WebServiceState & WebServiceState::operator =(const WebServiceState & other)
{
//    if (firstActivity == NULL)
//    {
//        firstActivity = new Activity[WorkFlow::Instance()->getActivitySize()];
//    }
//    for (int i = 0; i < WorkFlow::Instance()->getActivitySize(); i++)
//    {
//        firstActivity[i] = other.firstActivity[i];
//        globalState = other.globalState;
//    }
}

bool WebServiceState::init()
{
//    firstActivity = NULL;
    globalState = NOT_READY;
    return true;
}

//Activity* WebServiceState::getFirstActivity()
//{
//    return firstActivity;
//}

//void WebServiceState::setFirstActivity(Activity *_firstActivity)
//{
//    if (firstActivity != NULL)
//    {
//        delete[] firstActivity;
//    }
//    int activitySize = WorkFlow::Instance()->getActivitySize();
//    firstActivity = new Activity[activitySize];
//    for (int i = 0; i < activitySize; i++)
//    {
//        firstActivity[i] = _firstActivity[i];
//    }
//}


int WebServiceState::getGlobalState()
{
    return globalState;
}

void WebServiceState::setGlobalState(int _globalState)
{
    globalState = _globalState;
}

int WebServiceState::valueOfState(int state)
{
    switch (state)
    {
    case NOT_READY:
    case READY:
        return 0;
    case EXEC:
        return 1;
    case FAIL:
    case FINISH:
    case CANCEL:
        return 2;
    default:
        return -1;
    }
}
