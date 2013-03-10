#include "simulationmainwidget.h"

SimulationMainWidget::SimulationMainWidget(QWidget *parent) :
    QWidget(parent)
{
    init();

    createMessages();
    createButtons();

    QGridLayout *mainLayout = new QGridLayout;

    mainLayout->addWidget(messageLabel, 1, 0, 1, 5);
    mainLayout->addWidget(webserviceButton, 3, 1);
    mainLayout->addWidget(businessButton, 3, 3);

    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(2, 1);
    mainLayout->setColumnStretch(4, 1);
    mainLayout->setRowStretch(0, 1);
    mainLayout->setRowStretch(2, 1);
    mainLayout->setRowStretch(4, 1);

    setLayout(mainLayout);

    setWindowTitle(tr("仿真选择"));
    resize(400, 300);

    connect(webserviceButton, SIGNAL(clicked()), this, SLOT(doWebServiceSimulation()));
    connect(businessButton, SIGNAL(clicked()), this, SLOT(doBusinessSimulation()));
}

SimulationMainWidget::~SimulationMainWidget()
{

}

void SimulationMainWidget::createMessages()
{
    messageLabel = new QLabel(tr("<font size='+2'><p align='center'>请选择需要仿真的服务不确定性:</p></font>"));
}

void SimulationMainWidget::createButtons()
{
    webserviceButton = new QPushButton(tr("软件层面"));
    businessButton = new QPushButton(tr("业务层面"));
    webserviceButton->resize(100, 40);
    businessButton->resize(100, 40);
}

void SimulationMainWidget::init()
{
    wsw = NULL;
    bw = NULL;
}

void SimulationMainWidget::doWebServiceSimulation()
{
    wsw = new WebServiceMainWidget();
//    connect(wsw, SIGNAL(stopSignal()), this, SLOT(deleteWebServiceMainWidget()));
    connect(wsw, SIGNAL(deleteSignal()), this, SLOT(deleteWebServiceMainWidget()));
}

void SimulationMainWidget::doBusinessSimulation()
{
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
}

void SimulationMainWidget::deleteBusinessMainWidget()
{
    BusinessSimulation::sleepAMoment();
    if (bw != bw)
        delete bw;
    bw = NULL;
}
