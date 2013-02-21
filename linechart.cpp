#include "linechart.h"
#include "drawgraph.h"
#include <QDebug>

LineChart::LineChart(QWidget *parent) :
    QWidget(parent)
{
    resize(600, 400);
    setWindowTitle(tr("’€œﬂÕº"));
}


void LineChart::paintEvent(QPaintEvent *)
{
//    srand(1);
//    int first = rand() % 300;
//    for (int i = 0; i < 500; i += 10) {
//        int second = rand() % 300;
//        qDebug() << first << second;
//        DrawGraph::DrawLine(this, QPoint(i, first), QPoint(i+10, second));
//        first = second;
//    }

    DrawGraph::DrawEdge(this, QPoint(0, 0), QPoint(400, 0));
    DrawGraph::DrawEdge(this, QPoint(0, 0), QPoint(0, 400));
}
