#ifndef BUSINESSEVENT_H
#define BUSINESSEVENT_H

class Activity;

class BusinessEvent
{
public:
    BusinessEvent();

    static BusinessEvent random(int currTime, Activity ** activities, int workflowCount);

    enum{
            NORMAIL = -1,
            RESOUCE_NOT_USE = 0,
            NEED_ADD = 1,
            NEED_REDUCE = 2,
            NEED_CANCEL = 3,
            UC_COUNT = 4
        };

    int t;      // time
    int n;      // flow id
    int a;      // activity id
    int type;   // event type
    int t1;     // start time
    int t2;     // end time
    int amount; // amount of resource
};

#endif // BUSINESSEVENT_H
