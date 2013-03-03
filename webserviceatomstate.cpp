#include "webserviceatomstate.h"
#include "webservicestate.h"
#include "webserviceaction.h"

WebServiceAtomState::WebServiceAtomState()
{
}

QList<int> WebServiceAtomState::aviableActions(WebServiceAtomState &state)
{
    QList<int> res;
    switch (state.stateType)
    {
    case WebServiceState::NOT_READY:
        break;
    case WebServiceState::READY:
        res.append(WebServiceAction::TERMINATE);
        res.append(WebServiceAction::DO_NOTHING);
        res.append(WebServiceAction::SUBSTITUTE);
        res.append(WebServiceAction::RECOMPOSE);
        break;
    case WebServiceState::EXEC:
        break;
    case WebServiceState::FAIL:
        res.append(WebServiceAction::TERMINATE);
        res.append(WebServiceAction::RETRY);
        res.append(WebServiceAction::SUBSTITUTE);
        res.append(WebServiceAction::RECOMPOSE);
        break;
    case WebServiceState::FINISH:
        res.append(WebServiceAction::TERMINATE);
        res.append(WebServiceAction::DO_NOTHING);
        res.append(WebServiceAction::RECOMPOSE);
    case WebServiceState::CANCEL:
        break;
    default:
        break;
    }

    return res;
}
