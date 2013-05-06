#include "webserviceflow.h"
#include "resource.h"
#include "workflow.h"
#include "activity.h"
#include "webserviceatomstate.h"

#include <QDebug>
#include <QFile>
#include <QIODevice>
#include <QTextCodec>
#include <QStringList>
#include <qmath.h>
#include <QSet>

const QString start_filename = "data/ws_start";
const int start_member_size = 3;

WebServiceFlow::WebServiceFlow()
{
    init();
}

bool WebServiceFlow::init()
{
//    qDebug() << "WebServiceFlow::init() begin...";
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
    int activitySize = WorkFlow::Instance()->getActivitySize();
    activities = new Activity[activitySize];
    for (int j = 0; j < activitySize; j++)
    {
        line = codec->fromUnicode(stream.readLine());
        QStringList list = line.split("\t");
        if (list.size() != start_member_size) {
            continue;
        }
        activities[j].number = list[0].toInt();
        int serviceId = list[1].toInt();
        int resourceId = list[2].toInt();
        QList<AtomService> &all_service = WorkFlow::Instance()->all_service;
        QList<Resource> &all_resource = WorkFlow::Instance()->all_resource;
        for (int k = 0; k < all_service.size(); k++)
        {
            if (all_service[k].id == serviceId)
            {
                activities[j].blindService = &all_service[k];
                activities[j].blindService->free = false;
            }
        }
        for (int k = 0; k < all_resource.size(); k++)
        {
            if (all_resource[k].id == resourceId)
            {
                activities[j].resource = &all_resource[k];
                activities[j].resource->used++;
            }
        }
    }
    file.close();
    globalState = WebServiceAtomState::READY_N;
//    qDebug() << "WebServiceFlow::init() finised.";
    return true;
}

WebServiceFlow & WebServiceFlow::operator = (const WebServiceFlow & other)
{
    if (this == &other)
    {
        return *this;
    }
    for (int i = 0; i < WorkFlow::Instance()->getActivitySize(); i++)
    {
        activities[i] = other.activities[i];
    }
    return *this;
}

QString WebServiceFlow::name()
{
    QString res;
    if (globalState == WebServiceAtomState::STOP)
    {
        res = "STOP";
    }
    else if (globalState == WebServiceAtomState::EXEC)
    {
        res = "EXEC";
    }
    else if (globalState == WebServiceAtomState::READY_N)
    {
        res = "READY_N";
    }
    else if (globalState == WebServiceAtomState::READY_U)
    {
        res = "READU_U";
    }
    else if (globalState == WebServiceAtomState::FINISH_N)
    {
        res = "FINISH_N";
    }
    else if (globalState == WebServiceAtomState::FINISH_U)
    {
        res = "FINISH_U";
    }
    else if (globalState == WebServiceAtomState::FAIL)
    {
        res = "FAIL";
    }
    else
    {
        res = "NOT_STATE";
    }
    return res;
}

QString WebServiceFlow::toString()
{
    QString res = "[";
    for (int i = 0; i < WorkFlow::Instance()->getActivitySize(); i++)
    {
        res += QString("A%1:S%2,X%3 ")
                .arg(activities[i].number)
                .arg(activities[i].blindService->id)
                .arg(activities[i].x);
    }
    res = res.trimmed() + "]";
    return res;
}

bool WebServiceFlow::isFinished()
{
    if (globalState == WebServiceAtomState::STOP
            || globalState == WebServiceAtomState::FINISH_N
            || globalState == WebServiceAtomState::FINISH_U)
    {
        qDebug() << "WebServiceFlow::isFinished()... " << globalState;
        return true;
    }
    for (int i = 0; i < WorkFlow::Instance()->getActivitySize(); i++)
    {
        if (activities[i].x < 1.0)
        {
            return false;
        }
    }
    return true;
}
