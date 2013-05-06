#include "simulationmainwidget.h"

SimulationMainWidget::SimulationMainWidget(QWidget *parent) :
    QWidget(parent)
{
    init();

    createOpGroupBox();
    createWSTestGroupBox();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(opGroupBox);
    mainLayout->addWidget(wsTestGroupBox);
    setLayout(mainLayout);

    setWindowTitle(tr("主控制面板"));
    resize(400, 200);

    connect(webserviceButton, SIGNAL(clicked()), this, SLOT(doWebServiceSimulation()));
    connect(businessButton, SIGNAL(clicked()), this, SLOT(doBusinessSimulation()));

//    connect(wsTest1Button, SIGNAL(clicked()), this, SLOT(doWebServiceMarkov()));
    connect(wsTest1Button, SIGNAL(clicked()), this, SLOT(test1()));
    connect(wsTest2Button, SIGNAL(clicked()), this, SLOT(test2()));
    connect(wsTest3Button, SIGNAL(clicked()), this, SLOT(test3()));
}

SimulationMainWidget::~SimulationMainWidget()
{
    engClose(ep);
    ep = NULL;
}


void SimulationMainWidget::createOpGroupBox()
{
    messageLabel = new QLabel(tr("<font size='+2'><p align='center'>请选择需要仿真的服务不确定性:</p></font>"));
    webserviceButton = new QPushButton(tr("软件层面"));
    businessButton = new QPushButton(tr("业务层面"));
    opGroupBox = new QGroupBox(tr("流程仿真操作(软件层面/业务层面)"));
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addStretch();
    layout->addWidget(webserviceButton);
    layout->addStretch();
    layout->addWidget(businessButton);
    layout->addStretch();
    opGroupBox->setLayout(layout);

//    webserviceButton->resize(100, 40);
//    businessButton->resize(100, 40);
//    QGridLayout *layout = new QGridLayout;
//    layout->addWidget(messageLabel, 1, 0, 1, 5);
//    layout->addWidget(webserviceButton, 3, 1);
//    layout->addWidget(businessButton, 3, 3);
//    layout->setColumnStretch(0, 1);
//    layout->setColumnStretch(2, 1);
//    layout->setColumnStretch(4, 1);
//    layout->setRowStretch(0, 1);
//    layout->setRowStretch(2, 1);
//    layout->setRowStretch(4, 1);
//    opGroupBox->setLayout(layout);

}

void SimulationMainWidget::createWSTestGroupBox()
{
    wsTest1Button = new QPushButton(tr("实验1"));
    wsTest2Button = new QPushButton(tr("实验2"));
    wsTest3Button = new QPushButton(tr("实验3"));
    wsTestGroupBox = new QGroupBox(tr("软件层Matlab实验"));
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addStretch();
    layout->addWidget(wsTest1Button);
    layout->addStretch();
    layout->addWidget(wsTest2Button);
    layout->addStretch();
    layout->addWidget(wsTest3Button);
    layout->addStretch();
    wsTestGroupBox->setLayout(layout);

}

void SimulationMainWidget::init()
{
    wsw = NULL;
    bw = NULL;
    wss = NULL;

    ep = engOpen(NULL);
    if (!ep)
    {   // 定义Matlab引擎指针，启动引擎；失败则返回NULL
        qDebug() << "Can't start Matlab engine!";
        exit(-1);
    }
    engSetVisible(ep, false);
}

void SimulationMainWidget::doWebServiceMarkov()
{
    wsTest1Button->setEnabled(false);
    if (wss != NULL)
        delete wss;
    wss = new WebServiceSimulation();
    wss->setRunType(WebServiceSimulation::RUNTYPE_MATLAB_MARKOV);
    wss->start();
    connect(wss, SIGNAL(finished()), this, SLOT(doWebServiceGreedy()));
}

void SimulationMainWidget::doWebServiceGreedy()
{
    if (wss != NULL)
    {
        markovCmd = wss->getMatlabCmd();
        delete wss;
        wss = NULL;
    }
    wss = new WebServiceSimulation();
    wss->setRunType(WebServiceSimulation::RUNTYPE_MATLAB_GREEDY);
    wss->start();
    connect(wss, SIGNAL(finished()), this, SLOT(deleteWebServiceSim()));
}

void SimulationMainWidget::deleteWebServiceSim()
{
    if (wss != NULL)
    {
        greedCmd = wss->getMatlabCmd();
        delete wss;
        wss = NULL;
    }
    plotTest1();
    wsTest1Button->setEnabled(true);
}

void SimulationMainWidget::plotTest1()
{
    matlabCmd = QString("%1 %2")
            .arg(markovCmd)
            .arg(greedCmd);
    matlabCmd.append("cd E:\\Dev\\MATLAB7\\work\\webservice_uc; test1( t1, r1, c1, dt1, t2, r2, c2, dt2)");
    qDebug() << "Exec matlab cmd:" << matlabCmd;
    engEvalString(ep, matlabCmd.toStdString().c_str());
}

void SimulationMainWidget::test1()
{
    matlabCmd.clear();
    matlabCmd.append("cd E:\\Dev\\MATLAB7\\work\\test1_good\\v02; test1_func();");
    qDebug() << "Exec matlab cmd:" << matlabCmd;
    engEvalString(ep, matlabCmd.toStdString().c_str());
}

void SimulationMainWidget::test2()
{
    matlabCmd.clear();
    matlabCmd.append("cd E:\\Dev\\MATLAB7\\work\\test3_good\\v01; test3_func();");
    qDebug() << "Exec matlab cmd:" << matlabCmd;
    engEvalString(ep, matlabCmd.toStdString().c_str());
}

void SimulationMainWidget::test3()
{
    matlabCmd.clear();
    matlabCmd.append("cd E:\\Dev\\MATLAB7\\work\\test4_good\\v01; test4_func();");
    qDebug() << "Exec matlab cmd:" << matlabCmd;
    engEvalString(ep, matlabCmd.toStdString().c_str());
}

void SimulationMainWidget::doWebServiceSimulation()
{
    webserviceButton->setEnabled(false);
    businessButton->setEnabled(false);
    wsw = new WebServiceMainWidget();
//    connect(wsw, SIGNAL(stopSignal()), this, SLOT(deleteWebServiceMainWidget()));
    connect(wsw, SIGNAL(deleteSignal()), this, SLOT(deleteWebServiceMainWidget()));
}

void SimulationMainWidget::doBusinessSimulation()
{
    webserviceButton->setEnabled(false);
    businessButton->setEnabled(false);
    bw = new BusinessMainWidget();
//    connect(bw, SIGNAL(stopSignal()), this, SLOT(deleteBusinessMainWidget()));
    connect(bw, SIGNAL(deleteSignal()), this, SLOT(deleteBusinessMainWidget()));
}

void SimulationMainWidget::deleteWebServiceMainWidget()
{
    BusinessSimulation::sleepAMoment();
    if (wsw != NULL)
        delete wsw;
    wsw = NULL;
    webserviceButton->setEnabled(true);
    businessButton->setEnabled(true);
}

void SimulationMainWidget::deleteBusinessMainWidget()
{
    BusinessSimulation::sleepAMoment();
    if (bw != bw)
        delete bw;
    bw = NULL;
    webserviceButton->setEnabled(true);
    businessButton->setEnabled(true);
}
