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
    resize(400, 300);

    connect(webserviceButton, SIGNAL(clicked()), this, SLOT(doWebServiceSimulation()));
    connect(businessButton, SIGNAL(clicked()), this, SLOT(doBusinessSimulation()));

    connect(wsTest1Button, SIGNAL(clicked()), this, SLOT(doWebServiceMatlab()));
}

SimulationMainWidget::~SimulationMainWidget()
{

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
}

void SimulationMainWidget::doWebServiceMatlab()
{
    wsTest1Button->setEnabled(false);
    wss = new WebServiceSimulation();
//    wss->setMatlabRun(true);
    wss->start();
    connect(wss, SIGNAL(finished()), this, SLOT(deleteWebServiceMatlab()));
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

void SimulationMainWidget::deleteWebServiceMatlab()
{
    if (wss != NULL)
        delete wss;
    wss = NULL;
    wsTest1Button->setEnabled(true);
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
