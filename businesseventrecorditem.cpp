#include "businesseventrecorditem.h"

BusinessEventRecordItem::BusinessEventRecordItem()
{
}

BusinessEventRecordItem::BusinessEventRecordItem(const BusinessEventRecordItem & other)
{
    this->action = other.action;
    this->event = other.event;
}

BusinessEventRecordItem & BusinessEventRecordItem::operator =(const BusinessEventRecordItem& other)
{
    if (this == &other)
        return *this;
    this->action = other.action;
    this->event = other.event;

    return *this;
}

bool BusinessEventRecordItem::operator ==(const BusinessEventRecordItem& other) const
{
    return this->action == other.action && this->event == other.event;
}
