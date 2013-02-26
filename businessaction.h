#ifndef BUSINESSACTION_H
#define BUSINESSACTION_H
#include <QList>

class Resource;
class Activity;

class BusinessAction
{
public:
    BusinessAction();

    enum
    {
        RESOURCE_REPLACE = 1,
        RESOURCE_TRANSPORT = 2,
        RESOURCE_DO_NOTHING = 3,
        RESOURCE_TERMINATE_NEED = 4,
        RESOURCE_ADD_NEW_NEED = 5
    };

    int type;
    int reward;

    Activity * bug;
    Resource * replaceResource;
    QList<Resource*> otherFlowResource;
    int otherFlowId;
    QList<int> otherActivities;
};

#endif // BUSINESSACTION_H
