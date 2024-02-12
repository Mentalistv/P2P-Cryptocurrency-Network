#include "../headers/Event.h"
#include "../headers/Globals.h"


ReceiveTransaction::ReceiveTransaction(
    Node* receiver, Node* sender, Transaction txn, event_type type,  double time
) : Event(time, type), receiver(receiver), sender(sender), txn(txn) {}

ReceiveTransaction::~ReceiveTransaction() {}

void ReceiveTransaction::processEvent() const {
    // cout << "Inside ReceiveTransaction event at time " << time << endl;
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
        double latency = receiver->calculateLatencyToNode(neighbour, 1024);
        
        ReceiveTransaction rb(neighbour, receiver, txn, type, time);

        // event_queue.push(&rb);
    }
}

