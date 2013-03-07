#ifndef BUSINESSEVENTRECORDITEM_H
#define BUSINESSEVENTRECORDITEM_H
#include "businessevent.h"
#include "businessaction.h"

class BusinessEventRecordItem
{
public:
    BusinessEventRecordItem();

    BusinessEventRecordItem(const BusinessEventRecordItem & other);
    BusinessEventRecordItem & operator =(const BusinessEventRecordItem& other);
    bool operator ==(const BusinessEventRecordItem& other) const;

    BusinessEvent event;
    BusinessAction action;
};


#endif // BUSINESSEVENTRECORDITEM_H
