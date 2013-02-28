#ifndef BUSINESSACTIONWIDGET_H
#define BUSINESSACTIONWIDGET_H

#include <QWidget>

class QTableWidget;
class QLabel;
class BusinessAction;


class BusinessActionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BusinessActionWidget(QWidget *parent = 0);

    void setBusinessAction(BusinessAction* _actions);
    void setAutoBusinessAction(BusinessAction* _autoAction);
    BusinessAction* getBusinessAction();
    BusinessAction* getAutoBusinessAction();

signals:
    void updateActionsSignal();

public slots:
    void updateActionTable();

private:
    void createActionTable();

    QTableWidget* actionTable;
    QLabel* autoActionLabel;

    BusinessAction *actions;
    BusinessAction *autoAction;
};

#endif // BUSINESSACTIONWIDGET_H
