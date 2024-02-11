#include <iostream>
#include <queue>
#include <ctime>

using namespace std;

class Event{
    enum event_type{
        GENERATE_TRANSACTION,
        RECEIVE_TRANSACTION,
        MINE_BLOCK,
        RECEIVE_BLOCK
    };

    public:

    time_t arrival_time;

    void process_next_event();
};

typedef struct Event Event;

class Compare {
public:
    bool operator()(Event a, Event b)
    {
        if (a.arrival_time < b.arrival_time) {
            return true;
        }
        return false;
    }
};


