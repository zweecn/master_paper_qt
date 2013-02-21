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

int main(int argc, char *argv[])
{
    setlocale(LC_CTYPE, "zh_CN.GB2312");
    QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GBK"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));

    QApplication a(argc, argv);
    //    Widget w;
    //    w.show();

    //    ServiceGraph sg;
    //    sg.show();

    //    LineChart lc;
    //    lc.show();

    ServiceInfoTable sit;
    sit.show();

    BusinessSimulation bs;
    bs.run();

    CriticalPath cp;
    cp.runExample();

    return a.exec();
}
