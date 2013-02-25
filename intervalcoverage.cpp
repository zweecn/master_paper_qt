#include "intervalcoverage.h"

IntervalCoverage::IntervalCoverage(int _n, SegMent* _data, int _start, int _ed)
    : n(_n), data(_data), start(_start), ed(_ed)
{
}

IntervalCoverage::IntervalCoverage()
{
}

IntervalCoverage::~IntervalCoverage()
{
}

int IntervalCoverage::minCost()
{
    std::sort(data, data+n);
    if (start < data[0].left || ed > data[n-1].right)
    {
        return -1;
    }
    int* mem = new int[data[n-1].right + 2];
    memset(mem, 0, (data[n-1].right + 2) * sizeof(int));
    int* f = mem + 1;
    for (int i = 0; i < data[n-1].right + 1; i++)
    {
        f[i] = INT_MAX;
    }
    int first = 0;
    for (int i = 0; i < n; i++)
    {
        if (data[i].right >= start && data[i].left <= start)
        {
            first = i;
            break;
        }
    }
    std::cout << "first=" << first << std::endl;
    for (int i = 0; i < data[first].left; i++)
    {
        f[i] = 0;
    }
    for (int i = first; i < n; i++)
    {
        if (data[i].right < start) continue;
        int minLeft = INT_MAX;
        for (int j = data[i].left-1; j < data[i].right; j++)
        {
            minLeft = std::min(*(f+j), minLeft);
        }
        if (minLeft == INT_MAX)
        {
            break;
        }
        else
        {
            f[data[i].right] = minLeft + data[i].price;
        }
    }
    int res = -1;
    for (int i = ed; i <= data[n-1].right; i++)
    {
        if (f[i] != INT_MAX)
        {
            res = f[i];
            break;
        }
    }
    for (int i = 0; i <= data[n-1].right; i++)
    {
        std::cout << "f[" << i << "]=" << f[i] << std::endl;
    }
    std::cout << "ed=" << ed << " f[ed]=" << f[ed] << std::endl;
    delete[] mem;
    return res;
}

void IntervalCoverage::runExample()
{
    data = new SegMent[3];
    n = 3;
    start = 3;
    ed = 3;

    data[0].left = 0;
    data[0].right = 1;
    data[0].price = 3;

    data[1].left = 3;
    data[1].right = 4;
    data[1].price = 2;

    data[2].left = 0;
    data[2].right = 0;
    data[2].price = 1;

    std::cout << minCost() << std::endl;

    delete[] data;
}
