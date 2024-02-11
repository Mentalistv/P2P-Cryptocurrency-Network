#include <iostream>

using namespace std;

class Transaction
{
private:
    long txn_ID;

public:
    int sender;
    int receiver;
    double amount;
    double time;


    inline static long txn_count;
    
    Transaction();

    Transaction(int sender, int receiver, double amount, double time);

    long getTxnID();

};