#include "../headers/Event.h"
#include "../headers/Globals.h"

priority_queue<Event, vector<Event>, EventComparator> eventQueue;


void GenerateTransaction::processEvent(){
    Transaction txn = generate();
    transmit(txn);
    initializeNextTransaction();
}

// ------------------------------------------------------------------------------------------------
// this function creates transaction objectand add it to generator node's transaction pool
//-------------------------------------------------------------------------------------------------
Transaction GenerateTransaction::generate()
{

    double amount;
    int receiver_ID;

    // Taking a random amount from range (1 to user's balance)
    double lower_bound_amount = 1;
    double upper_bound_amount = generator->balance;
    amount = uniformDistributionDouble(lower_bound_amount, upper_bound_amount);

    // Taking a random node id from range (1 to #nodes - except the generator's id)
    int lower_bound_id = 0;
    int upper_bound_id = NUMBER_OF_NODES;
    receiver_ID = generator->id;
    while (receiver_ID == generator->id)
    { 
        // ensure randomly generated "id" is not equal to the generator's id
        receiver_ID = uniformDistributionInt(lower_bound_id, upper_bound_id);
    }

    // create object of Transaction
    Transaction txn(generator->id, receiver_ID, amount, time);

    // Update transaction pool and balance
    generator->transaction_pool[txn.getTxnID()] = txn;
    
    return txn;
}

// ------------------------------------------------------------------------------------------------
// This function will check all the neighbours of the transmitter,
// compute latency delay for each
// and push "ReceiveTransaction" event in the event queue
// ------------------------------------------------------------------------------------------------
void GenerateTransaction::transmit(Transaction txn)
{
    int nof_links = generator->nof_links;
    for (int i = 0; i < nof_links; i++)
    {
        // -------------------------- get the neighbour node ----------------------------
        int neighbour_id = generator->links.at(i);
        Node *neighbour = nodes.at(neighbour_id);

        // // -------------------------- Calculate latency ---------------------------------
        double latency = generator->calculateLatencyToNode(neighbour, TRANSACTION_SIZE_BYTES);
        
        
        ReceiveTransaction rb(neighbour, generator, txn, nodes, type, time + latency);

        eventQueue.push(rb);
        
    }
}

// -------------------- Generate next transaction after some delay ----------------------------------
void GenerateTransaction::initializeNextTransaction()
{

    // after a delay this node again generates a new transaction
    double delay;
    delay = exponentialDistribution(1.0 / TRANSACTION_INTERARRIVAL_MEAN);
    
    GenerateTransaction gt(generator, nodes, type, time + delay);
    eventQueue.push(gt);
}