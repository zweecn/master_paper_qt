#ifndef CRITICALPATH_H
#define CRITICALPATH_H

/**
 * CRITICALPATH(简单版) 关键路径
 * 输入：无环图g。
 * 输出：(1)各点最早完成时间ec；
 *       (2)各点最晚完成时间lc；
 *       (3)关键路径prev。
 * 结构：图g用邻接矩阵表示
 * 算法：拓扑排序，动态规划(DP)
 * 复杂度：O(|V|^2)
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
    int n;                        // n ：顶点个数
    std::vector<std::vector<int> > g;        // g ：图(graph)(用邻接矩阵(adjacent matrix)表示)
    std::vector<int> seq;            // seq ：拓扑序列(sequence)

    std::vector<int> ec;                // ec : 最早完成时间(early complete time)
    std::vector<int> lc;                // lc : 最晚完成时间(late complete time)
    std::vector<int> cp;                // cp : 关键路径(critical path)

    bool topSort();        // 拓扑排序
    void calculate();    // 关键路径

};


#endif // CRITICALPATH_H
