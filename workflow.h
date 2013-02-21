#ifndef WORKFLOW_H
#define WORKFLOW_H
#include <QMap>
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
    QMap<int, QList<int> > prefixMap;
    QMap<int, QList<int> > suffixMap;
};

#endif // WORKFLOW_H
