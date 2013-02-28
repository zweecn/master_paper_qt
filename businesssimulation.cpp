#include "businesssimulation.h"
#include "activity.h"
#include "workflow.h"
#include <QFile>
#include <QTextCodec>
#include <QStringList>
#include <QDebug>
#include "criticalpath.h"
#include "businessevent.h"
#include "servicegraph.h"
#include "intervalcoverage.h"
#include "businessaction.h"
#include "intervalcoverage.h"
#include "businesseventwidget.h"
#include "businessactionwidget.h"

#include <QApplication>
#include <QTime>
#include <QtGui>

const QString start_filename = "data/business_start";
const int start_member_size = 3;

QMutex nextStepMutex;
QWaitCondition nextStepCond;

BusinessSimulation::BusinessSimulation() : QThread()
{
    init();
    initEarlyLateComplate();
}

BusinessSimulation::~BusinessSimulation()
{
    for (int i = 0; i < workflowCount; i++) {
        delete[] activities[i];
    }
    delete[] activities;

    delete[] actions;
    delete[] runningActivities;
    delete[] finishedActivities;
    delete[] bugActivities;
}

void BusinessSimulation::clearData()
{
    for (int i = 0; i < workflowCount; i++)
    {
        runningActivities[i].clear();
        finishedActivities[i].clear();
        bugActivities[i].clear();
    }
}


void BusinessSimulation::run()
{
    qDebug() << "BusinessSimulation::run() begin...";
    clearData();

    for (int i = 0; i < workflowCount; i++) {
        runningActivities[i].insert(0);
        qDebug() << runningActivities[i] << finishedActivities[i];
    }

    int t = 0;
    while (!isFinished())
    {
        nextStepMutex.lock();
        qDebug() << "At t =" << t << "...";

        // [1] event
        BusinessEvent event = BusinessEvent::random(t, activities, workflowCount);
        bew->setEvent(&event);

        // [2] update show
        bugActivities[event.n].insert(event.a);
        updatePainter();

        // [3]
        BusinessAction * action = operation(event);

        if (!isAutoRun && event.type != BusinessEvent::NORMAIL)
            nextStepCond.wait(&nextStepMutex);

        // [4]
        recovery(action);


//        sleepAMoment();
        sleep(1);
        // [5] update show
        updatePainter();

        // [6] sleep a moment & time passed & update show
//        sleepAMoment();
        sleep(1);
        timePassed();
        updatePainter();

        // [7] next second
        t++;
        nextStepMutex.unlock();
    }

    qDebug() << "BusinessSimulation::run() finished.";
}

bool BusinessSimulation::isFinished()
{
    bool flag = true;
    for (int i = 0; i < workflowCount; i++) {
        if (!runningActivities[i].isEmpty()) {
            flag = false;
        }
    }
    if (flag) {
        return true;
    }
    return false;
}

BusinessAction* BusinessSimulation::operation(BusinessEvent &event)
{
    // [4] do sth
    if (event.type == BusinessEvent::RESOUCE_NOT_USE) { // ��Դ������
        qDebug() << "RESOUCE_NOT_USE";
        resourceReplace(event);
        termateDemand(event);
        actions[BusinessAction::RESOURCE_REPLACE].isActive = true;
        actions[BusinessAction::RESOURCE_TRANSPORT].isActive = transResource(event);
        actions[BusinessAction::RESOURCE_TERMINATE_NEED].isActive = true;

        actions[BusinessAction::RESOURCE_DO_NOTHING].isActive = false;
        actions[BusinessAction::RESOURCE_ADD_NEW_NEED].isActive = false;
    } else if (event.type == BusinessEvent::NEED_ADD) { //��������
        qDebug() << "NEED_ADD";
        resourceReplace(event);
        termateDemand(event);

        actions[BusinessAction::RESOURCE_REPLACE].isActive = true;
        actions[BusinessAction::RESOURCE_TRANSPORT].isActive = transResource(event);
        actions[BusinessAction::RESOURCE_TERMINATE_NEED].isActive = true;

        actions[BusinessAction::RESOURCE_DO_NOTHING].isActive = false;
        actions[BusinessAction::RESOURCE_ADD_NEW_NEED].isActive = false;
    } else if (event.type == BusinessEvent::NEED_REDUCE) { // �������
        doNothing(event);
        actions[BusinessAction::RESOURCE_DO_NOTHING].isActive = true;

        actions[BusinessAction::RESOURCE_REPLACE].isActive = false;
        actions[BusinessAction::RESOURCE_TRANSPORT].isActive = false;
        actions[BusinessAction::RESOURCE_TERMINATE_NEED].isActive = false;
        actions[BusinessAction::RESOURCE_ADD_NEW_NEED].isActive = false;
        qDebug() << "NEED_REDUCE";
    } else if (event.type == BusinessEvent::NEED_CANCEL) { // ����ȡ��
        termateDemand(event);
        actions[BusinessAction::RESOURCE_TERMINATE_NEED].isActive = true;

        actions[BusinessAction::RESOURCE_DO_NOTHING].isActive = false;
        actions[BusinessAction::RESOURCE_REPLACE].isActive = false;
        actions[BusinessAction::RESOURCE_TRANSPORT].isActive = false;
        actions[BusinessAction::RESOURCE_ADD_NEW_NEED].isActive = false;
        qDebug() << "NEED_CANCEL";
    } else {
        qDebug() << "Normal event";
        actions[BusinessAction::RESOURCE_TERMINATE_NEED].isActive = false;
        actions[BusinessAction::RESOURCE_DO_NOTHING].isActive = false;
        actions[BusinessAction::RESOURCE_REPLACE].isActive = false;
        actions[BusinessAction::RESOURCE_TRANSPORT].isActive = false;
        actions[BusinessAction::RESOURCE_ADD_NEW_NEED].isActive = false;
    }
    baw->setBusinessAction(actions);

    BusinessAction *resAction = NULL;
    for (int i = 0; i < BusinessAction::ACTIONS_COUNT; i++)
    {
        if (actions[i].isActive && (resAction == NULL || resAction->reward < actions[i].reward))
        {
            resAction = &actions[i];
        }
    }

    baw->setAutoBusinessAction(resAction);
    return resAction;
}

