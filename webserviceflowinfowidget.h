#ifndef WEBSERVICEFLOWINFOWIDGET_H
#define WEBSERVICEFLOWINFOWIDGET_H

#include <QWidget>
#include <QMutex>

class QTableWidget;
class QLabel;

class WebServiceFlowInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WebServiceFlowInfoWidget(QWidget *parent = 0);


signals:


public slots:


private:
    void createFlowInfoTable();

    QTableWidget* flowInfoTable;
    QLabel* flowInfoLabel;

};

#endif // WEBSERVICEFLOWINFOWIDGET_H
