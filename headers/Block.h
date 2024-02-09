#include<string>
#include<vector>
#include<unordered_map>

#include "Node.h"
#include "Transaction.h"


#ifndef BLOCK_H
#define BLOCK_H
#endif

using namespace std;

class Block {
    public:
        Block(string block_id, string ownr, string prev_blk_id, double time, vector <Transaction> i_txn, long h);
        
        string id;
        string owner;

        string prev_block_id;
        double timestamp;
        vector <Transaction> included_txn;
        long height;
};