bool BusinessSimulation::recovery(BusinessAction *action)
{

    if (action == NULL)
    {
        return false;
    }
    if (action->type == BusinessAction::RESOURCE_REPLACE)
    {
        action->bug->resource = action->newResource;
    }
    // bug, add resourList in activity;
    else if (action->type == BusinessAction::RESOURCE_TRANSPORT)
    {
        action->bug->resourceList = action->otherFlowResource;
        workflowState[action->event->n] = WORKFLOW_FAILED;
    }
    else if (action->type == BusinessAction::RESOURCE_TERMINATE_NEED)
    {
        workflowState[action->event->n] = WORKFLOW_FAILED;
    }
    else if (action->type == BusinessAction::RESOURCE_DO_NOTHING)
    {

    }
    else if (action->type == BusinessAction::RESOURCE_ADD_NEW_NEED)
    {

    }

    for (int i = 0; i < workflowCount; i++) {
//        if (workflowState[i] != WORKFLOW_FAILED)
            bugActivities[i].clear();
    }
    return true;
}

int BusinessSimulation::resourceReplace(BusinessEvent & event)
{
    Activity *bug = &activities[event.n][event.a];
    QSet<int> & running = runningActivities[event.n];

    QList<Resource> & all_resource = WorkFlow::Instance()->all_resource;
    Resource* resource = NULL;
    for (int i = 0; i < all_resource.size(); i++)
    {
        if (all_resource[i].type == bug->resource->type
                && all_resource[i].used < all_resource[i].use_type
                && all_resource[i].id != bug->resource->id)
        {
            if (resource == NULL) {
                resource = &all_resource[i];
            } else if (all_resource[i].price < resource->price) {
                resource = &all_resource[i];
            }
        }
    }
    qDebug() << "Old = " << bug->resource->id << "New = " << resource->id;
    // Calcacute the reward and return it
    int cost = resource->price;
    if (!running.contains(bug->number)) {
        cost = cost + bug->resource->price;
    }

//    bug->resource = resource;
//    bug->resource->used++;

    actions[BusinessAction::RESOURCE_REPLACE].bug = bug;
    actions[BusinessAction::RESOURCE_REPLACE].type = BusinessAction::RESOURCE_REPLACE;
    actions[BusinessAction::RESOURCE_REPLACE].event = &event;
    actions[BusinessAction::RESOURCE_REPLACE].reward = -cost;

    actions[BusinessAction::RESOURCE_REPLACE].oldResource = bug->resource;
    actions[BusinessAction::RESOURCE_REPLACE].newResource = resource;


//    qDebug() << bug->resource << resource;
    return cost;
}

