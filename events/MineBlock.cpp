#include "../headers/Event.h"
#include "../headers/Globals.h"

MineBlock::MineBlock(
    double time, event_type type, int miner_id, int mine_on_block_id
) : Event(time, type), miner_id(miner_id), prev_block_id(prev_block_id){}

MineBlock::~MineBlock() {}

void MineBlock::processEvent() const {
    // int new_prev_block_id;

    // int curr_deepest_block_id = nodes[miner_id]->deepest_block_id;

    // // checking if its the part of the longest chain
    // if(curr_deepest_block_id != prev_block_id){
    //     int curr_deepest_block_height = nodes[miner_id]->blocks[curr_deepest_block_id].height;

    //     int temp = prev_block_id;
    //     int given_chain_height = 0;

    //     while(nodes[miner_id]->blocks[temp].id != -1){
    //         given_chain_height++;
    //         temp = nodes[miner_id]->blocks[temp].id;
    //     }

    //     if(given_chain_height < curr_deepest_block_height)  return;
    //     new_prev_block_id = prev_block_id;
    // }
    // else{
    //     new_prev_block_id = curr_deepest_block_id;
    // }

    // // MOD

    // auto itr = nodes[miner_id]->transaction_pool.begin();
    // int count = 1023;
    // vector <Transaction> txn_to_be_included;

    // while(count-- && itr != nodes[miner_id]->transaction_pool.end()){
    //     txn_to_be_included.push_back(*itr);
    //     itr++;
    // }

    // for(auto x: txn_to_be_included){
    //     nodes[miner_id]->txn_pool.erase(x.TXNID);
    // }

    // Block* new_block = new Block("id", miner_id, new_prev_block_id, time, txn_to_be_included);


    Node* miner = nodes[miner_id];
    
    // If the deepest chain has changed since I started mining, do nothing here
    if (prev_block_id != miner->deepest_block_id) {
        return;
    }

    int count = 999;
    vector <Transaction> txns;

    // Mark the used transactions
    unordered_map<long, bool> used_txns;
    Block temp = miner->blocks[prev_block_id];
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

    miner->deepest_block_id = new_block.id;

    transmitBlock(new_block);

}

void MineBlock::transmitBlock(Block block) const {
    Node* miner = nodes[miner_id];

    for (int neighbour_id: miner->links) {
        double delay = 13; //TODO
        event_queue.push(new ReceiveBlock(time + delay, RECEIVE_BLOCK, neighbour_id, miner_id, block));
    }
}

double MineBlock::getPoWDelay() const {
    Node* miner = nodes[miner_id];

    
}