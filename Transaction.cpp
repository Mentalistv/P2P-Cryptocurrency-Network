#include "headers/Transaction.h"

Transaction::Transaction(int sender, int receiver, double amount, double time){
    this->sender = sender;
    this->receiver = receiver;
    this->amount = amount;
    this->txn_ID = txn_ID;
    this->time = time;

    txn_ID = txn_count++;
}

long Transaction::getTxnID(){
    return txn_ID;
}
