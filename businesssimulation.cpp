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
    for (int i = 0; i < workflowCount; i++) {
        runningActivities[i].insert(0);
        qDebug() << runningActivities[i] << finishedActivities[i];
        sg[i].show();
    }

    int t = 0;
    while (true)
    {
        qDebug() << "At t =" << t << " ...";
        //  �ж��Ƿ�ִ�н���
        bool flag = true;
        for (int i = 0; i < workflowCount; i++) {
            if (!runningActivities[i].isEmpty()) {
                flag = false;
            }
        }
        if (flag) {
            break;
        }

        // event

        // update show

        // sleep a moment

        // do sth

        // update show

        // sleep a moment & time passed & update show
        sleepAMoment();
        for (int i = 0; i < workflowCount; i++) {
            timePassed(activities[i], runningActivities[i], finishedActivities[i]);
            updatePainter(sg[i], runningActivities[i], finishedActivities[i]);
            qDebug() << "Flow" << i << ", runningActivities =" << runningActivities[i]
                     << "finishedActivities =" << finishedActivities[i];
        }

        // ��һ��
        t++;
    }
    qDebug() << "BusinessSimulation::run() finished.";
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
//        if (e.type == 0) { // ��Դ������

//        } else if (e.type == 1) { //��������

//        } else if (e.type == 2) { // �������

//        } else if (e.type == 3) { // ����ȡ��

//        } else {
//            qDebug() << "t = " << t << " Normal event.";
//        }

//        timePassed(t, startActivity, runningActivity, finishedActivity);
//        sleepAMoment();
////        updatePainter(runningActivity, finishedActivity);
//    }
//}

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

void BusinessSimulation::sleepAMoment()
{
    QTime dieTime = QTime::currentTime().addMSecs(100);
    while( QTime::currentTime() < dieTime ) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

// ������ʾ
void BusinessSimulation::updatePainter(ServiceGraph & sg, QSet<int> &runningActivity, QSet<int> &finishedActivity)
{
    for (int i = 0; i < WorkFlow::Instance()->getActivitySize(); i++) {
        if (finishedActivity.contains(i)) {
//            sg->colors[i].setRgb(0, 0, 255);
            sg.colors[i].setRgb(0, 0, 255);
        } else if (runningActivity.contains(i)) {
//            sg->colors[i].setRgb(255, 255, 0);
            sg.colors[i].setRgb(255, 255, 0);
        } else {

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
    QTextStream floStream(&file);
    QString line;
    QTextCodec *codec=QTextCodec::codecForName("GBK");
    floStream.setCodec(codec);
    line = codec->fromUnicode(floStream.readLine());
    line = line = codec->fromUnicode(floStream.readLine());
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
            line = codec->fromUnicode(floStream.readLine());
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
                }
            }
            for (int k = 0; k < all_resource.size(); k++)
            {
                if (all_resource[k].id == resourceId)
                {
                    activities[i][j].resource = &all_resource[k];
                }
            }
        }
        line = codec->fromUnicode(floStream.readLine());
    }
    file.close();

    qDebug() << "BusinessSimulation.init() finised.";
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
