//#ifndef MARKOVACTION_H
//#define MARKOVACTION_H
//#include "atomservice.h"
//#include "markovstate.h"

//#include <QList>

//struct RecomposeNode
//{
//    int activityId;
//    AtomService* oldService;
//    AtomService* newService;
//};

//class MarkovAction
//{
//public:
//    MarkovAction();
//    MarkovAction(const MarkovAction& other);
//    bool init();

//    QString name();
//    QString toString();

//    MarkovAction & operator =(const MarkovAction & other);
//    bool operator ==(const MarkovAction & other) const;
//    bool operator <(const MarkovAction & other) const;

//    double getPosibility();
//    double getPriceCost();
//    double getTimeCost();

//    double getReplacePosibility();
//    double getReplacePriceCost();
//    double getReplaceTimeCost();
//    double getReComposePosibility();
//    double getReComposePriceCost();
//    double getReComposeTimeCost();

//    static AtomService* nextFreeService(int activityId);
//    static QList<RecomposeNode> recomposeFreeService(int activityId);

//    // Actions:
//    static MarkovAction noAction(MarkovState & state);
//    static MarkovAction terminate(MarkovState & state);
//    static MarkovAction redo(MarkovState & state);
//    static MarkovAction replace(MarkovState & state);
//    static MarkovAction reComposite(MarkovState & state);

//    enum
//    {
//        ACTION_COUNT = 5,

//        MAX_REDO_COUNT = 1,
//        MAX_TERMINATE_COUNT = 1,
//        MAX_REPLACE_COUNT = 1,
//        MAX_RECOMPOSITE_COUNT = 1,

//        ERROR_ACTION = -1,
//        A_NO_ACTION = 0,
//        A_TERMINATE = 1,
//        A_RE_DO = 2,
//        A_REPLACE = 3,
//        A_RE_COMPOSITE = 4
//    };

//    int id;
//    int type;
//    int bugActivityId;

//    AtomService * oldService;
//    AtomService * newService;

//    QList<RecomposeNode> oldNewServiceList;

////    QList<int> replaceActivityList;
////    QList<AtomService*> oldServiceList;
////    QList<AtomService*> newServiceList;
//};

//#endif // MARKOVACTION_H
