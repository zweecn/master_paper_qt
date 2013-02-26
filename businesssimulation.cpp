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

#include <QApplication>
#include <QTime>
#include <QtGui>

const QString start_filename = "data/business_start";
const int start_member_size = 3;

//BusinessSimulation::BusinessSimulation(QWidget *parent) :
//    QWidget(parent)
BusinessSimulation::BusinessSimulation()
{
    init();
    initEarlyLateComplate();
}

BusinessSimulation::~BusinessSimulation()
{
    delete sg;
    for (int i = 0; i < workflowCount; i++) {
        delete[] activities[i];
    }
    delete[] activities;
}

void BusinessSimulation::run()
{
    qDebug() << "BusinessSimulation::run() begin...";
    sg = new ServiceGraph[workflowCount];
    QSet<int> *runningActivities = new QSet<int>[workflowCount];
    QSet<int> *finishedActivities = new QSet<int>[workflowCount];
    QSet<int> *bugActivities = new QSet<int>[workflowCount];
    for (int i = 0; i < workflowCount; i++) {
        runningActivities[i].insert(0);
        qDebug() << runningActivities[i] << finishedActivities[i];
        sg[i].show();
    }

    int t = 0;
    while (true)
    {
        // [0] if is finished ?
        qDebug() << "At t =" << t << " ...";
        bool flag = true;
        for (int i = 0; i < workflowCount; i++) {
            if (!runningActivities[i].isEmpty()) {
                flag = false;
            }
        }
        if (flag) {
            break;
        }

        // [1] event
        BusinessEvent event = BusinessEvent::random(t, activities, workflowCount);

        // [2] update show
        bugActivities[event.n].insert(event.a);
        for (int i = 0; i < workflowCount; i++) {
            updatePainter(sg[i], runningActivities[i], finishedActivities[i], bugActivities[i]);
        }

        // [3] sleep a moment
        sleepAMoment(100);

        actions = new BusinessAction[ACTIONS_COUNT];
        Activity * bug = &activities[event.n][event.a];
        // [4] do sth
        if (event.type == BusinessEvent::RESOUCE_NOT_USE) { // 资源不可用
            qDebug() << "RESOUCE_NOT_USE";
            resourceReplace(bug, runningActivities[event.n]);
            transResource(event.n, bug);
            termateDemand(bug, activities[event.n]);
        } else if (event.type == BusinessEvent::NEED_ADD) { //需求增加
            qDebug() << "NEED_ADD";
            resourceReplace(bug, runningActivities[event.n]);
            transResource(event.n, bug);
            termateDemand(bug, activities[event.n]);
        } else if (event.type == BusinessEvent::NEED_REDUCE) { // 需求减少
            doNothing(bug, activities[event.n]);
            qDebug() << "NEED_REDUCE";
        } else if (event.type == BusinessEvent::NEED_CANCEL) { // 需求取消
            termateDemand(bug, activities[event.n]);
            qDebug() << "NEED_CANCEL";
        } else {
            qDebug() << "Normal event";
        }
        for (int i = 0; i < workflowCount; i++) {
            bugActivities[i].clear();
        }

        BusinessAction *resAction = &actions[0];
        for (int i = 1; i < ACTIONS_COUNT; i++)
        {
            if (resAction->reward > actions[i].reward)
            {
                resAction = &actions[i];
            }
        }

        sleepAMoment(2000);
        // [5] update show
        for (int i = 0; i < workflowCount; i++) {
            updatePainter(sg[i], runningActivities[i], finishedActivities[i], bugActivities[i]);
        }

        // [6] sleep a moment & time passed & update show
        sleepAMoment();
        for (int i = 0; i < workflowCount; i++) {
            timePassed(activities[i], runningActivities[i], finishedActivities[i]);
            updatePainter(sg[i], runningActivities[i], finishedActivities[i], bugActivities[i]);
            qDebug() << "Flow" << i << ", runningActivities =" << runningActivities[i]
                     << "finishedActivities =" << finishedActivities[i];
        }

        // [7] next second
        t++;
    }

    delete[] actions;
    delete[] runningActivities;
    delete[] finishedActivities;
    delete[] bugActivities;

    qDebug() << "BusinessSimulation::run() finished.";
}

