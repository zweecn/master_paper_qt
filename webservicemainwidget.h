#ifndef WEBSERVICEMAINWIDGET_H
#define WEBSERVICEMAINWIDGET_H
#include "servicegraph.h"
#include "webserviceeventwidget.h"
#include "webserviceaction.h"
#include "webserviceactionwidget.h"
#include "webservicesimulation.h"

#include <QWidget>

class QPushButton;
class QGroupBox;
class WebServiceFlowInfoWidget;

class WebServiceMainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WebServiceMainWidget(QWidget *parent = 0);

signals:

public slots:
    void autoRun();

private:
    void createFlowGroupBox();
    void createFlowInfoGroupBox();
    void createEventGroupBox();
    void createActionGroupBox();
    void createButtonGroupBox();

    QGroupBox *flowGroupBox;

    QGroupBox *flowInfoGroupBox;
    WebServiceFlowInfoWidget* flowInfoWidget;

    QGroupBox *eventGroupBox;
    WebServiceEventWidget *eventWidget;

    QGroupBox *actionGroupBox;
    WebServiceActionWidget *actionWidget;

    QGroupBox *buttonGroupBox;
    QPushButton *autoStartButton;
    QPushButton *startButton;
    QPushButton *nextStepButton;

    void init();
    ServiceGraph *sg;
    WebServiceSimulation *wss;
};

#endif // WEBSERVICEMAINWIDGET_H
