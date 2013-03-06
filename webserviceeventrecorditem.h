#ifndef WEBSERVICEEVENTRECORDITEM_H
#define WEBSERVICEEVENTRECORDITEM_H
#include "webserviceevent.h"
#include "webserviceaction.h"

class WebServiceEventRecordItem
{
public:
    WebServiceEventRecordItem();

    bool operator ==(const WebServiceEventRecordItem& other) const;

    WebServiceEvent event;
    WebServiceAction action;
    double potentialReward;
    double probility;
};

#endif // WEBSERVICEEVENTRECORDITEM_H
