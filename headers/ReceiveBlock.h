#include "../headers/Event.h"

class ReceiveBlock : public Event {
    int receiver_id;
    Block block;

    ReceiveBlock(vector<Node*> &nodes, double time, event_type type, int receiver_id, Block block) : Event(nodes, time, type), receiver_id(receiver_id), block(block) {}

    void processEvent() override;

};