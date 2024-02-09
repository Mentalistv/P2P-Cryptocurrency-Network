#include<string>
#include<vector>
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

        // attributes useful for simulation
        double hashing_power;
};