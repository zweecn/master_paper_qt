#ifndef BUSINESSEVENT_H
#define BUSINESSEVENT_H

class BusinessEvent
{
public:
    BusinessEvent();

    static BusinessEvent random(int currTime);

    int t;
    int a;
    int type;
    int t1;
    int t2;
};

#endif // BUSINESSEVENT_H
