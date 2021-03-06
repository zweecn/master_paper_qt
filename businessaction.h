#ifndef BUSINESSACTION_H
#define BUSINESSACTION_H
#include <QList>

class Resource;
class Activity;
class BusinessEvent;

class BusinessAction
{
public:
    BusinessAction();

    BusinessAction(const BusinessAction& other);
    BusinessAction& operator ==(const BusinessAction & other);
    bool operator ==(const BusinessAction& other) const;


    QString name();
    QString toString();

    static QList<QString> nameList();

    enum
    {
        RESOURCE_REPLACE = 0,
        RESOURCE_TRANSPORT = 1,
        RESOURCE_DO_NOTHING = 2,
        RESOURCE_TERMINATE_NEED = 3,
        RESOURCE_ADD_NEW_NEED = 4,

        ACTIONS_COUNT = 5
    };

    int type;
    int reward;
    BusinessEvent * event;

    Activity * bug;
    Resource * oldResource;
    Resource * newResource;

    int otherFlowId;
    QList<Resource*> otherFlowResource;
    QList<int> otherFlowActivities;

    bool isActive;
};

#endif // BUSINESSACTION_H
