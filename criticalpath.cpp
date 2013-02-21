#include "criticalpath.h"

CriticalPath::CriticalPath()
{

}

CriticalPath::CriticalPath(int n, std::vector<std::vector<int> > &g)
{
    this->n = n;
    this->g = g;
}

void CriticalPath::reset(int n, std::vector<std::vector<int> > &g)
{
    this->n = n;
    this->g = g;
}

void CriticalPath::runExample()
{
    n = 9;
    g.assign(n, std::vector<int>(n, INT_MAX));
    g[0][1] = 6; g[0][2] = 4; g[0][3] = 5;
    g[1][4] = 1;
    g[2][4] = 1;
    g[3][5] = 2;
    g[4][6] = 9; g[4][7] = 7;
    g[5][7] = 4;
    g[6][8] = 2;
    g[7][8] = 4;
    if (topSort())
    {
         copy(seq.begin(), seq.end(), std::ostream_iterator<int>(std::cout, " ")); std::cout << std::endl;
         calculate();
         copy(ec.begin(), ec.end(), std::ostream_iterator<int>(std::cout, " ")); std::cout << std::endl;
         copy(lc.begin(), lc.end(), std::ostream_iterator<int>(std::cout, " ")); std::cout << std::endl;
         copy(cp.begin(), cp.end(), std::ostream_iterator<int>(std::cout, " ")); std::cout << std::endl;
         std::cout << "Latest time = " << getLatestTime() << std::endl;
    }
    else
    {
         std::cout << "Circles exist." << std::endl;
    }
}

void CriticalPath::run()
{
    if (topSort())
    {
        copy(seq.begin(), seq.end(), std::ostream_iterator<int>(std::cout, " ")); std::cout << std::endl;
        calculate();
        copy(ec.begin(), ec.end(), std::ostream_iterator<int>(std::cout, " ")); std::cout << std::endl;
        copy(lc.begin(), lc.end(), std::ostream_iterator<int>(std::cout, " ")); std::cout << std::endl;
        copy(cp.begin(), cp.end(), std::ostream_iterator<int>(std::cout, " ")); std::cout << std::endl;
        std::cout << "Latest time = " << getLatestTime() << std::endl;
    }
    else
    {
         std::cout << "Circles exist." << std::endl;
    }
}

bool CriticalPath::topSort()
{
    std::vector<int> inc(n, 0);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
             if (g[i][j] < INT_MAX) ++inc[j]; // ����ÿ���������ȣ�
    std::queue<int> que;
    for (int j = 0; j < n; ++j)
        if (inc[j] == 0) que.push(j); // �����������Ϊ0����ӡ�
    int seqc = 0;
    seq.resize(n);
    while (!que.empty())     // �������que�ǿգ�
    {
        int v = que.front(); que.pop();
        seq[seqc++] = v;      // ����v���ӣ�����seq�У�
        for (int w = 0; w < n; ++w)     // ��������vָ��Ķ���w��
            if (g[v][w] < INT_MAX)
                if (--inc[w] == 0) que.push(w); // ����w����ȣ����w�����Ϊ0����ӡ�
    }
    return seqc == n; // ���seq�Ѵ�������Ϊn�������������򣬷�����ڻ�·��
}

void CriticalPath::calculate()
{
    // �������ʱ�䣺ec[0] = 0;
    //               ec[v] = max(ec[u] + g[u][v])��
    ec.assign(n, INT_MIN);  ec[seq[0]] = 0;
    for (int i = 0; i < n - 1; ++i)
        for (int v = 0; v < n; ++v)
            if (g[seq[i]][v] < INT_MAX)
                ec[v] = std::max(ec[v], ec[seq[i]] + g[seq[i]][v]);
    // �������ʱ�䣺lc[n - 1] = ec[n - 1];
    //               lc[u] = min(lc[v] - g[u][v])��
    lc.assign(n, INT_MAX);    lc[seq[n - 1]] = ec[seq[n - 1]];
    for (int j = n - 1; j > 0; --j)
        for (int u = 0; u < n; ++u)
            if (g[u][seq[j]] < INT_MAX)
                lc[u] = std::min(lc[u], lc[seq[j]] - g[u][seq[j]]);
    // �ؼ�·����cp[0] = seq[0];
    //           if(�ɳ�ʱ��slack(u, v) = lc[v] - ec[u] - g[u][v]Ϊ��)
    //             { uΪ�ؼ�·���㣻���uΪseq[n - 1]��������}
    cp.clear(); cp.push_back(seq[0]);
    for (int i = 0; i < n - 1; ++i)
    {
        for (int v = 0; v < n; ++v)
            if (g[cp[i]][v] < INT_MAX)
            {
                int slack = lc[v] - ec[cp[i]] - g[cp[i]][v];
                if (slack == 0) { cp.push_back(v); break; }
            }
        if (cp.back() == seq[n - 1]) break;
    }
}

int CriticalPath::getLatestTime()
{
    return lc[lc.size()-1];
}
