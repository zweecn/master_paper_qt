#include "linechart.h"
#include "drawgraph.h"
#include <QDebug>
#include <QPainter>

LineChart::LineChart(QWidget *parent) :
    QWidget(parent)
{
    init();
    resize(600, 500);
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


    DrawGraph::DrawEdge(this, origin, QPoint(xLength + origin.x(), origin.y()));
    DrawGraph::DrawEdge(this, origin, QPoint(origin.x(), yLength + origin.y()));

    QPainter painter(this);
    if (!painter.isActive()) {
        painter.begin(this);
    }
    painter.setPen(Qt::SolidLine);
    painter.translate(origin);
    painter.drawText(QPoint(-10, 10), "0");
    painter.drawText(QPoint(xLength, 20), "time");
    painter.drawText(QPoint(-50, yLength), "Reward");


    painter.end();
}

void LineChart::init()
{
    xLength = 1000;
    yLength = 200;

    origin = QPoint(100, 20);


}
