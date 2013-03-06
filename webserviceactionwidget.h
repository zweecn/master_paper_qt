#ifndef WEBSERVICEACTIONWIDGET_H
#define WEBSERVICEACTIONWIDGET_H
#include "webserviceaction.h"

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
    QLabel* autoActionLabel;

    WebServiceAction *actions;
    WebServiceAction *autoAction;
};

#endif // WEBSERVICEACTIONWIDGET_H
