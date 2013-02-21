#ifndef LINECHART_H
#define LINECHART_H

#include <QWidget>

class LineChart : public QWidget
{
    Q_OBJECT
public:
    explicit LineChart(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *);

signals:

public slots:

};

#endif // LINECHART_H
