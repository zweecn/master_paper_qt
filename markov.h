#ifndef MARKOV_H
#define MARKOV_H
#include "markovrecord.h"

#include <QList>

class Markov
{
public:
    Markov();

    static QList<MarkovRecord> noActionRecords(MarkovState & state);
    static QList<MarkovRecord> terminateRecords(MarkovState & state);
    static QList<MarkovRecord> redoRecords(MarkovState & state);
    static QList<MarkovRecord> replaceRecords(MarkovState & state);
    static QList<MarkovRecord> reCompositeRecords(MarkovState & state);
    static QList<MarkovRecord> getRecords(MarkovState & state);

    static bool helper(MarkovState & state);
    static bool helper(MarkovAction & action);
};

#endif // MARKOV_H
