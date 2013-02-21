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

    int getResourceTypeCount();
    int getUCTypeCount();

protected:
    Config();
    static Config * _instance;

private:
    bool init();

    QMap<QString, QString> confMap;
};

#endif // CONFIG_H
