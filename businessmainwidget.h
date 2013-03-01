#ifndef BUSINESSMAINWIDGET_H
#define BUSINESSMAINWIDGET_H

#include <QWidget>
#include <QtGui>

#include "servicegraph.h"
#include "businesssimulation.h"
#include "businesseventwidget.h"
#include "businessactionwidget.h"
#include "serviceinfotable.h"
#include "linechart.h"


class BusinessMainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BusinessMainWidget(QWidget *parent = 0);

signals:

public slots:
    void autoRun();

    void manualRun();
    void nextStep();

    void enableNextStepButton();
    void disableNextStepButton();

private:

    // UI
    void createFlowGroupBox();
    void createEventGroupBox();
    void createActionGroupBox();
    void createServiceGroupBox();
    void createButtonGroupBox();
    void createRewardGroupBox();
    void createStateGroupBox();

    QGroupBox *flowGroupBox;

    QGroupBox *eventGroupBox;
    BusinessEventWidget *eventWidget;

    QGroupBox *actionGroupBox;
    BusinessActionWidget *actionWidget;

    QGroupBox *serviceInfoGroupBox;
    ServiceInfoTable* serviceInfoTable;

    QGroupBox *buttonGroupBox;
    QPushButton *autoStartButton;
    QPushButton *startButton;
    QPushButton *nextStepButton;

    QGroupBox *rewardGroupBox;
    LineChart *rewardLineChart;

    QGroupBox *stateGroupBox;


    // ALG
    void init();
    ServiceGraph *sg;
    BusinessSimulation *bs;
    int workflowCount;
};

#endif // BUSINESSMAINWIDGET_H
