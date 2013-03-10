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
class QLineEdit;

class WebServiceMainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WebServiceMainWidget(QWidget *parent = 0);
    ~WebServiceMainWidget();

protected:
    void closeEvent(QCloseEvent *);

signals:
    void stopSignal();
    void deleteSignal();

public slots:
    void autoRun();

    void manualRun();
    void nextStep();

    void enableNextStepButton();
    void disableNextStepButton();

    void upadteStateToFinished();
    void updateStateToFault();
    void updateStateToExec();

    void stop();

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
    QLineEdit *sleepEdit;
    QLabel *sleepLabel;

    void init();
    ServiceGraph *sg;
    WebServiceSimulation *wss;
};

#endif // WEBSERVICEMAINWIDGET_H
