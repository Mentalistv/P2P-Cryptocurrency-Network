#include "../headers/Event.h"
#include "../headers/Globals.h"

MineBlock::MineBlock(
    double time, event_type type, int miner_id, Block new_block
) : Event(time, type), miner_id(miner_id), new_block(new_block) {}

MineBlock::~MineBlock() {}

void MineBlock::processEvent() const {
    Node* miner = nodes[miner_id];

    if (miner->node_character_type == HONEST) {
        honestMinerMines();
    } else {
        selfishMinerMines();
    }
}


void MineBlock::honestMinerMines() const {
    Node* miner = nodes[miner_id];

    // If the deepest chain has changed since I started mining, do nothing here
    if (new_block.prev_block_id != miner->deepest_block_id) {
        return;
    }

    miner->blocks.insert({new_block.id, new_block});
    miner->deepest_block_id = new_block.id;
    miner->blocks[miner->deepest_block_id].arrival_time = time;
    miner->updateBalances(new_block);

    // printf("\n------------- Node %d mined block %d at time %f ----------------------\n", miner->id, new_block.id, time);
    transmitBlock(new_block);
}


void MineBlock::selfishMinerMines() const {
    Node* miner = nodes[miner_id];
    // TODO
    
    if(miner->lead == 0) {
        if(new_block.prev_block_id != miner->deepest_block_id)
            return;
    }

    miner->lead++;
    miner->blocks.insert({new_block.id, new_block});

    if(miner->lead < 2)
        transmitBlock(new_block);
    else{
        miner->private_chain.push(new_block);
    }

    double delay = getPoWDelay(miner->hashing_power);
    Block new_block = miner->createNewBlock(time);
    event_queue.push(new MineBlock(time + delay, MINE_BLOCK, miner_id, new_block));
}


void MineBlock::printEvent() const {
    cout << "Event MineBlock at time = " << time << endl; 
}

void MineBlock::transmitBlock(Block block) const {
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