#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
protected:
    void paintEvent(QPaintEvent *);
private:
    QPoint src;
    QPoint dst;
};

#endif // WIDGET_H
