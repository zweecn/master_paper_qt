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
