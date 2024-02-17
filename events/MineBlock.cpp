#include "../headers/Event.h"
#include "../headers/Globals.h"

MineBlock::MineBlock(
    double time, event_type type, Block new_block
) : Event(time, type), new_block(new_block) {}

MineBlock::~MineBlock() {}

void MineBlock::processEvent() const {
    int miner_id = new_block.owner_id;
    Node* miner = nodes[miner_id];

    // If the deepest chain has changed since I started mining, do nothing here
    if (new_block.prev_block_id != miner->deepest_block_id) {
        cout << "Mine block rejected" << endl;
        return;
    }

    // insert block to the node's tree and uddate the balances
    miner->blocks.insert({new_block.id, new_block});
    miner->deepest_block_id = new_block.id;
    miner->updateBalances(new_block);

    // printf("\n------------- Node %d mined block %d at time %f ----------------------\n", miner->id, new_block.id, time);
    transmitBlock(new_block);
}

void MineBlock::printEvent() const {
    cout << "Event MineBlock at time = " << time << endl; 
}

void MineBlock::transmitBlock(Block block) const {
    int miner_id = block.owner_id;
    Node* miner = nodes[miner_id];

    // printf("Transmitted to ...\n");

    for (int neighbour_id: miner->links) {
        Node *neighbour = nodes.at(neighbour_id);
        int message_size_bytes = block.getMessageSizeBytes();
        double delay = miner->calculateLatencyToNode(neighbour, message_size_bytes);
        
        // printf("MIne wala delay = %f\n", delay);
        event_queue.push(new ReceiveBlock(time + delay, RECEIVE_BLOCK, neighbour_id, miner_id, block));

        // printf("Node %d with delay %f\n", neighbour_id, delay);
    }

    // printf("\n");
}