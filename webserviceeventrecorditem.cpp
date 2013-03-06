#include "webserviceeventrecorditem.h"
#include <cmath>

WebServiceEventRecordItem::WebServiceEventRecordItem()
{
}

bool WebServiceEventRecordItem::operator ==(const WebServiceEventRecordItem& other) const
{
    double e = 0.00001;
    return other.event == event && other.action == action
            && fabs(other.potentialReward - potentialReward) < e
            && fabs(other.probility - probility) < e;
}