bool BusinessSimulation::transResource(BusinessEvent & event)
{
    int bugFlow = event.n;
    Activity *bug = &activities[event.n][event.a];

    int minCost = INT_MAX;
    int otherFlow = -1;
    QList<int> minResult;
    for (int i = 0; i < workflowCount; i++)
    {
        if (i == bugFlow)
        {

        }
        else
        {
            SegMent *data = toSegMent(activities[i]);
            IntervalCoverage *ic = new IntervalCoverage(workflowCount, data,
                                                        bug->earlyStart, bug->lateComplate);
            ic->minCost();
            if (minCost > ic->getResult())
            {
                minCost = ic->getResult();
                minResult = ic->getResultDataID();
                otherFlow = i;
            }
            delete[] data;
            delete ic;
        }
    }

    if (minCost == -1)
    {
        minCost = INT_MAX;
    }

    actions[BusinessAction::RESOURCE_TRANSPORT].bug = bug;
    actions[BusinessAction::RESOURCE_TRANSPORT].type = BusinessAction::RESOURCE_TRANSPORT;
    actions[BusinessAction::RESOURCE_TRANSPORT].event = &event;
    actions[BusinessAction::RESOURCE_TRANSPORT].reward = -minCost;

    actions[BusinessAction::RESOURCE_TRANSPORT].otherFlowId = otherFlow;
    actions[BusinessAction::RESOURCE_TRANSPORT].otherFlowActivities = minResult;
    actions[BusinessAction::RESOURCE_TRANSPORT].otherFlowResource.clear();
    for (int i = 0; i < minResult.size(); i++) {
        actions[BusinessAction::RESOURCE_TRANSPORT].otherFlowResource
                .append(activities[otherFlow][minResult[i]].resource);
    }

    actions[BusinessAction::RESOURCE_TRANSPORT].newResource = NULL;

    return (minCost != INT_MAX);
}

int BusinessSimulation::termateDemand(BusinessEvent & event)
{
    Activity *bug = &activities[event.n][event.a];
    Activity *firstActivity = activities[event.n];

    int cost = 0;
    for (int i = 0; i < WorkFlow::Instance()->getActivitySize(); i++)
    {
        cost += (firstActivity[i].resource->price
                + firstActivity[i].blindService->price);
    }

    actions[BusinessAction::RESOURCE_TERMINATE_NEED].bug = bug;
    actions[BusinessAction::RESOURCE_TERMINATE_NEED].type = BusinessAction::RESOURCE_TERMINATE_NEED;
    actions[BusinessAction::RESOURCE_TERMINATE_NEED].event = &event;
    actions[BusinessAction::RESOURCE_TERMINATE_NEED].reward = -cost;

    return cost;
}

int BusinessSimulation::doNothing(BusinessEvent & event)
{
    Activity *bug = &activities[event.n][event.a];

    actions[BusinessAction::RESOURCE_DO_NOTHING].bug = bug;
    actions[BusinessAction::RESOURCE_DO_NOTHING].type = BusinessAction::RESOURCE_DO_NOTHING;
    actions[BusinessAction::RESOURCE_DO_NOTHING].event = &event;
    actions[BusinessAction::RESOURCE_DO_NOTHING].reward = 0;

    return 0;
}


void BusinessSimulation::timePassed(int flowId)
{
    Activity *startActivity = activities[flowId];
    QSet<int> & runningActivity = runningActivities[flowId];
    QSet<int> & finishedActivity = finishedActivities[flowId];

    std::vector<std::vector<int> > g = toGraph(startActivity);
    CriticalPath cp(WorkFlow::Instance()->getActivitySize(), g);
    cp.run();
    QSetIterator<int> it(runningActivity);
    while (it.hasNext())
    {
        int tmpNum = it.next();
        Activity* tmp = &startActivity[tmpNum];
        tmp->x += 1.0/cp.getLatestTime();
        if (tmp->x >= 1.0) {
            tmp->x = 1.0;
            tmp->state = Activity::FINISHED;
            finishedActivity.insert(tmpNum);
            // ��������Ļ
            for (int x = 0; x < WorkFlow::Instance()->getActivitySize(); x++) {
                QSet<int> prefixs = WorkFlow::Instance()->getPrefixs(x);
                if ((prefixs - finishedActivity).isEmpty()) {
                    runningActivity.insert(x);
                }
            }
        }
    }
    it = finishedActivity;
    while (it.hasNext()) {
        runningActivity.remove(it.next());
    }
}

void BusinessSimulation::timePassed()
{
    for (int i = 0; i < workflowCount; i++) {
//        if (workflowState[i] == WORKFLOW_FAILED)
//            continue;
        timePassed(i);
        qDebug() << "Flow" << i << ", runningActivities =" << runningActivities[i]
                 << "finishedActivities =" << finishedActivities[i];
    }
}

