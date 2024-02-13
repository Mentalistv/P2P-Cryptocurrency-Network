#include "../headers/Event.h"

using namespace std;

Event::Event(double time, event_type type) : time(time), type(type) {}

Event::~Event() {}

void Event::processEvent() const {
    cout << "Processing generic Event: " << endl;
}

void Event::printEvent() const {
    cout << "Print generic Event at time =  " << time << endl;
}

bool EventComparator::operator()(const Event* event1, const Event* event2) const {
    return event1->time > event2->time;
}