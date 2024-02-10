#include<string>
#include<vector>

#include "../headers/Block.h"
#include "../constants/NodeConstants.h"

#ifndef NODE_H
#define NODE_H
#endif

using namespace std;

class Node {
    public:
        Node(int node_id, NodeType nt, NodeCPUType nct);
        
        int id;
        NodeType node_type;
        NodeCPUType node_cpu_type;

        vector<int> links;
        unordered_map<int, Block> blocks;
        int deepest_block_id;

        // attributes useful for simulation
        double hashing_power;

};