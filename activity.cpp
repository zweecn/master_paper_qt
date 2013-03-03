#include "activity.h"

Activity::Activity()
{
    blindService = 0;
    resource = 0;
    resourceList.clear();

    number = 0;
    x = 0;
    state = 0;
    redoCount = 0;
    replaceCount = 0;

    earlyStart = 0;
    lateStart = 0;
    earlyComplate = 0;
    lateComplate = 0;
}

Activity & Activity::operator = (const Activity & other)
{
    if (this == &other)
    {
        return *this;
    }
    blindService = other.blindService;
    resource = other.resource;

    resourceList = other.resourceList;

    number = other.number;
    x = other.x;
    state = other.state;
    redoCount = other.redoCount;
    replaceCount = other.replaceCount;

    earlyStart = other.earlyStart;
    lateStart = other.lateStart;
    earlyComplate = other.earlyComplate;
    lateComplate = other.lateComplate;

    return *this;
}

bool Activity::operator ==(const Activity & other)
{
    return (number == other.number);
}

bool Activity::operator <(const Activity & other)
{
    return (number < other.number);
}
