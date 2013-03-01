#ifndef BUSINESSSTATEWIDGET_H
#define BUSINESSSTATEWIDGET_H

#include <QWidget>


class QTableWidget;
class QLabel;
class Activity;

class BusinessStateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BusinessStateWidget(QWidget *parent = 0);

    void setActivities(Activity **_activities);
signals:
    void updateStateTableSignal();

public slots:
    void updateStateTable();

private:

    void createStateable();

    QTableWidget *stateTable;
    Activity **activities;
};

#endif // BUSINESSSTATEWIDGET_H
