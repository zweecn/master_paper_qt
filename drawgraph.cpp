#include "drawgraph.h"
#include <QPainter>
#include <cmath>
#include <QDebug>

DrawGraph::DrawGraph()
{
}

void DrawGraph::DrawPoint(QPaintDevice *device, const QPoint &pos, const QString &text, const QColor &color)
{
    QPainter painter(device);

    //set brush for point
//    QBrush brush(QColor::fromRgb(255, 0, 0), Qt::SolidPattern);
    QBrush brush(color, Qt::SolidPattern);
    painter.setBrush(brush);

    //no pen for boundary
    painter.setPen(Qt::NoPen);

    //begin paint
    if (!painter.isActive()) {
        painter.begin(device);
    }

    //draw src and dst
    painter.drawEllipse(pos, pointSize, pointSize);

    painter.setPen(Qt::SolidLine);
    painter.drawText(pos, text);

    painter.end();

//    qDebug() << "Draw " << text << " finished.";
}


void DrawGraph::DrawEdge(QPaintDevice *device, const QPoint &src, const QPoint &dst)
{

    QPainter painter(device);

//    //set brush for point
//    QBrush brush(QColor::fromRgb(255, 0, 0), Qt::SolidPattern);
//    painter.setBrush(brush);

//    //no pen for boundary
//    painter.setPen(Qt::NoPen);

//    //begin paint
    if (!painter.isActive()) {
        painter.begin(device);
    }

//    //draw src and dst
//    painter.drawEllipse(src, pointSize, pointSize);
//    painter.drawEllipse(dst, pointSize, pointSize);

    //calculate edge angle in degree
    int deltaX = (dst.x() - src.x());
    int deltaY = (dst.y() - src.y());
    double lineAngle = atan(double(deltaY)/deltaX) * 180 /3.1415;

    //restore pen to draw edge
    painter.setPen(Qt::SolidLine);

    //draw edge
    painter.drawLine(src.x(), src.y(), dst.x(), dst.y());

    //now start to draw arrow
    //first move origion point in coordinate to dst
    painter.translate(dst.x(), dst.y());

    //then rotate coordinate
    painter.rotate(lineAngle);

    //if dst is on the left of src, there are two case for arrow
    if (dst.x() < src.x())
    {
        painter.drawLine(arrowSize * 2, -arrowSize, 0, 0);
        painter.drawLine(arrowSize * 2, arrowSize, 0, 0);
    }
    else
    {
        painter.drawLine(-arrowSize * 2, -arrowSize, 0, 0);
        painter.drawLine(-arrowSize * 2, arrowSize, 0, 0);
    }
//    qDebug("%f", lineAngle);
//    qDebug() << "Draw line " << src << " -> " << dst << " finished.";
    //end paint
    painter.end();

    //you can change brush and pen above for your requirement
}

void DrawGraph::DrawText(QPaintDevice *device, const QString &str)
{
    QPainter painter(device);

    if (!painter.isActive()) {
        painter.begin(device);
    }
    painter.setPen(Qt::SolidLine);
    if (str != "") {
        painter.drawText(QPoint(20, 20), str);
    } else {
        QString planQos("预订全局QoS(Price, Exectime)");
        QString currQos("当前全局QoS(Price, Exectime)");
        QString event("Event(t, a, Type)");
        QString op("OP(不作为/重试/替换/重组/终止)");
        QString info(planQos + "\n" + currQos + "\n" + event + "\n" + op);
//        qDebug() << info;
        painter.drawText(QPoint(20, 20), info);
    }

    painter.end();
}

void DrawGraph::DrawAll(QPaintDevice *device, const QList<QPoint> &points,
                        const QList<QString> &pointsName, const QList<QColor> &colors,
                        int** graph, const QString &str)
{
    for (int i = 0; i < pointsName.size(); i++) {
        DrawGraph::DrawPoint(device, points[i], pointsName[i], colors[i]);
    }
    for (int i = 0; i < pointsName.size(); i++) {
        for (int j = 0; j < pointsName.size(); j++) {
            if (graph[i][j] == 1) {
                DrawGraph::DrawEdge(device, points[i], points[j]);
            }
        }
    }
    DrawGraph::DrawText(device, str);
}
