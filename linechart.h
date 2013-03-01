#ifndef LINECHART_H
#define LINECHART_H

#include <QWidget>
#include <QPoint>

class LineChart : public QWidget
{
    Q_OBJECT
public:
    explicit LineChart(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *);

signals:

public slots:

private:
    void init();

    int xLength;
    int yLength;
    int delta;
    QPoint origin;
};

#endif // LINECHART_H
