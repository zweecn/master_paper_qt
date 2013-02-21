#include "servicegraph.h"
#include "drawgraph.h"

ServiceGraph::ServiceGraph(QWidget *parent) :
    QWidget(parent)
{
    resize(600, 400);
    setWindowTitle(tr("服务流程图"));
}


void ServiceGraph::paintEvent(QPaintEvent *)
{
    QList<QString> pointsName;
    pointsName.reserve(2);
    pointsName.push_back("A");
    pointsName.push_back("B");
    pointsName.push_back("C");

    QList<QPoint> points;
    points.push_back(QPoint(200, 200));
    points.push_back(QPoint(300, 200));
    points.push_back(QPoint(100, 300));

    QList<QColor> colors;
    colors.push_back(QColor::fromRgb(255, 0, 0));
    colors.push_back(QColor::fromRgb(0, 0, 255));
    colors.push_back(QColor::fromRgb(0, 0, 255));

    int** graph = new int*[pointsName.size()];
    for (int i = 0; i < pointsName.size(); i++) {
        graph[i] = new int[pointsName.size()];
        memset(graph[i], 0, sizeof(int) * pointsName.size());
    }

    graph[0][1] = 1;
    graph[0][2] = 1;
    graph[1][2] = 1;

    DrawGraph::DrawAll(this, points, pointsName, colors, graph, "");

    for (int i = 0; i < pointsName.size(); i++) {
        delete graph[i];
    }
    delete[] graph;
}
