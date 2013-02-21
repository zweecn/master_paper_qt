#ifndef ACTIVITY_H
#define ACTIVITY_H
#include "atomservice.h"
#include "resource.h"

//class AtomService;
//class Resource;
class Activity
{
public:
    Activity();

    AtomService *blindService;
    Resource *resource;

    int number;
    double x;
    int state;
    int redoCount;
    int replaceCount;
};

#endif // ACTIVITY_H
