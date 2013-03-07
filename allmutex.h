#ifndef ALLMUTEX_H
#define ALLMUTEX_H

#include <QMutex>
#include <QWaitCondition>
#include <QReadWriteLock>

extern QWaitCondition nextStepCond;

extern QMutex eventWidgetMutex;
extern QMutex serviceGraphMutex;
extern QMutex actionWidgetMutex;
extern QMutex nextStepMutex;
extern QMutex stateWidgetMutex;

extern QMutex eventHistoryWidgetMutex;
extern QMutex serviceFlowInfoWidgetMutex;
extern QReadWriteLock actionWidgetReadWriteMutex;
#endif // ALLMUTEX_H
