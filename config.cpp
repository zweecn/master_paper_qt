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

int Config::getResourceTypeCount()
{
    return confMap["RESOURCE_TYPE_COUNT"].toInt();
}

int Config::getUCTypeCount()
{
    return confMap["UC_TYPE_COUNT"].toInt();
}

int Config::getWorkflowCount()
{
    return confMap["WORKFLOW_COUNT"].toInt();
}
