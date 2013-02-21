#ifndef DRAWGRAPH_H
#define DRAWGRAPH_H

#include <QPoint>
#include <QColor>
class QPaintDevice;
class DrawGraph
{
public:
    DrawGraph();
    static void DrawEdge(QPaintDevice *device, const QPoint &src, const QPoint &dst);
    static void DrawPoint(QPaintDevice *device, const QPoint &pos, const QString &text, const QColor &color);
    static void DrawText(QPaintDevice *device, const QString &str);
    static void DrawAll(QPaintDevice *device, const QList<QPoint> &points,
                            const QList<QString> &pointsName, const QList<QColor> &colors,
                            int** graph, const QString &str);

    static const int pointSize = 15;//you can adjust this for point size
    static const int arrowSize = 5;
};

#endif // DRAWGRAPH_H
