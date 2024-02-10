#include <iostream>

using namespace std;

class Transaction
{
    public:
    int sender;
    int receiver;
    double amount;
    long txn_ID;
    double time;

    Transaction();
    Transaction(int sender, int receiver, double amount, long txn_ID, double time);
    string get_transaction_string_format();
};