int BusinessSimulation::resourceReplace(Activity *bug, QSet<int> & running)
{
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
    int reward = resource->price;
//    qDebug() << "resource->price =" << reward << "bug->resource->price = " << bug->resource->price;
    if (!running.contains(bug->number)) {
        reward -= bug->resource->price;
    }
//    bug->resource = resource;
//    bug->resource->used++;

    actions[REPLACE_POS].bug = bug;
    actions[REPLACE_POS].type = BusinessAction::RESOURCE_REPLACE;
    actions[REPLACE_POS].reward = reward;

    actions[REPLACE_POS].replaceResource = resource;

    return reward;
}

int BusinessSimulation::termateDemand(Activity *bug, Activity *firstActivity)
{
    int res = 0;
    for (int i = 0; i < WorkFlow::Instance()->getActivitySize(); i++)
    {
        res += (firstActivity[i].resource->price
                + firstActivity[i].blindService->price);
    }

    actions[TERMINATE_POS].bug = bug;
    actions[TERMINATE_POS].type = BusinessAction::RESOURCE_TERMINATE_NEED;
    actions[TERMINATE_POS].reward = res;

    return res;
}

int BusinessSimulation::doNothing(Activity *bug, Activity *firstActivity)
{
    actions[DO_NOTHING_POS].bug = bug;
    actions[DO_NOTHING_POS].type = BusinessAction::RESOURCE_DO_NOTHING;
    actions[DO_NOTHING_POS].reward = 0;

    return 0;
}

int BusinessSimulation::transResource(int bugFlow, Activity *bug)
{
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
            IntervalCoverage *ic = new IntervalCoverage(workflowCount, data, bug->earlyStart, bug->lateComplate);
            ic->minCost();
            if (minCost < ic->getResult())
            {
                minCost = ic->getResult();
                minResult = ic->getResultDataID();
                otherFlow = i;
            }
            delete[] data;
            delete ic;
        }
    }

    actions[TRANS_POS].bug = bug;
    actions[TRANS_POS].type = BusinessAction::RESOURCE_TRANSPORT;
    actions[TRANS_POS].reward = minCost;

    actions[TRANS_POS].otherFlowId = otherFlow;
    actions[TRANS_POS].otherActivities = minResult;
    for (int i = 0; i < minResult.size(); i++) {
        actions[TRANS_POS].otherFlowResource
                .append(activities[otherFlow][minResult[i]].resource);
    }
    return minCost;
}

void BusinessSimulation::timePassed(Activity *startActivity, QSet<int> & runningActivity, QSet<int> & finishedActivity)
{
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
            // 加入后续的活动
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

void BusinessSimulation::sleepAMoment(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime ) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

// 更新显示
void BusinessSimulation::updatePainter(ServiceGraph & sg, QSet<int> &runningActivity,
                                       QSet<int> &finishedActivity, QSet<int> &bugActivity)
{
    for (int i = 0; i < WorkFlow::Instance()->getActivitySize(); i++) {
        if (finishedActivity.contains(i)) {
            //            sg->colors[i].setRgb(0, 0, 255);
            sg.colors[i].setRgb(0, 0, 255);
        } else if (runningActivity.contains(i)) {
            //            sg->colors[i].setRgb(255, 255, 0);
            sg.colors[i].setRgb(255, 255, 0);
        } else if (bugActivity.contains(i)) {
            sg.colors[i].setRgb(255, 0, 0);
        } else {
            sg.colors[i].setRgb(0, 255, 0);
        }
    }
    //    sg->update();
    sg.update();
}

void BusinessSimulation::printCurrState(int t, QSet<int> & runningActivity)
{
    qDebug() << "At " << t << ", running: " << runningActivity;
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

//void BusinessSimulation::oneFlowProcess()
//{
//    int t = 0;
//    Activity* startActivity = activities[0];
//    QSet<int> runningActivity;
//    QSet<int> finishedActivity;
//    runningActivity.insert(0);
//    while (!runningActivity.isEmpty() && ++t)
//    {
//        printCurrState(t, runningActivity);
//        BusinessEvent e = BusinessEvent::random(t);
//        if (e.type == 0) { // 资源不可用

//        } else if (e.type == 1) { //需求增加

//        } else if (e.type == 2) { // 需求减少

//        } else if (e.type == 3) { // 需求取消

//        } else {
//            qDebug() << "t = " << t << " Normal event.";
//        }

//        timePassed(t, startActivity, runningActivity, finishedActivity);
//        sleepAMoment();
////        updatePainter(runningActivity, finishedActivity);
//    }
//}
