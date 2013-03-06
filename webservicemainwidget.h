#ifndef WEBSERVICEMAINWIDGET_H
#define WEBSERVICEMAINWIDGET_H
#include "servicegraph.h"
#include "webserviceeventwidget.h"
#include "webserviceaction.h"
#include "webserviceactionwidget.h"
#include <QWidget>

class QPushButton;
class QGroupBox;

class WebServiceMainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WebServiceMainWidget(QWidget *parent = 0);

signals:

public slots:

private:
    void createFlowGroupBox();
    void createEventGroupBox();
    void createActionGroupBox();

    QGroupBox *flowGroupBox;

    QGroupBox *eventGroupBox;
    WebServiceEventWidget *eventWidget;

    QGroupBox *actionGroupBox;
    WebServiceActionWidget *actionWidget;

    ServiceGraph *sg;
};

#endif // WEBSERVICEMAINWIDGET_H
