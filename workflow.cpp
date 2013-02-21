#include "workflow.h"
#include <QFile>
#include <QDebug>
#include <QTextCodec>
#include <QList>
#include <QStringList>
#include "atomservice.h"
#include "resource.h"

const QString workflow_filename = "data/graph";
const QString service_filename = "data/services";
const QString resource_filename = "data/resource";
const int service_member_size = 7;
const int resource_member_size = 4;

WorkFlow* WorkFlow::_instance = 0;

WorkFlow::WorkFlow()
{
    readGraphInfo();
    initPrefixSuffix();
    readService();
    readResource();
}

bool WorkFlow::readGraphInfo()
{
    QFile file(workflow_filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Can not open service file.";
        return false;
    }
    QTextStream floStream(&file);
    QString line;
    QTextCodec *codec=QTextCodec::codecForName("GBK");
    floStream.setCodec(codec);
    line = codec->fromUnicode(floStream.readLine());
    line = codec->fromUnicode(floStream.readLine());
    activitySize = line.toInt();
    graph = new int*[activitySize];
    for (int i = 0; i < activitySize; i++)
    {
        graph[i] = new int[activitySize];
        memset(graph[i], 0, sizeof(int) * activitySize);
    }
    while (!floStream.atEnd())
    {
        line = codec->fromUnicode(floStream.readLine());
        QStringList list = line.split("->");
        if (list.size() != 2) {
            continue;
        }
        int prefix = list[0].toInt();
        int suffix = list[1].toInt();
        graph[prefix][suffix] = 1;
    }
    file.close();

    qDebug() << "readGraphInfo finised.";
    return true;
}

bool WorkFlow::initPrefixSuffix()
{
    for (int i = 0; i < activitySize; i++) {
        QList<int> prefixTempList;
        QList<int> suffixTempList;
        for (int j = 0; j < activitySize; j++) {
            if (graph[i][j] == 1) {
                suffixTempList.append(j);
            }
            if (graph[j][i] == 1) {
                prefixTempList.append(j);
            }
        }

        suffixMap[i] = suffixTempList;
        prefixMap[i] = prefixTempList;
    }
    return true;
}

bool WorkFlow::readService()
{
    QFile file(service_filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Can not open service file.";
        return false;
    }
    QTextStream floStream(&file);
    QString line;
    QTextCodec *codec=QTextCodec::codecForName("GBK");
    floStream.setCodec(codec);
    line = codec->fromUnicode(floStream.readLine());
//    qDebug() << title;
    while (!floStream.atEnd())
    {
        line = codec->fromUnicode(floStream.readLine());
        QStringList list = line.split("\t");
        if (list.size() != service_member_size - 2) {
            continue;
        }
        AtomService tempService;
        tempService.free = true;
        tempService.redoCount = 0;
        tempService.activityNumber = list[0].toInt();
        tempService.id = list[1].toInt();
        tempService.price = list[2].toInt();
        tempService.reliability = list[3].toDouble();
        tempService.execTime = list[4].toInt();
        all_service.append(tempService);
//        qDebug() << line;
//        qWarning(line);
    }
    file.close();

    qDebug() << "In readService, all_service.size = " << all_service.size();

    return true;
}

bool WorkFlow::readResource()
{
    QFile file(resource_filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Can not open resource file.";
        return false;
    }
    QTextStream floStream(&file);
    QString line;
    QTextCodec *codec=QTextCodec::codecForName("GBK");
    floStream.setCodec(codec);
    line = codec->fromUnicode(floStream.readLine());
//    qDebug() << title;
    while (!floStream.atEnd())
    {
        line = codec->fromUnicode(floStream.readLine());
        QStringList list = line.split("\t");
        if (list.size() != resource_member_size) {
            continue;
        }
        Resource r;
        r.id = list[0].toInt();
        r.type = list[1].toInt();
        r.use_type = list[2].toInt();
        r.type = list[3].toInt();
        all_resource.append(r);
//        qDebug() << line;
//        qWarning(line);
    }
    file.close();

    qDebug() << "In readResource, all_resource.size = " << all_resource.size();
    return true;
}

int WorkFlow::getActivitySize()
{
    return activitySize;
}

bool WorkFlow::hasEdge(int i, int j)
{
    return (graph[i][j] == 1);
}
