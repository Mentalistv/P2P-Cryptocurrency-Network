#include "../headers/Event.h"
#include "../headers/Globals.h"

priority_queue<Event, vector<Event>, EventComparator> eventQueue;


// TODO: Optkize if possible, maintain the blances for longest chain, update it if the longest chain changes 
bool ReceiveBlock::verifyBlock() {
    Node *receiver = nodes[receiver_id];
    int prev_block_id = incoming_block.prev_block_id;
    // Verify transactions in the block
    vector<double> balances = receiver->calculateBalancesFromBlock(prev_block_id);

    for (Transaction txn : incoming_block.transactions) {
        balances[txn.sender] -= txn.amount;
        balances[txn.receiver] += txn.amount;
    }

    // TODO : consider the initial balances

    // if any of the balances is negative, its invalid
    for (double balance: balances) {
        if (balance < 0)
            return false;
    }

    return true;
}


void ReceiveBlock::updateTransactionPool() {
    Node *receiver = nodes[receiver_id];

    for (Transaction txn: incoming_block.transactions) {
        if (receiver->transaction_pool.find(txn.getTxnID()) != receiver->transaction_pool.end())
            receiver->transaction_pool.erase(txn.getTxnID());
    }
}

void ReceiveBlock::processEvent() {
    receiveBlock();
}

void ReceiveBlock::receiveBlock() {
    Node *receiver = nodes[receiver_id];

    // if the block is already present, then return
    if (receiver->blocks.find(incoming_block.id) != receiver->blocks.end())
        return;

    int prev_block_id = incoming_block.prev_block_id;

    if (!verifyBlock())
        return;
    
    // Add the block to the tree
    int prev_height = receiver->blocks[prev_block_id].height;
    int new_height = prev_height + 1;
    receiver->blocks[incoming_block.id] = incoming_block;
    receiver->blocks[incoming_block.id].height = prev_height + 1;


    // Set deepest block to the incoming block if it has greatest height
    if (receiver->blocks[receiver->deepest_block_id].height < new_height) {
        receiver->deepest_block_id = incoming_block.id;

        // // remove the txns in the block from the receiver's TXN pool
        // updateTransactionPool();

        
        double delay = 13; // TODO: Mine block delay

        MineBlock mb(nodes, time + delay, MINE_BLOCK, receiver_id, incoming_block.id);
        eventQueue.push(mb);
    }

    // Add receive block events to transmit the received block
    for (int neighbour_id: receiver->links) {
        if (neighbour_id != sender_id) {
            double latency = 13; //TODO
            ReceiveBlock newRcvBlkEvent(nodes, time + latency, RECEIVE_BLOCK, neighbour_id, receiver_id, incoming_block);
            eventQueue.push(newRcvBlkEvent);
        }       
    }

    // Add mine block events
}