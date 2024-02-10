#ifndef EVENT_H
#define EVENT_H

#include <vector>
#include "../headers/Node.h"
#include "../constants/EventConstants.h"

using namespace std;

class Event{
    public:
    Event(vector<Node> &nodes, double time, event_type type);

    vector<Node> nodes;
    event_type type;

    double time;

    virtual void processEvent() = 0;
};

class Compare {
public:
    bool operator()(Event a, Event b)
    {
        if (a.time < b.time) {
            return true;
        }
        return false;
    }
};


#endif