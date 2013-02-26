#include "intervalcoverage.h"
#include <QDebug>

IntervalCoverage::IntervalCoverage(int _n, SegMent* _data, int _start, int _ed)
    : n(_n), data(_data), start(_start), ed(_ed)
{
    record = 0;
    f = 0;
    prefix = 0;

    // [1] new memory. init memory with INT_MAX
    std::sort(data, data+n);
    int size = std::max(data[n-1].right + 2, ed + 2);
    initMem(size);
}

IntervalCoverage::~IntervalCoverage()
{
    freeMem();
}

void IntervalCoverage::reset(int _n, SegMent* _data, int _start, int _ed)
{
    freeMem();

    n = _n;
    data = _data;
    start = _start;
    ed = _ed;

    record = 0;
    f = 0;
    prefix = 0;

    // [1] new memory. init memory with INT_MAX
    std::sort(data, data+n);
    int size = std::max(data[n-1].right + 2, ed + 2);
    initMem(size);
}

bool IntervalCoverage::initMem(int size)
{
    f = new int[size];
    memset(f, 0, size * sizeof(int));
    f++;

    record = new int[size];
    memset(record, 0, size * sizeof(int));
    record++;

    prefix = new int[size];
    memset(prefix, 0, size * sizeof(int));
    prefix++;

    for (int i = -1; i < size - 1; i++)
    {
        f[i] = prefix[i] = record[i] = INT_MAX;
    }

    return true;
}

bool IntervalCoverage::freeMem()
{
    if (--f)
        delete[] f;
    if (--record)
        delete[] record;
    if (--prefix)
        delete[] prefix;

    std::cout << "Free mem succeed.\n";
    return true;
}

int IntervalCoverage::minCost()
{
    resPos = ed;
    res = -1;
    if (start < data[0].left || ed > data[n-1].right)
    {
        return -1;
    }
    // [2] find first data could cover the start
    int first = 0;
    for (int i = 0; i < n; i++)
    {
        if (data[i].right >= start && data[i].left <= start)
        {
            first = i;
            break;
        }
    }
    // [3] init the value of data[first].left
    for (int i = -1; i < data[first].left; i++)
    {
        f[i] = 0;
    }
    // [4] algorithm
    for (int i = first; i < n; i++)
    {
        if (data[i].right < start) continue;
        int minLeft = INT_MAX;
        int minId = INT_MAX;
        int prefixTmp = INT_MAX;
        for (int j = data[i].left-1; j < data[i].right; j++)
        {
            if (minLeft > *(f+j))
            {
                minLeft = *(f+j);
                minId = data[i].id;
                prefixTmp = j;
            }
        }
        if (minLeft == INT_MAX)
        {
            break;
        }
        else if (f[data[i].right] > minLeft + data[i].price)
        {
            f[data[i].right] = minLeft + data[i].price;
            prefix[data[i].right] = prefixTmp;
            record[data[i].right] = minId;
        }
    }
    resultDataID.clear();
    for (int i = 0; i <= data[n-1].right; i++)
    {
        if (record[i] != INT_MAX)
        {
            resultDataID.append(record[i]);
        }
    }
    for (int i = ed; i <= data[n-1].right; i++)
    {
        if (f[i] != INT_MAX)
        {
            res = f[i];
            resPos = i;
            break;
        }
    }

    return res;
}

void IntervalCoverage::printResult()
{
    for (int i = -1; i <= data[n-1].right; i++)
    {
        std::cout << "f[" << i << "]=" << f[i]
        << " prefix[" << i << "]=" << prefix[i]
        << " record[" << i << "]=" << record[i]
        <<  std::endl;

    }
    std::cout << "resPos=" << resPos << " f[resPos]=" << f[resPos] << std::endl;
    std::cout << "The result is: " << res << std::endl;
}

QList<int>& IntervalCoverage::getResultDataID()
{
    return resultDataID;
}

int IntervalCoverage::getResult()
{
    return res;
}

void IntervalCoverage::runExample()
{
    SegMent* _d = new SegMent[3];
    int _n = 3;
    int _start = 0;
    int _ed = 4;

    _d[0].id = 0;
    _d[0].left = 0;
    _d[0].right = 1;
    _d[0].price = 3;

    _d[1].id = 1;
    _d[1].left = 2;
    _d[1].right = 3;
    _d[1].price = 2;

    _d[2].id = 2;
    _d[2].left = 3;
    _d[2].right = 4;
    _d[2].price = 1;

    IntervalCoverage ic(_n, _d, _start, _ed);
    ic.minCost();
    std::cout << ic.getResult() << std::endl;
    ic.printResult();
    for (int i = 0; i < ic.resultDataID.size(); i++) {
        std::cout << ic.resultDataID[i] << " ";
    }

    delete[] _d;
    std::cout << " END" << std::endl;
}

