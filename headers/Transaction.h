#include <iostream>

using namespace std;

class Transaction
{
public:
    long txn_ID;
    int sender;
    int receiver;
    double amount;
    double time;


    inline static long txn_count;
    
    Transaction();

    Transaction(int sender, int receiver, double amount, double time);
};