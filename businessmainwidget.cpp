#include "businessmainwidget.h"
#include "drawgraph.h"

BusinessMainWidget::BusinessMainWidget(QWidget *parent) :
    QWidget(parent)
{
    init();

    createFlowGroupBox();
    createEventGroupBox();
    createActionGroupBox();
    createButtonGroupBox();

    connect(autoStartButton, SIGNAL(clicked()), this, SLOT(autoRun()));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(flowGroupBox);
    mainLayout->addWidget(eventGroupBox);
    mainLayout->addWidget(actionGroupBox);
    mainLayout->addWidget(buttonGroupBox);
    setLayout(mainLayout);

    setWindowTitle(tr("�û���������"));
    resize(800, 700);
}

void BusinessMainWidget::createFlowGroupBox()
{
    flowGroupBox = new QGroupBox(tr("��������(Service Workflow)"));
    sg = new ServiceGraph[bs->getWorkflowCount()];
    for (int i = 0; i < bs->getWorkflowCount(); i++)
    {
        sg[i].setFixedHeight(sg[i].getRecommedHeight());
        sg[i].setFlowId(i);
    }
    QGridLayout *flowLayout = new QGridLayout;
    for (int i = 0; i < bs->getWorkflowCount(); i++)
    {
        flowLayout->addWidget(&sg[i], 0, i);
    }
    flowGroupBox->setLayout(flowLayout);
}

void BusinessMainWidget::createEventGroupBox()
{
    eventGroupBox = new QGroupBox(tr("�¼�(Event)"));
    eventWidget = new BusinessEventWidget();

    QHBoxLayout *eventLayout = new QHBoxLayout;
    eventLayout->addWidget(eventWidget);
    eventGroupBox->setLayout(eventLayout);
    eventGroupBox->setFixedHeight(110);
}

void BusinessMainWidget::createActionGroupBox()
{
    actionGroupBox = new QGroupBox(tr("����(Actions)"));
    actionWidget = new BusinessActionWidget();

    QHBoxLayout *actionLayout = new QHBoxLayout;
    actionLayout->addWidget(actionWidget);
    actionGroupBox->setLayout(actionLayout);
}

void BusinessMainWidget::createButtonGroupBox()
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


void BusinessMainWidget::init()
{
    bs = new BusinessSimulation();
}

void BusinessMainWidget::autoRun()
{
    autoStartButton->setEnabled(false);
    bs->setServiceGraph(sg);
    bs->setBusinessEventWidget(eventWidget);
    bs->setBusinessActionWidget(actionWidget);
    bs->run();
    autoStartButton->setEnabled(true);
}
