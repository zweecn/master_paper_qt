#include <QtGui/QApplication>
#include <QTextCodec>
#include <QDebug>
#include <QFile>
#include "widget.h"
#include "linechart.h"
#include "servicegraph.h"
#include "serviceinfotable.h"
#include "atomservice.h"
#include "resource.h"
#include "businesssimulation.h"
#include "criticalpath.h"
#include "workflow.h"
#include "intervalcoverage.h"
#include "businessmainwidget.h"
#include "webservicesimulation.h"
#include "webserviceflow.h"
#include "test.h"
#include "webservicemainwidget.h"
#include "simulationmainwidget.h"

void customMessageHandler(QtMsgType type, const char* msg)
{
    QString txt;
    switch(type)
    {
    case QtDebugMsg:
        txt = QString("Debug: %1").arg(msg);
        break;
    case QtWarningMsg:
        txt = QString("Warning: %1").arg(msg);
        break;
    case QtCriticalMsg:
        txt = QString("Critical: %1").arg(msg);
        break;
    case QtFatalMsg:
        txt = QString("Critical: %1").arg(msg);
        abort();
    }
    QFile outFile("sim.log");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
}

int main(int argc, char *argv[])
{
    setlocale(LC_CTYPE, "zh_CN.GB2312");
    QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GBK"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));

    QApplication a(argc, argv);

    qInstallMsgHandler(customMessageHandler);

    SimulationMainWidget w;
    w.show();

    return a.exec();
}
