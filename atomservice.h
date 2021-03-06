#ifndef ATOMSERVICE_H
#define ATOMSERVICE_H

class AtomService
{
public:
    AtomService();

    int activityNumber;
    int id;

    int redoCount;
    bool free;

    int price;
    int reliability;
    int execTime;

    bool operator ==(AtomService &other);
    bool operator <(AtomService &other);
};

#endif // ATOMSERVICE_H
