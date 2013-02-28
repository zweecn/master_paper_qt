#ifndef BUSINESSMAINWIDGET_H
#define BUSINESSMAINWIDGET_H

#include <QWidget>
#include <QtGui>
#include "servicegraph.h"
#include "businesssimulation.h"

#include "businesseventwidget.h"
#include "businessactionwidget.h"

class BusinessMainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BusinessMainWidget(QWidget *parent = 0);

signals:

public slots:
    void autoRun();
private:

    // UI
    void createFlowGroupBox();
    void createEventGroupBox();
    void createActionGroupBox();
    void createButtonGroupBox();

    QGroupBox *flowGroupBox;

    QGroupBox *eventGroupBox;
    BusinessEventWidget *eventWidget;

    QGroupBox *actionGroupBox;
    BusinessActionWidget *actionWidget;

    QGroupBox *buttonGroupBox;
    QPushButton *autoStartButton;
    QPushButton *startButton;
    QPushButton *nextStepButton;

    // ALG
    void init();
    ServiceGraph *sg;
    BusinessSimulation *bs;

};

#endif // BUSINESSMAINWIDGET_H