#include "widget.h"
#include <QPainter>
#include "drawgraph.h"
#include <QDebug>
#include <windows.h>
#include <QTimer>
#include <QCoreApplication>

Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
    resize(600, 400);
    setWindowTitle(tr("服务流程图(样例)"));
}

Widget::~Widget()
{

}

//paint event will be called when this widget update, you can also call update() manually
void Widget::paintEvent(QPaintEvent *)
{
//    QPoint p1(200, 200);
//    QPoint p2(300, 200);
//    DrawGraph::DrawEdge(this, p1, p2);//E
//    DrawGraph::DrawEdge(this, QPoint(200, 200), QPoint(300, 200));//E
//    DrawGraph::DrawEdge(this, QPoint(200, 200), QPoint(200, 300));//N
//    DrawGraph::DrawEdge(this, QPoint(200, 200), QPoint(100, 200));//W
//    DrawGraph::DrawEdge(this, QPoint(200, 200), QPoint(200, 100));//S
//    DrawGraph::DrawEdge(this, QPoint(200, 200), QPoint(300, 100));//NE
//    DrawGraph::DrawEdge(this, QPoint(200, 200), QPoint(300, 300));//SE
//    DrawGraph::DrawEdge(this, QPoint(200, 200), QPoint(100, 100));//NW
//    DrawGraph::DrawEdge(this, QPoint(200, 200), QPoint(100, 300));//SW

    QList<QString> pointsName;
    pointsName.reserve(2);
    pointsName.push_back("A");
    pointsName.push_back("B");

    QList<QPoint> points;
    points.push_back(QPoint(200, 200));
    points.push_back(QPoint(300, 200));

    QList<QColor> colors;
    colors.push_back(QColor::fromRgb(255, 0, 0));
    colors.push_back(QColor::fromRgb(0, 0, 255));

    int** graph = new int*[pointsName.size()];
    for (int i = 0; i < pointsName.size(); i++) {
        graph[i] = new int[pointsName.size()];
        memset(graph[i], 0, sizeof(int) * pointsName.size());
    }

//    for (int i = 0; i < pointsName.size(); i++) {
//        for (int j = 0; j < pointsName.size(); j++) {
//            qDebug() << graph[i][j];
//        }
//    }

    graph[0][1] = 1;

    DrawGraph::DrawAll(this, points, pointsName, colors, graph, QString());

    for (int i = 0; i < pointsName.size(); i++) {
        delete[] graph[i];
    }
    delete[] graph;

//    for (int i = 0; i < pointsName.size(); i++) {
//        DrawGraph::DrawPoint(this, points[i], pointsName[i]);
//    }
//    for (int i = 0; i < pointsName.size(); i++) {
//        for (int j = 0; j < pointsName.size(); j++) {
//            if (graph[i][j] == 1) {
//                DrawGraph::DrawEdge(this, points[i], points[j]);
//            }
//        }
//    }
}
