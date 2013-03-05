#ifndef WEBSERVICEACTION_H
#define WEBSERVICEACTION_H

#include <QList>
#include <QString>

struct ReplaceNode
{
    int activityId;
    int oldServiceId;
    int newServiceId;

    bool operator ==(const ReplaceNode & other)
    {
        return (activityId == other.activityId
                && oldServiceId == other.oldServiceId
                && newServiceId == other.newServiceId);
    }
};

class WebServiceAction
{
public:
    WebServiceAction();
    WebServiceAction(int _id, int _type);
    WebServiceAction(const WebServiceAction &other);
    bool operator ==(const WebServiceAction & other) const;
    WebServiceAction & operator =(const WebServiceAction & other);

    int hash();
    QString toString();

    enum
    {
        NO_NEED_DO = 0,
        TERMINATE = 1,
        DO_NOTHING = 2,
        REPLACE = 3,
        RE_COMPOSE = 4,
        RETRY = 5,

        ACTION_SIZE = 6
    };

    int id;
    int dc;
    int dt;

    int type;
    QList<ReplaceNode> replaceList;
};

#endif // WEBSERVICEACTION_H
