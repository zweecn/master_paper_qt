#ifndef WEBSERVICEEVENTRECORDITEM_H
#define WEBSERVICEEVENTRECORDITEM_H
#include "webserviceevent.h"
#include "webserviceaction.h"
#include "webservicerecovery.h"

class WebServiceEventRecordItem
{
public:
    WebServiceEventRecordItem();

    WebServiceEventRecordItem(const WebServiceEventRecordItem & other);
    WebServiceEventRecordItem & operator =(const WebServiceEventRecordItem& other);
    bool operator ==(const WebServiceEventRecordItem& other) const;

    WebServiceEvent event;
    MarkovResultItem result;
    QList<MarkovResultItem> validResultList;
};

#endif // WEBSERVICEEVENTRECORDITEM_H
