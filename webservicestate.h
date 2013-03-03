#ifndef WEBSERVICESTATE_H
#define WEBSERVICESTATE_H

#include <QList>

class Activity;

class WebServiceState
{
public:
    WebServiceState();
    ~WebServiceState();

    static int valueOfState(int state);

    WebServiceState & operator = (const WebServiceState & other);

//    Activity* getFirstActivity();
//    void setFirstActivity(Activity* _firstActivity);

    int getGlobalState();
    void setGlobalState(int _globalState);

    enum
    {
        NOT_READY = 0,
        READY = 1,
        EXEC = 2,
        FAIL = 3,
        FINISH = 4,
        CANCEL = 5,
        STATE_COUNT = 6
    };

private:
    bool init();

    Activity* faultActivity;
    int globalState;

    int id;
    bool finished;
    QList<WebServiceState*> nextStateArray;
    QList<Activity*> nextToDoActivityList;
    Activity* nextToDoActivity;

};

#endif // WEBSERVICESTATE_H
