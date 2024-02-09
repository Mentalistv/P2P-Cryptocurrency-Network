#include "../headers/Event.h"


Event::Event(vector<Node> &nodes, double time, event_type type) {
    this->nodes = nodes;
    this->time = time;
    this->type = type;
}