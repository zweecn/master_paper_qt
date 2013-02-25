#ifndef INTERVALCOVERAGE_H
#define INTERVALCOVERAGE_H

#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<cmath>
#include<cctype>
#include<iostream>
#include<functional>
#include<algorithm>
#include <climits>

struct SegMent
{
    int left, right, price;
    SegMent(){}
    SegMent(int _l, int _r, int _price) : left(_l), right(_r), price(_price){}
    friend bool operator<(const SegMent a, const SegMent b){return a.right < b.right;}
};

class IntervalCoverage
{
public:
    IntervalCoverage(int n, SegMent* data, int start, int ed);
    IntervalCoverage();
    ~IntervalCoverage();
    int minCost();
    void runExample();
private:
    int n;
    int start;
    int ed;
    SegMent *data;
};


#endif // INTERVALCOVERAGE_H
