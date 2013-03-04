#ifndef CONFIG_H
#define CONFIG_H
#include <QMap>

class Config
{
public:
    static Config * Instance()
    {
        if( 0 == _instance)
        {
            _instance =  new Config();
        }
        return _instance;
    }

//    int getUCTypeCount();
    int getWorkflowCount();
    int getOneActivityServiceCount();
    int getServiceMinPrice();
    int getServiceMaxPrice();
    int getServiceMinReliability();
    int getServiceMaxReliability();
    int getServiceMinExectime();
    int getServiceMaxExectime();

    int getResourceCount();
    int getResourceTypeCount();
    int getResourceUseTypeCount();
    int getResourceMinPriceCount();
    int getResourceMaxPriceCount();

    double getWeaken();
    int getPuinishmentPerSecond();
    int getPuinishmentFailed();
    int getReduceLayerSize();
    bool isExtendTree();

protected:
    Config();
    static Config * _instance;

private:
    bool init();

    QMap<QString, QString> confMap;
};

#endif // CONFIG_H
