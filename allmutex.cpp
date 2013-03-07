#include "allmutex.h"

QWaitCondition nextStepCond;

QMutex eventWidgetMutex;
QMutex serviceGraphMutex;
QMutex actionWidgetMutex;
QMutex nextStepMutex;
QMutex stateWidgetMutex;

QMutex eventHistoryWidgetMutex;
QMutex serviceFlowInfoWidgetMutex;
QReadWriteLock actionWidgetReadWriteMutex;
