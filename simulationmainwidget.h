#ifndef SIMULATIONMAINWIDGET_H
#define SIMULATIONMAINWIDGET_H
#include "webservicemainwidget.h"
#include "businessmainwidget.h"
#include "webservicesimulation.h"

#include <QWidget>
#include <engine.h>

class SimulationMainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SimulationMainWidget(QWidget *parent = 0);
    ~SimulationMainWidget();
signals:

public slots:
    // Not used
    void doWebServiceMarkov();
    void doWebServiceGreedy();
    void deleteWebServiceSim();
    void plotTest1();

    // Test1 - Test3
    void test1();
    void test2();
    void test3();

    void doWebServiceSimulation();
    void doBusinessSimulation();

    void deleteWebServiceMainWidget();
    void deleteBusinessMainWidget();

private:
    void init();
    void createOpGroupBox();
    void createWSTestGroupBox();

    QPushButton *webserviceButton;
    QPushButton *businessButton;
    QGroupBox* opGroupBox;

    QPushButton *wsTest1Button;
    QPushButton *wsTest2Button;
    QPushButton *wsTest3Button;
    QGroupBox *wsTestGroupBox;

    QLabel *messageLabel;

    WebServiceMainWidget* wsw;
    BusinessMainWidget* bw;

    WebServiceSimulation* wss;
    QString matlabCmd;
    QString markovCmd;
    QString greedCmd;
    Engine *ep;
};

#endif // SIMULATIONMAINWIDGET_H
