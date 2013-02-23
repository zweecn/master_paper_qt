#ifndef CRITICALPATH_H
#define CRITICALPATH_H

/**
 * CRITICALPATH(�򵥰�) �ؼ�·��
 * ���룺�޻�ͼg��
 * �����(1)�����������ʱ��ec��
 *       (2)�����������ʱ��lc��
 *       (3)�ؼ�·��prev��
 * �ṹ��ͼg���ڽӾ����ʾ
 * �㷨���������򣬶�̬�滮(DP)
 * ���Ӷȣ�O(|V|^2)
 */
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <list>
#include <stack>
#include <queue>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <functional>
#include <climits>

class Activity;

class CriticalPath
{
public:
    CriticalPath();
    CriticalPath(int n, std::vector<std::vector<int> > &g);
    void reset(int n, std::vector<std::vector<int> > &g);
    void runExample();
    void run();
    int getLatestTime();
    int getEarlyComplateTime(Activity* startActivity, int activityNumber);
    int getLateComplateTime(Activity* startActivity, int activityNumber);
    int getEarlyStartTime(Activity* startActivity, int activityNumber);
    int getLateStartTime(Activity* startActivity, int activityNumber);

private:
    int n;                        // n ���������
    std::vector<std::vector<int> > g;        // g ��ͼ(graph)(���ڽӾ���(adjacent matrix)��ʾ)
    std::vector<int> seq;            // seq ����������(sequence)

    std::vector<int> ec;                // ec : �������ʱ��(early complete time)
    std::vector<int> lc;                // lc : �������ʱ��(late complete time)
    std::vector<int> cp;                // cp : �ؼ�·��(critical path)

    bool topSort();        // ��������
    void calculate();    // �ؼ�·��

};


#endif // CRITICALPATH_H
