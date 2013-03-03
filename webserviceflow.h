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


    Activity* activities;
};

#endif // WEBSERVICEFLOW_H
