#include<string>
#include<vector>
#include "../constants/NodeConstants.h"


#ifndef NODE_H
#define NODE_H
#endif

using namespace std;

class Node {
    public:
        Node(string node_id, NodeType nt, NodeCPUType nct);
        
        string id;
        NodeType node_type;
        NodeCPUType node_cpu_type;

        // attributes useful for simulation
        double hashing_power;
};