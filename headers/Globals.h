#ifndef GLOBAL_VARIABLE_H
#define GLOBAL_VARIABLE_H

#include "./Event.h"

extern vector<Node*> nodes;

extern priority_queue<Event*, vector<Event*>, EventComparator> event_queue; // Declaration

#endif
