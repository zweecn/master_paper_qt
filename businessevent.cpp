#include "businessevent.h"
#include "workflow.h"
#include "config.h"
#include "activity.h"
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <QDebug>

BusinessEvent::BusinessEvent()
{
    t = 0;      // time
    n = 0;      // flow id
    a = 0;      // activity id
    type = NORMAIL;   // event type
    t1 = 0;     // start time
    t2 = 0;     // end time
//    amount = 0; // amount of resource
}

BusinessEvent BusinessEvent::random(int currTime, Activity ** activities, int workflowCount)
{
//    qDebug() << "BusinessEvent::random() begin...";
    BusinessEvent e;
    e.t = currTime;
    srand(time(0));
    int i = rand() % workflowCount;
    Activity* startActivity = activities[i];
    for (int j = 0; j < WorkFlow::Instance()->getActivitySize(); j++)
    {
        int p = (rand() % 100);
        if (startActivity[j].state == Activity::FINISHED) {
            continue;
        } else if (p > startActivity[j].blindService->reliability) {
//            qDebug() << p << startActivity[j].blindService->reliability;
            e.n = i;
            e.a = j;
            e.type = rand() % UC_COUNT;
            e.t1 = rand() % (startActivity[WorkFlow::Instance()->getActivitySize()-1].lateComplate
                             - startActivity[j].earlyStart) + currTime;
            e.t2 = rand() % (startActivity[WorkFlow::Instance()->getActivitySize()-1].lateComplate
                             - startActivity[j].earlyStart) + currTime;
            return e;
        }
    }
    return e;
}
