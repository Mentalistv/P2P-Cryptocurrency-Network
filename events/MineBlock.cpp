#include "../headers/Event.h"
#include "../headers/Globals.h"

MineBlock::MineBlock(
    double time, event_type type, int miner_id, int mine_on_block_id
) : Event(time, type), miner_id(miner_id), prev_block_id(prev_block_id){}

MineBlock::~MineBlock() {}

void MineBlock::processEvent() const {
    // // Node *miner = &nodes[miner_id];
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

}