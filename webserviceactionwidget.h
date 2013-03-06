#ifndef WEBSERVICEACTIONWIDGET_H
#define WEBSERVICEACTIONWIDGET_H
#include "webserviceaction.h"
#include "webservicerecovery.h"

#include <QWidget>
#include <QMutex>

class QTableWidget;
class QLabel;
class WebServiceAction;

class WebServiceActionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WebServiceActionWidget(QWidget *parent = 0);

    void setMarkovResult(QList<MarkovResultItem>& _markovResult);

    void setWebServiceAction(WebServiceAction* _actions);
    void setAutoWebServiceAction(WebServiceAction* _autoAction);
    WebServiceAction* getWebServiceAction();
    WebServiceAction* getAutoWebServiceAction();

    int getCurrentRow();

signals:
    void updateActionsSignal();

public slots:
    void updateActionTable();

private:
    void createActionTable();

    QTableWidget* actionTable;
    QLabel* markovActionLabel;
    QLabel* greedyActionLabel;

    WebServiceAction *actions;
    WebServiceAction *autoAction;

    QList<MarkovResultItem> markovResult;
};

#endif // WEBSERVICEACTIONWIDGET_H
