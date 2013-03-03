#ifndef MARKOV_H
#define MARKOV_H
#include "markovrecord.h"

#include <QList>

class Markov
{
public:
    Markov();

    static QList<MarkovRecord> noActionRecords(MarkovState & state);
};

#endif // MARKOV_H
