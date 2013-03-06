#include "webservicemainwidget.h"
#include "servicegraph.h"
#include "webserviceeventwidget.h"
#include <QtGui>

WebServiceMainWidget::WebServiceMainWidget(QWidget *parent) :
    QWidget(parent)
{
    createFlowGroupBox();
    createEventGroupBox();
    createActionGroupBox();

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(flowGroupBox, 0, 0);
    mainLayout->addWidget(eventGroupBox, 1, 0);
    mainLayout->addWidget(actionGroupBox, 1, 1);
    setLayout(mainLayout);

    setWindowTitle(tr("�û���������(Web Service)"));
    resize(1024, 800);
    showMaximized();
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
