#ifndef WORKFLOW_H
#define WORKFLOW_H
#include <QMap>
#include <QSet>
#include <QList>

class AtomService;
class Resource;

class WorkFlow
{
public:
    static WorkFlow * Instance()
    {
        if( 0 == _instance)
        {
            _instance =  new WorkFlow();
        }
        return _instance;
    }

    int getActivitySize();
    bool hasEdge(int i, int j);
    QSet<int> getSuffixs(int x);
    QSet<int> getPrefixs(int x);

    bool makeService();
    bool makeResource();

    QList<AtomService> all_service;
    QList<Resource> all_resource;

protected:
    WorkFlow();
    static WorkFlow * _instance;

private:
    bool readGraphInfo();
    bool initPrefixSuffix();
    bool readService();
    bool readResource();

    int** graph;
    int activitySize;
    int serviceSize;
    QMap<int, QSet<int> > prefixMap;
    QMap<int, QSet<int> > suffixMap;
};

#endif // WORKFLOW_H
