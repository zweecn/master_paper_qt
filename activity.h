#ifndef ACTIVITY_H
#define ACTIVITY_H
#include "atomservice.h"
#include "resource.h"
#include <QList>

class Activity
{
public:
    Activity();

    enum {FINISHED = -1};

    AtomService *blindService;
    Resource *resource;

    QList<Resource*> resourceList;

    int number;
    double x;
    int state;
    int redoCount;
    int replaceCount;

    int earlyStart;
    int lateStart;
    int earlyComplate;
    int lateComplate;
};

#endif // ACTIVITY_H
