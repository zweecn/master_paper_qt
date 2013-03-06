#include "webservicemainwidget.h"
#include "servicegraph.h"
#include "webserviceeventwidget.h"
#include "webserviceflowinfowidget.h"
#include <QtGui>

WebServiceMainWidget::WebServiceMainWidget(QWidget *parent) :
    QWidget(parent)
{
    init();

    createFlowGroupBox();
    createFlowInfoGroupBox();
    createEventGroupBox();
    createActionGroupBox();
    createButtonGroupBox();

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(flowGroupBox, 0, 0);
    mainLayout->addWidget(flowInfoGroupBox, 0, 1);
    mainLayout->addWidget(eventGroupBox, 1, 0);
    mainLayout->addWidget(actionGroupBox, 1, 1);
    mainLayout->addWidget(buttonGroupBox, 2, 0, 1, 2);
    setLayout(mainLayout);

    setWindowTitle(tr("�û���������(Web Service)"));
    resize(1024, 800);
    showMaximized();

    connect(autoStartButton, SIGNAL(clicked()), this, SLOT(autoRun()));
//    connect(startButton, SIGNAL(clicked()), this, SLOT(manualRun()));
//    connect(nextStepButton, SIGNAL(clicked()), this, SLOT(nextStep()));
}

void WebServiceMainWidget::createFlowGroupBox()
{
    flowGroupBox = new QGroupBox(tr("��������(Service Workflow)"));
    sg = new ServiceGraph();
    sg->setFlowId(0);
    sg->zoomOut(1.6);
    sg->setFixedHeight(sg->getRecommedHeight());
    QGridLayout *flowLayout = new QGridLayout;
    flowLayout->addWidget(sg);
    flowGroupBox->setLayout(flowLayout);
}

void WebServiceMainWidget::createFlowInfoGroupBox()
{
    flowInfoGroupBox = new QGroupBox(tr("����������Ϣ(Web Service Flow Infomation)"));
    flowInfoWidget = new WebServiceFlowInfoWidget();
    QGridLayout *flowInfoLayout = new QGridLayout;
    flowInfoLayout->addWidget(flowInfoWidget);
    flowInfoGroupBox->setLayout(flowInfoLayout);
}

void WebServiceMainWidget::createEventGroupBox()
{
    eventGroupBox = new QGroupBox(tr("�¼�(Event)"));
    eventWidget = new WebServiceEventWidget();

    QHBoxLayout *eventLayout = new QHBoxLayout;
    eventLayout->addWidget(eventWidget);
    eventGroupBox->setLayout(eventLayout);
    //    eventGroupBox->setFixedHeight(120);
}

void WebServiceMainWidget::createActionGroupBox()
{
    actionGroupBox = new QGroupBox(tr("����(Actions)"));
    actionWidget = new WebServiceActionWidget();

    QHBoxLayout *actionLayout = new QHBoxLayout;
    actionLayout->addWidget(actionWidget);
    actionGroupBox->setLayout(actionLayout);
}

void WebServiceMainWidget::createButtonGroupBox()
{
    buttonGroupBox = new QGroupBox(tr("����(Operations)"));
    autoStartButton = new QPushButton(tr("�Զ�����"));
    startButton = new QPushButton(tr("��ʼ"));
    nextStepButton = new QPushButton(tr("��һ��"));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(autoStartButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(nextStepButton);
    buttonGroupBox->setLayout(buttonLayout);
}

void WebServiceMainWidget::init()
{
    wss = new WebServiceSimulation();
}

void WebServiceMainWidget::autoRun()
{
    autoStartButton->setEnabled(false);
    startButton->setEnabled(false);
    nextStepButton->setEnabled(false);

//    wss->setAutoRun(true);
    wss->setServiceGraph(sg);
//    wss->setBusinessEventWidget(eventWidget);
//    wss->setBusinessActionWidget(actionWidget);
//    wss->setBusinessStateWidget(stateWidget);
    wss->start();
}
