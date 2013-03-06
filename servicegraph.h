#ifndef SERVICEGRAPH_H
#define SERVICEGRAPH_H

#include <QWidget>
#include <QList>
#include <QMutex>

class ServiceGraph : public QWidget
{
    Q_OBJECT
public:
    explicit ServiceGraph(QWidget *parent = 0);
    ~ServiceGraph();

    void zoomOut(double multiple);

    void setPointsName(QList<QString>& _pointsName);
    void setPoints(QList<QPoint>& _points);
    void setColors(QList<QColor>& _colors);
    void setGraph(int ** _graph);
    void setFlowId(int _flowId);

    QList<QString>& getPointsName();
    QList<QPoint>& getPoints();
    QList<QColor>& getColors();
    int** getGraph();
    int getFlowId();

    int getRecommedHeight();
    int getRecommedWidth();

signals:
    void updateGraphSignal();

public slots:

protected:
    void paintEvent(QPaintEvent *);
    bool init();
private:

    QList<QString> pointsName;
    QList<QPoint> points;
    QList<QColor> colors;
    int** graph;

    int flowId;
    int recommendHeight;
    int recommendWidth;
};

#endif // SERVICEGRAPH_H