// ������ʾ
void BusinessSimulation::updatePainter(int flowId, ServiceGraph & sg)
{
    QSet<int> & runningActivity = runningActivities[flowId];
    QSet<int> & finishedActivity = finishedActivities[flowId];
    QSet<int> & bugActivity = bugActivities[flowId];

    QList<QColor> colors = sg.getColors();
    for (int i = 0; i < WorkFlow::Instance()->getActivitySize(); i++) {
        if (finishedActivity.contains(i)) {
            colors[i].setRgb(0, 0, 255);
        } else if (runningActivity.contains(i)) {
            colors[i].setRgb(255, 255, 0);
        } else if (bugActivity.contains(i)) {
            colors[i].setRgb(255, 0, 0);
        } else {
            colors[i].setRgb(0, 255, 0);
        }
    }
    sg.setColors(colors);
}

void BusinessSimulation::updatePainter()
{
    for (int i = 0; i < workflowCount; i++) {
        updatePainter(i, sg[i]);
    }
}

void BusinessSimulation::sleepAMoment(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime ) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}


void BusinessSimulation::printCurrState(int t, int flowId)
{
    qDebug() << "At " << t << ", running: " << runningActivities[flowId];
}

bool BusinessSimulation::init()
{
    qDebug() << "BusinessSimulation.init() begin...";
    //    connect(sg, SIGNAL(destroyed()), this, SLOT(deleteLater()));
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
    workflowCount = line.toInt();
    if (workflowCount <= 0)
    {
        qDebug() << "BusinessSimulation::init() workflowCount = 0";
        return false;
    }
    activities = new Activity*[workflowCount];
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

    workflowState = new int[workflowCount];
    for (int i = 0; i < workflowCount; i++)
    {
        workflowState[i] = WORKFLOW_READY;
    }

    runningActivities = new QSet<int>[workflowCount];
    finishedActivities = new QSet<int>[workflowCount];
    bugActivities = new QSet<int>[workflowCount];
    actions = new BusinessAction[BusinessAction::ACTIONS_COUNT];
    for (int i = 0; i < BusinessAction::ACTIONS_COUNT; i++)
    {
        actions[i].type = i;
    }

    qDebug() << "BusinessSimulation.init() finised.";
    return true;

}

bool BusinessSimulation::initEarlyLateComplate()
{
    qDebug() << "BusinessSimulation::initEarlyLateComplate() begin..." << workflowCount;
    for (int i = 0; i < workflowCount; i++)
    {
        Activity* startActivity = activities[i];
        std::vector<std::vector<int> > g = toGraph(startActivity);
        CriticalPath cp(WorkFlow::Instance()->getActivitySize(), g);
        cp.run();
        for (int j = 0; j < WorkFlow::Instance()->getActivitySize(); j++)
        {
            startActivity[j].earlyStart = cp.getEarlyStartTime(startActivity, j);
            startActivity[j].lateStart = cp.getLateStartTime(startActivity, j);
            startActivity[j].earlyComplate = cp.getEarlyComplateTime(startActivity, j);
            startActivity[j].lateComplate = cp.getLateComplateTime(startActivity, j);
        }
    }

    qDebug() << "BusinessSimulation::initEarlyLateComplate() finished.";
    return true;
}

std::vector<std::vector<int> > BusinessSimulation::toGraph(Activity* a)
{
    std::vector<std::vector<int> > g;
    int n = WorkFlow::Instance()->getActivitySize();
    g.assign(n, std::vector<int>(n, INT_MAX));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (WorkFlow::Instance()->hasEdge(i, j))
            {
                g[i][j] = a[j].blindService->execTime;
            }
        }
    }

    return g;
}

SegMent* BusinessSimulation::toSegMent(Activity *a)
{
    SegMent *data = new SegMent[workflowCount];
    for (int i = 0; i < workflowCount; i++)
    {
        data[i].id = i;
        data[i].left = a[i].earlyStart;
        data[i].right = a[i].lateComplate;
        data[i].price = a[i].resource->price;
    }
    return data;
}

int BusinessSimulation::getWorkflowCount()
{
    return workflowCount;
}

void BusinessSimulation::setServiceGraph(ServiceGraph * _sg)
{
    this->sg = _sg;
}

ServiceGraph* BusinessSimulation::getServiceGraph()
{
    return sg;
}

void BusinessSimulation::setBusinessEventWidget(BusinessEventWidget *_bew)
{
    bew = _bew;
}

BusinessEventWidget * BusinessSimulation::getBusinessEventWidget()
{
    return bew;
}

void BusinessSimulation::setBusinessActionWidget(BusinessActionWidget *_baw)
{
    baw = _baw;
}

BusinessActionWidget * BusinessSimulation::getBusinessActionWidget()
{
    return baw;
}

void BusinessSimulation::setAutoRun(bool _isAutoRun)
{
    isAutoRun = _isAutoRun;
}
