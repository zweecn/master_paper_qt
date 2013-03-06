#ifndef WEBSERVICEFLOW_H
#define WEBSERVICEFLOW_H
#include "workflow.h"
#include "activity.h"

class WebServiceFlow
{
public:
    WebServiceFlow();
    bool init();

    WebServiceFlow & operator = (const WebServiceFlow & other);

    QString toString();
    bool isFinished();

    Activity* activities;
    int globalState;
};

#endif // WEBSERVICEFLOW_H
