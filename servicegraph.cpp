#include "servicegraph.h"
#include "drawgraph.h"

ServiceGraph::ServiceGraph(QWidget *parent) :
    QWidget(parent)
{
    init();
    resize(recommendWidth, recommendHeight);
    setWindowTitle(tr("��������ͼ"));
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
    recommendHeight = 200;
    recommendWidth = 350;

    pointsName.clear();
    for (int i = 0; i < 9; i++) {
        pointsName.push_back(QString("a%1").arg(i+1));
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
        points[i].setX(points[i].x()/2);
        points[i].setY(points[i].y()/2);
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
    DrawGraph::DrawAll(this, points, pointsName, colors, graph, tr("���� %1").arg(flowId));
}
