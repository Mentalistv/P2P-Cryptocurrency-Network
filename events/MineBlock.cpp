#include "../headers/Event.h"
#include "../headers/Globals.h"

MineBlock::MineBlock(
    double time, event_type type, int miner_id, int mine_on_block_id
) : Event(time, type), miner_id(miner_id), mine_on_block_id(mine_on_block_id){
    // calculating and adding delay here
    long double delay = getPoWDelay();
    this->time = time + delay;
}

MineBlock::~MineBlock() {}

void MineBlock::processEvent() const {
    cout << "At mineblock time = ********************************************** " << time << endl;

    Node* miner = nodes[miner_id];

    cout << "prev_block_id " << mine_on_block_id << " dbi " << miner->deepest_block_id << endl;
    cout << "no of blocks = " << miner->blocks.size();

    
    // If the deepest chain has changed since I started mining, do nothing here
    if (mine_on_block_id != miner->deepest_block_id) {
        return;
    }

    int count = 999;
    vector <Transaction> txns;

    // Mark the used transactions
    unordered_map<long, bool> used_txns;
    Block temp = miner->blocks[mine_on_block_id];
    while (temp.id != -1) {
        for (Transaction t: temp.transactions) {
            used_txns[t.txn_ID] = true;
        }
        temp = miner->blocks[temp.prev_block_id];
        // cout << "ahahah " << temp.id << endl;
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
    cout << "inserted new block " << new_block.id << " " << new_block.prev_block_id << endl;

    cout << "BLOCK_ID_GENERATOR " << BLOCK_ID_GENERATOR << endl;

    miner->deepest_block_id = new_block.id;

    transmitBlock(new_block);

}

void MineBlock::transmitBlock(Block block) const {
    Node* miner = nodes[miner_id];

    for (int neighbour_id: miner->links) {
        Node *neighbour = nodes.at(neighbour_id);
        int message_size_bytes = block.getMessageSizeBytes();
        double delay = miner->calculateLatencyToNode(neighbour, message_size_bytes);
        
        printf("MIne wala delay = %f\n", delay);
        event_queue.push(new ReceiveBlock(time + delay, RECEIVE_BLOCK, neighbour_id, miner_id, block));
    }
}

long double MineBlock::getPoWDelay() const {
    Node* miner = nodes[miner_id];
    double delay = exponentialDistribution((double) BLOCK_INTERARRIVAL_TIME/ (double)miner->hashing_power);
    return delay;
}