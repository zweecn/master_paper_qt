#ifndef RESOURCE_H
#define RESOURCE_H

class Resource
{
public:
    Resource();
    int id;
    int type;
    int use_type;
    int price;

    bool operator ==(Resource &other);
};

#endif // RESOURCE_H
