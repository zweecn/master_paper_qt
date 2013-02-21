#ifndef SERVICEGRAPH_H
#define SERVICEGRAPH_H

#include <QWidget>

class ServiceGraph : public QWidget
{
    Q_OBJECT
public:
    explicit ServiceGraph(QWidget *parent = 0);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *);

};

#endif // SERVICEGRAPH_H
