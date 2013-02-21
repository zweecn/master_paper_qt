#include <QtGui>
#include "serviceinfotable.h"
#include "atomservice.h"
#include "workflow.h"

//extern QList<AtomService> all_service;

ServiceInfoTable::ServiceInfoTable(QWidget *parent) :
    QWidget(parent)
{
    resize(600, 400);
    drawTable();
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(infoTable);
    setLayout(mainLayout);
}


void ServiceInfoTable::drawTable()
{
    infoTable = new QTableWidget();
    infoTable->setColumnCount(COLUMN_SIZE);
    QList<AtomService>& all_service = WorkFlow::Instance()->all_service;
    infoTable->setRowCount(all_service.size());
    QStringList header;
    header << "ActivityNo" << "ServiceNo" << "Price" << "Reliability" << "ExecTime" << "is free" << "redo count";
    infoTable->setHorizontalHeaderLabels(header);

    for (int i = 0; i < infoTable->rowCount(); i++) {
        QString temp;
        infoTable->setItem(i, 0, new QTableWidgetItem(temp.setNum(all_service[i].activityNumber)));
        infoTable->setItem(i, 1, new QTableWidgetItem(temp.setNum(all_service[i].id)));
        infoTable->setItem(i, 2, new QTableWidgetItem(temp.setNum(all_service[i].price)));
        infoTable->setItem(i, 3, new QTableWidgetItem(temp.setNum(all_service[i].reliability)));
        infoTable->setItem(i, 4, new QTableWidgetItem(temp.setNum(all_service[i].execTime)));
        infoTable->setItem(i, 5, new QTableWidgetItem(temp.setNum(all_service[i].free)));
        infoTable->setItem(i, 6, new QTableWidgetItem(temp.setNum(all_service[i].redoCount)));
    }
}
