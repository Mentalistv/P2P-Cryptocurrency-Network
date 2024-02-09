#include<iostream>
#include<vector>
#include <string>

#include "constants/SimulationParamenters.h"
#include "constants/NodeConstants.h"
#include "headers/Network.h"

using namespace std;

vector<Node> nodes;

int main(int argc, char const *argv[]) {    
    int node_id = 0;
    for (int i = 0; i < NUMBER_OF_NODES; i++) {
        Node newnode(node_id++, NODE_FAST, NODE_CPU_FAST);
        nodes.push_back(newnode);
    }

    generateNetwork(nodes);


    // TODO: initilize fast slow nodes and fast slow CPUs

    // TODO: Add initilial events

    return 0;
}
