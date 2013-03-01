#ifndef ALLMUTEX_H
#define ALLMUTEX_H

#include <QMutex>
#include <QWaitCondition>

extern QWaitCondition nextStepCond;

extern QMutex eventWidgetMutex;
extern QMutex serviceGraphMutex;
extern QMutex actionWidgetMutex;
extern QMutex nextStepMutex;

#endif // ALLMUTEX_H
