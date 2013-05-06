#ifndef SIMULATIONMAINWIDGET_H
#define SIMULATIONMAINWIDGET_H
#include "webservicemainwidget.h"
#include "businessmainwidget.h"
#include "webservicesimulation.h"

#include <QWidget>

class SimulationMainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SimulationMainWidget(QWidget *parent = 0);
    ~SimulationMainWidget();
signals:

public slots:
    void doWebServiceMatlab();
    void doWebServiceSimulation();
    void doBusinessSimulation();
    void deleteWebServiceMatlab();
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
};

#endif // SIMULATIONMAINWIDGET_H
