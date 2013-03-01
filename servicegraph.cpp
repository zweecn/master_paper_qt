#include "servicegraph.h"
#include "drawgraph.h"
#include "allmutex.h"

ServiceGraph::ServiceGraph(QWidget *parent) :
    QWidget(parent)
{
    init();
    resize(recommendWidth, recommendHeight);
    setWindowTitle(tr("服务流程图"));

    connect(this, SIGNAL(updateGraphSignal()), this, SLOT(update()));
}

ServiceGraph::~ServiceGraph()
{
    for (int i = 0; i < pointsName.size(); i++) {
        delete[] graph[i];
    }
    delete[] graph;
}

bool ServiceGraph::init()
{
    flowId = 0;
    recommendHeight = 180;
    recommendWidth = 300;

    pointsName.clear();
    for (int i = 0; i < 9; i++) {
        pointsName.push_back(QString("a%1").arg(i));
    }

    points.clear();
    points.push_back(QPoint(100, 250));
    points.push_back(QPoint(250, 100));
    points.push_back(QPoint(500, 100));
    points.push_back(QPoint(250, 200));
    points.push_back(QPoint(200, 300));
    points.push_back(QPoint(300, 300));
    points.push_back(QPoint(400, 250));
    points.push_back(QPoint(500, 250));
    points.push_back(QPoint(600, 250));

    for (int i = 0; i < points.size(); i++) {
        points[i].setX(points[i].x()/2.2);
        points[i].setY(points[i].y()/2.2);
    }

    colors.clear();
    for (int i = 0; i < 9; i++) {
        colors.push_back(QColor::fromRgb(0, 255, 0));
    }

    graph = new int*[pointsName.size()];
    for (int i = 0; i < pointsName.size(); i++) {
        graph[i] = new int[pointsName.size()];
        memset(graph[i], 0, sizeof(int) * pointsName.size());
    }

    graph[0][1] = 1;
    graph[0][3] = 1;
    graph[0][4] = 1;
    graph[1][2] = 1;
    graph[3][6] = 1;
    graph[4][5] = 1;
    graph[5][6] = 1;
    graph[6][7] = 1;
    graph[7][8] = 1;
    graph[2][8] = 1;

    return true;
}

void ServiceGraph::paintEvent(QPaintEvent *)
{
    serviceGraphMutex.lock();
    DrawGraph::DrawAll(this, points, pointsName, colors, graph, tr("流程 %1").arg(flowId));
    serviceGraphMutex.unlock();
}


void ServiceGraph::setPointsName(QList<QString>& _pointsName)
{
    pointsName = _pointsName;
    emit updateGraphSignal();
}

void ServiceGraph::setPoints(QList<QPoint>& _points)
{
    points = _points;
    emit updateGraphSignal();
}

void ServiceGraph::setColors(QList<QColor>& _colors)
{
    colors = _colors;
    emit updateGraphSignal();
}

void ServiceGraph::setGraph(int ** _graph)
{
    graph = _graph;
    emit updateGraphSignal();
}

void ServiceGraph::setFlowId(int _flowId)
{
    flowId = _flowId;
    emit updateGraphSignal();
}

QList<QString>& ServiceGraph::getPointsName()
{
    return pointsName;
}

QList<QPoint>& ServiceGraph::getPoints()
{
    return points;
}

QList<QColor>& ServiceGraph::getColors()
{
    return colors;
}

int** ServiceGraph::getGraph()
{
    return graph;
}

int ServiceGraph::getFlowId()
{
    return flowId;
}

int ServiceGraph::getRecommedHeight()
{
    return recommendHeight;
}

int ServiceGraph::getRecommedWidth()
{
    return recommendWidth;
}
