#include "../headers/Event.h"

class MineBlock : public Event {
    int miner_id;
    int prev_block_id;

    MineBlock(vector<Node> &nodes, double time, event_type type, int miner_id) : Event(nodes, time, type), miner_id(miner_id), prev_block_id(prev_block_id){}

    void processEvent() override;

};