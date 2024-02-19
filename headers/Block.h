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


        double timestamp{};
        vector <Transaction> transactions;
        
        
        // for local storage at node
        int height = 0;

        int getMessageSizeBytes() const;
};