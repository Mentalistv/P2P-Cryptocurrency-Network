#include "../headers/Transaction.h"

Transaction::Transaction() {
    // default constructor
}

Transaction::Transaction(int sender, int receiver, double amount, double time){
    this->sender = sender;
    this->receiver = receiver;
    this->amount = amount;
    this->time = time;

    this->txn_ID = this->txn_count++;
}
