#include<string>
#include<vector>
#include <unordered_map> 
#include <queue>

#include "../constants/NodeConstants.h"
#include "Block.h"

#ifndef NODE_H
#define NODE_H
#endif

using namespace std;

class Node {
    public:
        Node();
        Node(int id, NodeType node_type, NodeCPUType node_cpu_type, NodeCharacterType nc);
        
        int id;
        double balance;
        vector<double> balances; // balances as per the current longest chain
        NodeType node_type;
        NodeCPUType node_cpu_type;
        NodeCharacterType node_character_type;

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

        void updateBalances(Block block);

        // This creates a new block mined on the deepest block
        Block createNewBlock(double time);


        vector<int> wait_queue;


        // Fields specific to Selfish Miner
        queue<Block> private_chain;
        int lead = 0;
        int state_zero_dash = 0;
        void releasePrivateChain(double time);
        
};