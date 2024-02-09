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
        Block(string block_id, Node ownr, unordered_map <string, Block> blkchn, string prev_blk_id, double t, Transaction cb_txn, vector <Transaction> i_txn);
        
        string id;
        Node owner;

        unordered_map <string, Block> blockchain;

        string prev_block_id;
        double time;
        Transaction coinbase_txn;
        vector <Transaction> included_txn;
        
};