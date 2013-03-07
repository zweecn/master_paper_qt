#include "webservicemainwidget.h"
#include "servicegraph.h"
#include "webserviceeventwidget.h"
#include "webserviceflowinfowidget.h"
#include "allmutex.h"

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
    connect(startButton, SIGNAL(clicked()), this, SLOT(manualRun()));
    connect(nextStepButton, SIGNAL(clicked()), this, SLOT(nextStep()));
    connect(wss, SIGNAL(normalEventSignal()), this, SLOT(disableNextStepButton()));
    connect(wss, SIGNAL(badEventSignal()), this, SLOT(enableNextStepButton()));
    connect(wss, SIGNAL(execFinishedSignal()), this, SLOT(disableNextStepButton()));
    connect(wss, SIGNAL(normalEventSignal()), this, SLOT(updateStateToExec()));
    connect(wss, SIGNAL(badEventSignal()), this, SLOT(updateStateToFault()));
    connect(wss, SIGNAL(execFinishedSignal()), this, SLOT(upadteStateToFinished()));
    connect(wss, SIGNAL(stopSignal()), this, SLOT(stop()));
}

WebServiceMainWidget::~WebServiceMainWidget()
{
    delete wss;
}

void WebServiceMainWidget::closeEvent(QCloseEvent *)
{
    wss->setSleepMSecond(0);
    wss->stop();
    nextStepCond.wakeAll();
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
    sleepEdit = new QLineEdit();
    sleepLabel = new QLabel(tr("设定间隔时间(ms)"));
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

void WebServiceMainWidget::init()
{
    wss = new WebServiceSimulation();
}

void WebServiceMainWidget::autoRun()
{
    autoStartButton->setEnabled(false);
    startButton->setEnabled(false);
    nextStepButton->setEnabled(false);

    int sleepMsecond = sleepEdit->text().toInt() * 1000;
    wss->setSleepMSecond(sleepMsecond);
    sleepEdit->setEnabled(false);
    wss->setAutoRun(true);
    wss->setServiceGraph(sg);
    wss->setWebServiceEventWidget(eventWidget);
    wss->setWebServiceActionWidget(actionWidget);
    wss->setWebServiceFlowInfoWidget(flowInfoWidget);
    wss->start();
}

void WebServiceMainWidget::manualRun()
{
    autoStartButton->setEnabled(false);
    startButton->setEnabled(false);
    nextStepButton->setEnabled(false);

    int sleepMsecond = sleepEdit->text().toInt() * 1000;
    wss->setSleepMSecond(sleepMsecond);
    sleepEdit->setEnabled(false);
    wss->setAutoRun(false);
    wss->setServiceGraph(sg);
    wss->setWebServiceEventWidget(eventWidget);
    wss->setWebServiceActionWidget(actionWidget);
    wss->setWebServiceFlowInfoWidget(flowInfoWidget);
    wss->start();
}

void WebServiceMainWidget::nextStep()
{
    int res = actionWidget->getCurrentRow();
    qDebug() << "Select Row: " << res ;
    if (res == -1)
    {
        QMessageBox::critical(this, tr("错误"), tr("请选中一个[有效]的动作! "), QMessageBox::Ok);
    }
    else
    {
        wss->setSelectActionId(res);
        QString q = wss->getSelectItem()->toString();
        int ret = QMessageBox::question(this, tr("选择确认"), tr("您选择的是动作：%1").arg(q),
                              QMessageBox::Ok, QMessageBox::Cancel);
        switch (ret)
        {
        case QMessageBox::Ok:
            nextStepCond.wakeOne();
            break;
        case QMessageBox::Cancel:
            break;
        }
    }

}

void WebServiceMainWidget::enableNextStepButton()
{
    nextStepButton->setEnabled(true);
}

void WebServiceMainWidget::disableNextStepButton()
{
    nextStepButton->setEnabled(false);
}

void WebServiceMainWidget::upadteStateToFinished()
{
    sg->setExecState(QString("执行结束"));
}

void WebServiceMainWidget::updateStateToFault()
{
    sg->setExecState(QString("发生不确定性事件..."));
}

void WebServiceMainWidget::updateStateToExec()
{
    sg->setExecState(QString("执行中..."));
}

void WebServiceMainWidget::stop()
{
    emit stopSignal();
}
