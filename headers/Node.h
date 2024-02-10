#include<string>
#include<vector>
#include <unordered_map> 

#include "../constants/NodeConstants.h"
#include "Transaction.h"

#include "../headers/Block.h"
#include "../constants/NodeConstants.h"

#ifndef NODE_H
#define NODE_H
#endif

using namespace std;

class Node {
    public:
        Node(int id, NodeType node_type, NodeCPUType node_cpu_type);
        
        int id;
        double balance;
        NodeType node_type;
        NodeCPUType node_cpu_type;

        int nof_links;
        vector<int> links;
        unordered_map<int, Block> blocks;
        int deepest_block_id;

        // attributes useful for simulation
        double hashing_power;

        unordered_map<long, Transaction> transaction_pool; 
        
};