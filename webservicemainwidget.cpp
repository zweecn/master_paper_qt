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

    setWindowTitle(tr("用户操作界面(Web Service)"));
    resize(1024, 800);
    showMaximized();

    connect(autoStartButton, SIGNAL(clicked()), this, SLOT(autoRun()));
//    connect(startButton, SIGNAL(clicked()), this, SLOT(manualRun()));
//    connect(nextStepButton, SIGNAL(clicked()), this, SLOT(nextStep()));
}

void WebServiceMainWidget::createFlowGroupBox()
{
    flowGroupBox = new QGroupBox(tr("服务流程(Service Workflow)"));
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
    flowInfoGroupBox = new QGroupBox(tr("服务流程信息(Web Service Flow Infomation)"));
    flowInfoWidget = new WebServiceFlowInfoWidget();
    QGridLayout *flowInfoLayout = new QGridLayout;
    flowInfoLayout->addWidget(flowInfoWidget);
    flowInfoGroupBox->setLayout(flowInfoLayout);
}

void WebServiceMainWidget::createEventGroupBox()
{
    eventGroupBox = new QGroupBox(tr("事件(Event)"));
    eventWidget = new WebServiceEventWidget();

    QHBoxLayout *eventLayout = new QHBoxLayout;
    eventLayout->addWidget(eventWidget);
    eventGroupBox->setLayout(eventLayout);
    //    eventGroupBox->setFixedHeight(120);
}

void WebServiceMainWidget::createActionGroupBox()
{
    actionGroupBox = new QGroupBox(tr("动作(Actions)"));
    actionWidget = new WebServiceActionWidget();

    QHBoxLayout *actionLayout = new QHBoxLayout;
    actionLayout->addWidget(actionWidget);
    actionGroupBox->setLayout(actionLayout);
}

void WebServiceMainWidget::createButtonGroupBox()
{
    buttonGroupBox = new QGroupBox(tr("操作(Operations)"));
    autoStartButton = new QPushButton(tr("自动仿真"));
    startButton = new QPushButton(tr("开始"));
    nextStepButton = new QPushButton(tr("下一步"));

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
