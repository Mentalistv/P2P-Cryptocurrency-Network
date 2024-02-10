#include <iostream>

#include "Utility.h"
#include "Event.h"
#include "Node.h"
#include "Transaction.h"
#include "ReceiveTransaction.h"
#include "../constants/SimulationParamenters.h"
using namespace std;

class GenerateTransaction : public Event
{

public:
    Node *generator;
    long txn_ID;

    GenerateTransaction(Node *generator, long txn_ID, vector<Node*> &nodes, event_type type, double time);

    
    //
    void processEvent() override;
    
    // ------------------------------------------------------------------------------------------------ 
    //this function creates transaction object, add it to generator node's and adjust the balance 
    //-------------------------------------------------------------------------------------------------
    Transaction generate();


    // ------------------------------------------------------------------------------------------------
    // This function will check all the neighbours of the transmitter,
    // compute latency delay for each 
    // and push "ReceiveTransaction" event in the event queue 
    // ------------------------------------------------------------------------------------------------
    void transmit(Transaction txn);

    // -------------------- Generate next transaction after some delay ---------------------------------- 
    void initializeNextTransaction(long txn_ID);
};
