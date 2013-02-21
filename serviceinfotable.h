#ifndef SERVICEINFOTABLE_H
#define SERVICEINFOTABLE_H

#include <QWidget>
#include <QTableWidget>
#include <QList>

class AtomService;
class ServiceInfoTable : public QWidget
{
    Q_OBJECT
public:
    explicit ServiceInfoTable(QWidget *parent = 0);

signals:

public slots:
    void drawTable();

private:
    enum {COLUMN_SIZE = 7};
    QTableWidget* infoTable;

};

#endif // SERVICEINFOTABLE_H
