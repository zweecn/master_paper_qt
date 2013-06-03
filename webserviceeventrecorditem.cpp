#include "webserviceeventrecorditem.h"
#include <cmath>

WebServiceEventRecordItem::WebServiceEventRecordItem()
{
}

WebServiceEventRecordItem::WebServiceEventRecordItem(const WebServiceEventRecordItem & other)
{
    this->event = other.event;
    this->result = other.result;
    this->validResultList = other.validResultList;
}

WebServiceEventRecordItem & WebServiceEventRecordItem::operator =(const WebServiceEventRecordItem& other)
{
    if (this == &other)
        return *this;
    this->event = other.event;
    this->result = other.result;
    this->validResultList = other.validResultList;
    return *this;
}

bool WebServiceEventRecordItem::operator ==(const WebServiceEventRecordItem& other) const
{
    return other.event == event && result == other.result
            && this->validResultList == other.validResultList;
}
