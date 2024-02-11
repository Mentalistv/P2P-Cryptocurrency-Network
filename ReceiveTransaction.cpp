#include "headers/ReceiveTransaction.h"

ReceiveTransaction::ReceiveTransaction(Node* receiver, Node* sender, Transaction txn, vector<Node*> &nodes, event_type type, double time){
    this->receiver = receiver;
    this->sender = sender;
    this->txn = txn;
    this->nodes = nodes;
    this->type = type;
    this->time = time;
}

void ReceiveTransaction::processEvent(){

    int status = receive();
    if(status){
       return; // transaction already exists or some other error 
    }

    transmit();

}

// ----------------------------- This funtiom receives transation and updates Node's local data --------------------------
int ReceiveTransaction::receive(){
    
    // add transaction to the transaction pool if it does not already exists
    if(receiver->transaction_pool.count(txn.getTxnID())){
        return 1;
    }
    else{
        receiver->transaction_pool[txn.getTxnID()] = txn;
        return 0;
    }

}

// ------------------------------------------------------------------------------------------------
// This function will check all the neighbours of the transmitter,(exclude the sender)
// compute latency delay for each,
// and push "ReceiveTransaction" event in the event queue 
// ------------------------------------------------------------------------------------------------
void ReceiveTransaction::transmit(){
    int nof_links = receiver->nof_links;
    for (int i = 0; i < nof_links; i++)
    {

        // -------------------------- get the neighbour node ----------------------------
        int neighbour_id = receiver->links.at(i);
        if (neighbour_id = sender->id) // not transmit to sender again
        {
            continue;
        }
        Node *neighbour = nodes.at(neighbour_id);

        
        // -------------------------- Calculate latency ---------------------------------
        // 1. Calculate capacity (in Mbps)
        double capacity;
        if (receiver->node_type == NODE_SLOW || neighbour->node_type == NODE_SLOW)
        {
            capacity = 5;
        }
        else
        {
            capacity = 100;
        }

        // 2. Size of transaction to be transmitted (in bits)
        int size_of_txn = 1024*8; // 1KB

        // 3. Generate queuing delay from exponential distribution (in ms)
        double queue_delay;
        double mean = 96 / capacity;
        queue_delay = exponentialDistribution(1 / mean);
        
        // 4. Compute latency
        double latency = size_of_txn / (capacity * 1000) + queue_delay + LIGHT_SPEED_DELAY; 

        
        
        ReceiveTransaction rb(neighbour, receiver, txn, nodes, type, time);

        // TODO : Push Receive Transaction event in event queue
        
    }
}

