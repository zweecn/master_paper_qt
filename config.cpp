#include "config.h"
#include <QFile>
#include <QStringList>
#include <QTextCodec>
#include <QTextStream>
#include <QDebug>

const QString conf_filename = "data/conf";

Config * Config::_instance = 0;

Config::Config()
{
    init();
}

bool Config::init()
{
    QFile file(conf_filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Can not open resource file.";
        return false;
    }
    QTextStream floStream(&file);
    QString line;
    QTextCodec *codec=QTextCodec::codecForName("GBK");
    floStream.setCodec(codec);
    while (!floStream.atEnd())
    {
        line = codec->fromUnicode(floStream.readLine());
        QStringList list = line.split("=");
        if (list.size() != 2) {
            continue;
        }
        confMap[list[0]] = list[1];
    }
    file.close();

    qDebug() << "Config::init() finished.";
    return true;
}

//int Config::getUCTypeCount()
//{
//    return confMap["UC_TYPE_COUNT"].toInt();
//}

int Config::getWorkflowCount()
{
    return confMap["WORKFLOW_COUNT"].toInt();
}

int Config::getOneActivityServiceCount()
{
    return confMap["ONE_ACTIVITY_SERVICE_COUNT"].toInt();
}

int Config::getServiceMinPrice()
{
    return confMap["SERVICE_MIN_PRICE"].toInt();
}

int Config::getServiceMaxPrice()
{
    return confMap["SERVICE_MAX_PRICE"].toInt();
}

int Config::getServiceMinReliability()
{
    return confMap["SERVICE_MIN_RELIABILITY"].toInt();
}

int Config::getServiceMaxReliability()
{
    return confMap["SERVICE_MAX_RELIABILITY"].toInt();
}

int Config::getServiceMinExectime()
{
    return confMap["SERVICE_MIN_EXECTIME"].toInt();
}

int Config::getServiceMaxExectime()
{
    return confMap["SERVICE_MAX_EXECTIME"].toInt();
}

int Config::getResourceCount()
{
    return confMap["RESOURCE_COUNT"].toInt();
}

int Config::getResourceTypeCount()
{
    return confMap["RESOURCE_TYPE_COUNT"].toInt();
}

int Config::getResourceUseTypeCount()
{
    return confMap["RESOURCE_USE_TYPE_COUNT"].toInt();
}

int Config::getResourceMinPriceCount()
{
    return confMap["RESOURCE_MIN_PRICE"].toInt();
}

int Config::getResourceMaxPriceCount()
{
    return confMap["RESOURCE_Max_PRICE"].toInt();
}

double Config::getWeaken()
{
    return confMap["WEAKEN"].toDouble();
}

int Config::getPuinishmentPerSecond()
{
    return confMap["PUNISHMENT_PER_SECOND"].toInt();
}

int Config::getReduceLayerSize()
{
    return confMap["REDUCE_LAYER_SIZE"].toInt();
}

bool Config::isExtendTree()
{
    return (bool) confMap["IS_EXTEND_TREE"].toInt();
}

int Config::getPuinishmentFailed()
{
    return confMap["PUNISHMENT_FAILED"].toInt();
}

int Config::getPuinishmentStop()
{
    return confMap["PUNISHMENT_STOP"].toInt();
}

int Config::getPuinishmentReadyN()
{
    return confMap["PUNISHMENT_READY_N"].toInt();
}

int Config::getPuinishmentReadyU()
{
    return confMap["PUNISHMENT_READY_U"].toInt();
}

int Config::getRewardFinishN()
{
    return confMap["REWARD_FINISH_N"].toInt();
}

int Config::getRewardFinishU()
{
    return confMap["REWARD_FINISH_U"].toInt();
}

