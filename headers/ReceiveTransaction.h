#include <iostream>

#include "Utility.h"
#include "Event.h"
#include "Node.h"
#include "Transaction.h"
#include "../constants/SimulationParamenters.h"

using namespace std;

class ReceiveTransaction : public Event
{
public:
    Node* Receiver;
    Node* Sender;
    Transaction txn;

    void processEvent() override;
    
    ReceiveTransaction(Node* Receiver, Node* Sender, Transaction txn, vector<Node*> &nodes, event_type, double time);

    void receive();
    void transmit();

};