#include <iostream>

#include "Event.h"
#include "ReceiveTransaction.h"
#include "../constants/SimulationParamenters.h"
using namespace std;

class GenerateTransaction : public Event
{

public:
    Node *generator;

    GenerateTransaction(Node *generator, vector<Node*> &nodes, event_type type, double time);

    //
    void processEvent() override;
    
    // ------------------------------------------------------------------------------------------------ 
    //this function creates transaction object and add it to generator node's transaction pool 
    //-------------------------------------------------------------------------------------------------
    Transaction generate();


    // ------------------------------------------------------------------------------------------------
    // This function will check all the neighbours of the transmitter,
    // compute latency delay for each 
    // and push "ReceiveTransaction" event in the event queue 
    // ------------------------------------------------------------------------------------------------
    void transmit(Transaction txn);

    // -------------------- Generate next transaction after some delay ---------------------------------- 
    void initializeNextTransaction();
};
