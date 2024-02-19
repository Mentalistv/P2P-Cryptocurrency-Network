#include "../headers/Event.h"
#include "../headers/Globals.h"

MineBlock::MineBlock(
    double time, event_type type, int miner_id, int mine_on_block_id
) : Event(time, type), miner_id(miner_id), mine_on_block_id(mine_on_block_id) {}

MineBlock::~MineBlock() {}

void MineBlock::processEvent() const {
    Node* miner = nodes[miner_id];

    // If the deepest chain has changed since I started mining, do nothing here
    if (mine_on_block_id != miner->deepest_block_id) {
        return;
    }

    int count = 1023;
    vector <Transaction> txns;

    // Mark the used transactions
    unordered_map<long, bool> used_txns;

    Block temp = miner->blocks[mine_on_block_id];
    while (temp.id != -1) {
        for (Transaction t: temp.transactions) {
            used_txns[t.txn_ID] = true;
        }
        temp = miner->blocks[temp.prev_block_id];
    }

    // Add transactions except the used ones
    for (auto it = miner->transaction_pool.begin(); it != miner->transaction_pool.end(); ++it) {
        const pair<long, Transaction>& element = *it;
        if (!used_txns[element.first]) {
            txns.push_back(element.second);
            count--;
        }
        if (count == 0)
            break;
    }

    Transaction coinbase(COINBASE_TXN_SENDER_ID, miner_id, 50.0, time);
    txns.push_back(coinbase);

    Block new_block = Block(BLOCK_ID_GENERATOR++, miner_id, miner->deepest_block_id, time, txns);
    int prev_height = miner->blocks[miner->deepest_block_id].height;
    new_block.height = prev_height + 1;

    // cout << time << " when block mined miner = " << miner_id << " mine on " << mine_on_block_id << endl;


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