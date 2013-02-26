#ifndef INTERVALCOVERAGE_H
#define INTERVALCOVERAGE_H
#include <QList>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <cctype>
#include <iostream>
#include <functional>
#include <algorithm>
#include <climits>

struct SegMent
{
    int id, left, right, price;
    SegMent(){}
    SegMent(int _id, int _l, int _r, int _price) : id(_id), left(_l), right(_r), price(_price){}
    friend bool operator<(const SegMent a, const SegMent b)
    {
        if (a.right == b.right)
        {
            if (a.left < b.left)
            {
                return true;
            }
            else if (a.left > b.left)
            {
                return false;
            }
            else
            {
                return a.price < b.price;
            }
        }
        return a.right < b.right;
    }
};

class IntervalCoverage
{
public:
    IntervalCoverage(int _n, SegMent* _data, int _start, int _ed);
    ~IntervalCoverage();
    int minCost();
    int getResult();
    void printResult();
    QList<int>& getResultDataID();
    void reset(int _n, SegMent* _data, int _start, int _ed);

    static void runExample();

private:
    bool initMem(int size);
    bool freeMem();
    int n;
    int start;
    int ed;

    int res;
    int resPos;
    QList<int> resultDataID;

    SegMent *data;
    int *record;
    int *f;
    int *prefix;
};

#endif // INTERVALCOVERAGE_H
