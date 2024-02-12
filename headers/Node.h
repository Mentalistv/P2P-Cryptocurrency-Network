#include<string>
#include<vector>
#include <unordered_map> 

#include "../constants/NodeConstants.h"
#include "../headers/Block.h"

#ifndef NODE_H
#define NODE_H
#endif

using namespace std;

class Node {
    public:
        Node();
        Node(int id, NodeType node_type, NodeCPUType node_cpu_type);
        
        int id;
        double balance;
        NodeType node_type;
        NodeCPUType node_cpu_type;

        vector<int> links;
        unordered_map<int, Block> blocks;
        int deepest_block_id;

        // attributes useful for simulation
        double hashing_power;

        unordered_map<long, Transaction> transaction_pool; 

        vector<double> calculateBalancesFromBlock(int block_id);

        double calculateLatencyToNode(Node* node, int message_size);

        // create a new block to add over the longest chain
        Block createNewBlock();
        
        static vector<Node*> nodes;

        int getTransactionPoolSize();
};