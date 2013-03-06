#ifndef WEBSERVICEFLOWINFOWIDGET_H
#define WEBSERVICEFLOWINFOWIDGET_H

#include "webserviceflow.h"

#include <QWidget>
#include <QMutex>

class QTableWidget;
class QLabel;

class WebServiceFlowInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WebServiceFlowInfoWidget(QWidget *parent = 0);

    void setWebServiceFlow(WebServiceFlow * _wsf);

signals:
    void updateFlowInfoTableSignal();

public slots:
    void updateFlowInfo();

private:
    void createFlowInfoTable();

    QTableWidget* flowInfoTable;
    QLabel* flowInfoLabel;

    WebServiceFlow *wsf;

};

#endif // WEBSERVICEFLOWINFOWIDGET_H
