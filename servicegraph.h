#ifndef SERVICEGRAPH_H
#define SERVICEGRAPH_H

#include <QWidget>
#include <QList>

class ServiceGraph : public QWidget
{
    Q_OBJECT
public:
    explicit ServiceGraph(QWidget *parent = 0);
    ~ServiceGraph();

    QList<QString> pointsName;
    QList<QPoint> points;
    QList<QColor> colors;
    int** graph;

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *);
    bool init();
};

#endif // SERVICEGRAPH_H
