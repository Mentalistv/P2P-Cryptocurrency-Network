#include "../headers/Event.h"

struct EventComparator {
    bool operator()(const Event* event1, const Event* event2) const {
        // Custom logic to compare events, you can modify this as per your requirements
        return event1->time > event2->time;
    }
};