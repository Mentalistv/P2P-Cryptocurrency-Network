#include "headers/ReceiveTransaction.h"

ReceiveTransaction::ReceiveTransaction(Node* Receiver, Node* Sender, Transaction txn, vector<Node*> &nodes, event_type type, double time){
    this->Receiver = Receiver;
    this->Sender = Sender;
    this->txn = txn;
    this->nodes = nodes;
    this->type = type;
    this->time = time;
}

