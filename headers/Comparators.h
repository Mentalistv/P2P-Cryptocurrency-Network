#include "../headers/Event.h"

class CompareEvents {
public:
    bool operator()(Event& a, Event& b)
    {
        if (a.time < b.time) {
            return true;
        }
        return false;
    }
};