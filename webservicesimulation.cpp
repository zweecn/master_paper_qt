#include "webservicesimulation.h"
//#include "webservicestate.h"
//#include "webserviceaction.h"
#include "resource.h"
#include "workflow.h"
#include "activity.h"

#include <QDebug>
#include <QFile>
#include <QIODevice>
#include <QTextCodec>
#include <QStringList>
#include <qmath.h>
#include <QSet>

const QString start_filename = "data/business_start";
const int start_member_size = 3;


WebServiceSimulation::WebServiceSimulation()
{
}

WebServiceSimulation::~WebServiceSimulation()
{
//    delete[] actions;
}

void WebServiceSimulation::run()
{
    init();
}

bool WebServiceSimulation::markov()
{
//    QList<UTG> utgList;
//    WSState currState;
//    UTG u;
//    u.rootState = currState;
//    QSet<int> suffix = WorkFlow::Instance()->getSuffixs(currState.activityId);

    return true;
}

//WebServiceActionResultNode WebServiceSimulation::terminate()
//{
//    WebServiceActionResultNode node(currState);
//    node.p[WebServiceState::FAIL] = 1;
//    node.states[WebServiceState::FAIL].setGlobalState(WebServiceState::FAIL);
//    for (int i = 0; i < WorkFlow::Instance()->getActivitySize(); i++)
//    {

//    }

//    return node;
//}

//    NOT_READY = 0,
//    READY = 1,
//    EXEC = 2,
//    FAIL = 3,
//    FINISH = 4,
//    CANCEL = 5,
//    STATE_COUNT = 6

//WebServiceActionResultNode WebServiceSimulation::doNothing()
//{
//    WebServiceActionResultNode node(currState);
//    node.p[WebServiceState::FAIL] = 1;


//    return node;
//}

//WebServiceActionResultNode WebServiceSimulation::retry()
//{

//    WebServiceActionResultNode node(currState);


//    return node;
//}

//WebServiceActionResultNode WebServiceSimulation::substitute()
//{
//    WebServiceActionResultNode node(currState);


//    return node;
//}

//WebServiceActionResultNode WebServiceSimulation::recompose()
//{
//    WebServiceActionResultNode node(currState);


//    return node;
//}

bool WebServiceSimulation::init()
{
    qDebug() << "WebServiceSimulation::init() begin...";
    QFile file(start_filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Can not open start_filename file.";
        return false;
    }
    QTextStream stream(&file);
    QString line;
    QTextCodec *codec=QTextCodec::codecForName("GBK");
    stream.setCodec(codec);
    line = codec->fromUnicode(stream.readLine());
    line = line = codec->fromUnicode(stream.readLine());
    int workflowCount = line.toInt();
    if (workflowCount <= 0)
    {
        qDebug() << "WebServiceSimulation::init() workflowCount = 0";
        return false;
    }
    Activity** activities = new Activity*[workflowCount];
    for (int i = 0; i < workflowCount; i++)
    {
        int activitySize = WorkFlow::Instance()->getActivitySize();
        activities[i] = new Activity[activitySize];
        for (int j = 0; j < activitySize; j++)
        {
            line = codec->fromUnicode(stream.readLine());
            QStringList list = line.split("\t");
            if (list.size() != start_member_size) {
                continue;
            }
            activities[i][j].number = list[0].toInt();
            int serviceId = list[1].toInt();
            int resourceId = list[2].toInt();
            QList<AtomService> &all_service = WorkFlow::Instance()->all_service;
            QList<Resource> &all_resource = WorkFlow::Instance()->all_resource;
            for (int k = 0; k < all_service.size(); k++)
            {
                if (all_service[k].id == serviceId)
                {
                    activities[i][j].blindService = &all_service[k];
                    activities[i][j].blindService->free = false;
                }
            }
            for (int k = 0; k < all_resource.size(); k++)
            {
                if (all_resource[k].id == resourceId)
                {
                    activities[i][j].resource = &all_resource[k];
                    activities[i][j].resource->used++;
                }
            }
        }
        line = codec->fromUnicode(stream.readLine());
    }
    file.close();

//    firstActivity = activities[0];
//    actions = new WebServiceAction[WebServiceAction::ACTION_COUNT];

//    stateSize = pow(WebServiceState::STATE_COUNT, WorkFlow::Instance()->getActivitySize());
//    qDebug() << WebServiceState::STATE_COUNT << WorkFlow::Instance()->getActivitySize() << stateSize;

    qDebug() << "WebServiceSimulation::init() finised.";
    return true;

}
