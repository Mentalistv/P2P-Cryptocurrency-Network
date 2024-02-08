#include <iostream>

#include "Node.h"

using namespace std;

class Transaction
{
    public:
    Node* sender;
    Node* receiver;
    double amount;
    int TXNID;
    double time;
};