#include<string>
#include<vector>
#include<unordered_map>

#include "Transaction.h"

#ifndef BLOCK_H
#define BLOCK_H
#endif

using namespace std;

class Block {
    public:
        Block();
        Block(int block_id, int owner_id, int prev_blk_id, double time, vector<Transaction> i_txn);
        int id{};
        int owner_id{};
        int prev_block_id{};


        double timestamp = 0.0;
        vector <Transaction> transactions;
        
        int getMessageSizeBytes() const;
        
        // for local storage at node
        int height = 0;
        double arrival_time = 0.0;

};