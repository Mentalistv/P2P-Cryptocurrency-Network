#include <iostream>

#include "Utility.h"
#include "Event.h"
#include "../constants/SimulationParamenters.h"

using namespace std;

class ReceiveTransaction : public Event
{
public:
    Node* receiver;
    Node* sender;
    Transaction txn;

    ReceiveTransaction(Node* receiver, Node* sender, Transaction txn, vector<Node*> &nodes, event_type type, double time);
    
    //
    void processEvent() override;
    

    // ----------------------------- This funtiom receives transation and updates Node's local data --------------------------
    int receive();

    // ------------------------------------------------------------------------------------------------
    // This function will check all the neighbours of the transmitter,(exclude the sender)
    // compute latency delay for each,
    // and push "ReceiveTransaction" event in the event queue 
    // ------------------------------------------------------------------------------------------------
    void transmit();

};