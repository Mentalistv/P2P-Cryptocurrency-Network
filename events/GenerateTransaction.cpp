#include "../headers/Event.h"
#include "../headers/Globals.h"


GenerateTransaction::GenerateTransaction(
    Node *generator, event_type type, double time
) : Event (time, type), generator(generator) {}

GenerateTransaction::~GenerateTransaction() {}

void GenerateTransaction::processEvent() const {
    // cout << "Inside GenerateTransaction event at time " << time << endl;
    Transaction txn = generate();
    transmit(txn);
    initializeNextTransaction();
}

void GenerateTransaction::printEvent() const {
    cout << "Event Generate Transaction at time = " << time << endl; 
}

// ------------------------------------------------------------------------------------------------
// this function creates transaction objectand add it to generator node's transaction pool
//-------------------------------------------------------------------------------------------------
Transaction GenerateTransaction::generate() const
{

    double amount;
    int receiver_ID;

    // Taking a random amount from range (1 to user's balance)
    double lower_bound_amount = 1;
    double upper_bound_amount = generator->balance/SPEND_FRACTION_OF_BALANCE;
    amount = uniformDistributionDouble(lower_bound_amount, upper_bound_amount);

    // printf("Amount = %f balance = %f\n", amount, generator->balance);

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
    generator->transaction_pool[txn.txn_ID] = txn;
    generator->balance -= amount;

    // printf("\n------------- Node %d generated transaction %d at time %f ----------------------\n", generator->id, txn.txn_ID, time);
    
    return txn;
}

// ------------------------------------------------------------------------------------------------
// This function will check all the neighbours of the transmitter,
// compute latency delay for each
// and push "ReceiveTransaction" event in the event queue
// ------------------------------------------------------------------------------------------------
void GenerateTransaction::transmit(Transaction txn) const
{

    // printf("Transmitted to ...\n");
    
    for (int link: generator->links)
    {
        // -------------------------- get the neighbour node ----------------------------
        int neighbour_id = link;
        Node *neighbour = nodes.at(neighbour_id);

        // // -------------------------- Calculate latency ---------------------------------
        double latency = generator->calculateLatencyToNode(neighbour, TRANSACTION_SIZE_BYTES);
        event_queue.push(new ReceiveTransaction(neighbour, generator, txn, type, time + latency));

        // printf("Node %d with delay %f\n", neighbour_id, latency);
    }

    // printf("\n");
}

// -------------------- Generate next transaction after some delay ----------------------------------
void GenerateTransaction::initializeNextTransaction() const
{
    // after a delay this node again generates a new transaction
    double delay;
    delay = exponentialDistribution((double) TRANSACTION_INTERARRIVAL_MEAN * NUMBER_OF_NODES);
    // if (generator->id == 0)
    //     cout << "delay " << delay << endl;

    event_queue.push(new GenerateTransaction(generator, type, time + delay));

}