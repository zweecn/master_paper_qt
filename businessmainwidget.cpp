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
    createServiceGroupBox();
    createButtonGroupBox();
    createRewardGroupBox();
    createStateGroupBox();

//    QVBoxLayout *mainLayout = new QVBoxLayout;
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(flowGroupBox, 0, 0, 1, 2);
//    mainLayout->addWidget(serviceInfoGroupBox, 1, 0);
    mainLayout->addWidget(stateGroupBox, 1, 0, 1, 2);
    mainLayout->addWidget(eventGroupBox, 2, 0);
    mainLayout->addWidget(actionGroupBox, 2, 1);
    mainLayout->addWidget(buttonGroupBox, 3, 0, 1, 2);
//    mainLayout->addWidget(rewardGroupBox, 3, 0, 1, 2);

    setLayout(mainLayout);

    setWindowTitle(tr("�û���������"));
    resize(1024, 800);
    showMaximized();

    connect(autoStartButton, SIGNAL(clicked()), this, SLOT(autoRun()));
    connect(startButton, SIGNAL(clicked()), this, SLOT(manualRun()));
    connect(nextStepButton, SIGNAL(clicked()), this, SLOT(nextStep()));
    connect(bs, SIGNAL(normalEventSignal()), this, SLOT(disableNextStepButton()));
    connect(bs, SIGNAL(badEventSignal()), this, SLOT(enableNextStepButton()));
    connect(bs, SIGNAL(normalEventSignal()), this, SLOT(updateStateToExec()));
    connect(bs, SIGNAL(badEventSignal()), this, SLOT(updateStateToFault()));
    connect(bs, SIGNAL(execFinishedSignal()), this, SLOT(upadteStateToFinished()));
    connect(bs, SIGNAL(badEventSignal(int)), this, SLOT(updateStateToFault(int)));
}

BusinessMainWidget::~BusinessMainWidget()
{
    delete bs;
}

void BusinessMainWidget::closeEvent(QCloseEvent *)
{
    bs->setSleepMSecond(0);
    bs->stop();
    nextStepCond.wakeAll();
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
        flowLayout->addWidget(&sg[i], i / 4, i % 4);
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
//    eventGroupBox->setFixedHeight(120);
}

void BusinessMainWidget::createActionGroupBox()
{
    actionGroupBox = new QGroupBox(tr("����(Actions)"));
    actionWidget = new BusinessActionWidget();

    QHBoxLayout *actionLayout = new QHBoxLayout;
    actionLayout->addWidget(actionWidget);
    actionGroupBox->setLayout(actionLayout);
}

void BusinessMainWidget::createServiceGroupBox()
{
    serviceInfoGroupBox = new QGroupBox(tr("��ѡ������Ϣ"));
    serviceInfoTable = new ServiceInfoTable();

    QHBoxLayout *serviceLayout = new QHBoxLayout;
    serviceLayout->addWidget(serviceInfoTable);
    serviceInfoGroupBox->setLayout(serviceLayout);
}

void BusinessMainWidget::createButtonGroupBox()
{
    buttonGroupBox = new QGroupBox(tr("����(Operations)"));
    autoStartButton = new QPushButton(tr("�Զ�����"));
    startButton = new QPushButton(tr("��ʼ"));
    nextStepButton = new QPushButton(tr("��һ��"));
    sleepEdit = new QLineEdit();
    sleepLabel = new QLabel(tr("�趨���ʱ��(ms)"));
    sleepEdit->setFixedWidth(60);
    sleepEdit->setText(tr("1000"));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(sleepLabel);
    buttonLayout->addWidget(sleepEdit);
    buttonLayout->addWidget(autoStartButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(nextStepButton);
    buttonGroupBox->setLayout(buttonLayout);
}

void BusinessMainWidget::createRewardGroupBox()
{
    rewardGroupBox = new QGroupBox(tr("����(Reward)"));
    rewardLineChart = new LineChart();

    QHBoxLayout *rewardLayout = new QHBoxLayout;
    rewardLayout->addWidget(rewardLineChart);
    rewardGroupBox->setLayout(rewardLayout);
    rewardGroupBox->setFixedHeight(300);
}

void BusinessMainWidget::createStateGroupBox()
{
    stateGroupBox = new QGroupBox(tr("��������״̬(State)"));
    stateWidget = new BusinessStateWidget();
    QVBoxLayout *stateLayout = new QVBoxLayout;
    stateLayout->addWidget(stateWidget);
    stateGroupBox->setLayout(stateLayout);
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

    int sleepMsecond = sleepEdit->text().toInt() * 1000;
    bs->setSleepMSecond(sleepMsecond);
    sleepEdit->setEnabled(false);
    bs->setAutoRun(true);
    bs->setServiceGraph(sg);
    bs->setBusinessEventWidget(eventWidget);
    bs->setBusinessActionWidget(actionWidget);
    bs->setBusinessStateWidget(stateWidget);
    bs->start();
}

void BusinessMainWidget::manualRun()
{
    autoStartButton->setEnabled(false);
    startButton->setEnabled(false);
    nextStepButton->setEnabled(false);

    int sleepMsecond = sleepEdit->text().toInt() * 1000;
    bs->setSleepMSecond(sleepMsecond);
    sleepEdit->setEnabled(false);
    bs->setAutoRun(false);
    bs->setServiceGraph(sg);
    bs->setBusinessEventWidget(eventWidget);
    bs->setBusinessActionWidget(actionWidget);
    bs->setBusinessStateWidget(stateWidget);
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

void BusinessMainWidget::upadteStateToFinished()
{
    for (int i = 0; i < workflowCount; i++)
        sg[i].setExecState(QString("����"));
}

void BusinessMainWidget::updateStateToFault()
{
    for (int i = 0; i < workflowCount; i++)
    sg[i].setExecState(QString("��ȷ���¼�.."));
}

void BusinessMainWidget::updateStateToFault(int _flowId)
{
    sg[_flowId].setExecState(QString("��ȷ���¼�.."));
}

void BusinessMainWidget::updateStateToExec()
{
    for (int i = 0; i < workflowCount; i++)
    sg[i].setExecState(QString("ִ����..."));
}

void BusinessMainWidget::stop()
{
    emit stopSignal();
}
