#include "../headers/Event.h"
#include "../headers/Globals.h"


ReceiveTransaction::ReceiveTransaction(
    Node* receiver, Node* sender, Transaction txn, event_type type, double time
) : Event(time, type), receiver(receiver), sender(sender), txn(txn) {}

ReceiveTransaction::~ReceiveTransaction() {}

void ReceiveTransaction::processEvent() const {
    cout << "Inside ReceiveTransaction event at time " << time << endl;
    int status = receive();
    if(status){
       return; // transaction already exists or some other error 
    }

    transmit();

}

// ----------------------------- This funtiom receives transation and updates Node's local data --------------------------
int ReceiveTransaction::receive() const {
    
    // add transaction to the transaction pool if it does not already exists
    if(receiver->transaction_pool.count(txn.txn_ID)){
        return 1;
    }
    else{
        receiver->transaction_pool[txn.txn_ID] = txn;
        return 0;
    }

}

// ------------------------------------------------------------------------------------------------
// This function will check all the neighbours of the transmitter,(exclude the sender)
// compute latency delay for each,
// and push "ReceiveTransaction" event in the event queue 
// ------------------------------------------------------------------------------------------------
void ReceiveTransaction::transmit() const {
    for (int link: receiver->links)
    {
        // -------------------------- get the neighbour node ----------------------------
        int neighbour_id = link;
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

        // 2. Size of transReceiveTransaction(Node* receiver, Node* sender, Transaction txn, vector<Node*> &nodes, event_type type, double time, EventQueue& event_queue) : Event(nodes, time, type), receiver(receiver), sender(sender), txn(txn) {}action to be transmitted (in bits)
        int size_of_txn = 1024*8; // 1KB

        // 3. Generate queuing delay from exponential distribution (in ms)
        double queue_delay;
        double mean = 96 / capacity;
        queue_delay = exponentialDistribution(1 / mean);
        
        // 4. Compute latency
        double latency = size_of_txn / (capacity * 1000) + queue_delay + LIGHT_SPEED_DELAY; 
        
        ReceiveTransaction rb(neighbour, receiver, txn, type, time);

        // event_queue.push(&rb);
    }
}

