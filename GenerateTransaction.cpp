#include "headers/GenerateTransaction.h"

GenerateTransaction::GenerateTransaction(Node *generator, long txn_ID, vector<Node *> &nodes, event_type type, double time)
{
    this->generator = generator;
    this->txn_ID = txn_ID;
    this->time = time;
}


void GenerateTransaction::processEvent(){
    Transaction txn = generate();
    transmit(txn);
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
    Transaction txn(generator->id, receiver_ID, amount, txn_ID, time);

    // Update transaction pool and balance
    generator->transaction_pool[txn_ID] = txn;
    
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

        // -------------------------- Calculate latency ---------------------------------
        // 1. Calculate capacity
        double capacity;
        if (generator->node_type == NODE_SLOW || neighbour->node_type == NODE_SLOW)
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

        
        
        ReceiveTransaction rb(neighbour, generator, txn, nodes, type, time);

        // TODO : Push Receive Transaction event in event queue
        
    }
}

// -------------------- Generate next transaction after some delay ----------------------------------
void GenerateTransaction::initializeNextTransaction(long txn_ID)
{

    // after a delay this node again generates a new transaction
    double delay;
    delay = exponentialDistribution(1.0 / TRANSACTION_INTERARRIVAL_MEAN);
    GenerateTransaction gt(generator, txn_ID, nodes, type, delay);
    // TODO---> event_queue.push(&gt)
}