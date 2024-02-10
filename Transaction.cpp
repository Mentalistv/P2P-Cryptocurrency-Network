#include "headers/Transaction.h"

Transaction::Transaction(int sender, int receiver, double amount, long txn_ID, double time){
    this->sender = sender;
    this->receiver = receiver;
    this->amount = amount;
    this->txn_ID = txn_ID;
    this->time = time;
}
