#include "../headers/Event.h"

// struct EventComparator {
//     bool operator()(const Event& a, const Event& b) {
//         return a.time > b.time; // For min-heap priority queue
//     }
// };


bool EventComparator::operator()(Event& a, Event& b) {
    if (a.time < b.time) {
        return true;
    }
    return false;
}