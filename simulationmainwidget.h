#ifndef SIMULATIONMAINWIDGET_H
#define SIMULATIONMAINWIDGET_H
#include "webservicemainwidget.h"
#include "businessmainwidget.h"

#include <QWidget>

class SimulationMainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SimulationMainWidget(QWidget *parent = 0);
    ~SimulationMainWidget();
signals:

public slots:
    void doWebServiceSimulation();
    void doBusinessSimulation();
    void deleteWebServiceMainWidget();
    void deleteBusinessMainWidget();

private:
    void init();
    void createButtons();
    void createMessages();

    QPushButton *webserviceButton;
    QPushButton *businessButton;

    QLabel *messageLabel;

    WebServiceMainWidget* wsw;
    BusinessMainWidget* bw;
};

#endif // SIMULATIONMAINWIDGET_H
