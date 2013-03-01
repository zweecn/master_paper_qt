#include "businessmainwidget.h"
#include "drawgraph.h"
#include "config.h"

#include <QMessageBox>

BusinessMainWidget::BusinessMainWidget(QWidget *parent) :
    QWidget(parent)
{
    init();

    createFlowGroupBox();
    createEventGroupBox();
    createActionGroupBox();
    createButtonGroupBox();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(flowGroupBox);
    mainLayout->addWidget(eventGroupBox);
    mainLayout->addWidget(actionGroupBox);
    mainLayout->addWidget(buttonGroupBox);
    setLayout(mainLayout);

    setWindowTitle(tr("�û���������"));
    resize(800, 700);

    connect(autoStartButton, SIGNAL(clicked()), this, SLOT(autoRun()));
    connect(startButton, SIGNAL(clicked()), this, SLOT(manualRun()));
    connect(nextStepButton, SIGNAL(clicked()), this, SLOT(nextStep()));
    connect(bs, SIGNAL(normalEventSignal()), this, SLOT(disableNextStepButton()));
    connect(bs, SIGNAL(badEventSignal()), this, SLOT(enableNextStepButton()));

}

void BusinessMainWidget::createFlowGroupBox()
{
    flowGroupBox = new QGroupBox(tr("��������(Service Workflow)"));
    sg = new ServiceGraph[workflowCount];
    for (int i = 0; i < workflowCount; i++)
    {
        sg[i].setFixedHeight(sg[i].getRecommedHeight());
        sg[i].setFlowId(i);
    }
    QGridLayout *flowLayout = new QGridLayout;
    for (int i = 0; i < workflowCount; i++)
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
    workflowCount = Config::Instance()->getWorkflowCount();
}

void BusinessMainWidget::autoRun()
{
    autoStartButton->setEnabled(false);
    startButton->setEnabled(false);
    nextStepButton->setEnabled(false);

    bs->setAutoRun(true);
    bs->setServiceGraph(sg);
    bs->setBusinessEventWidget(eventWidget);
    bs->setBusinessActionWidget(actionWidget);
    bs->start();
}

void BusinessMainWidget::manualRun()
{
    autoStartButton->setEnabled(false);
    startButton->setEnabled(false);
    nextStepButton->setEnabled(false);

    bs->setAutoRun(false);
    bs->setServiceGraph(sg);
    bs->setBusinessEventWidget(eventWidget);
    bs->setBusinessActionWidget(actionWidget);
    bs->start();
}

void BusinessMainWidget::nextStep()
{
    int res = actionWidget->getCurrentRow();
    qDebug() << "Select Row: " << res ;
    if (res == -1)
    {
        QMessageBox::critical(this, tr("����"), tr("��ѡ��һ��[��Ч]�Ķ���! "), QMessageBox::Ok);
    }
    else
    {
        bs->setSelectActionId(res);
        nextStepCond.wakeOne();
    }
}

void BusinessMainWidget::enableNextStepButton()
{
    nextStepButton->setEnabled(true);
}

void BusinessMainWidget::disableNextStepButton()
{
    nextStepButton->setEnabled(false);
}
