#include "businessevent.h"
#include "workflow.h"
#include "config.h"
#include <cstdlib>
#include <iostream>

BusinessEvent::BusinessEvent()
{
}

BusinessEvent BusinessEvent::random(int currTime)
{
    BusinessEvent e;
    e.t = currTime;
    e.a = std::rand() % WorkFlow::Instance()->getActivitySize();
    e.type = std::rand() % Config::Instance()->getUCTypeCount();
    e.t1 = std::rand();
    e.t2 = std::rand();
    return e;
}
