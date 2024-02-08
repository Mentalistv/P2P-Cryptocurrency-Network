#include<iostream>
#include<vector>
#include <string>

#include "constants/SimulationParamenters.h"
#include "constants/NodeConstants.h"
#include "headers/Node.h"
#include "headers/Network.h"

using namespace std;


int main(int argc, char const *argv[]) {
    
    char c = 'A';
    vector<Node> nodes;
    for (int i = 0; i < NUMBER_OF_NODES; i++) {
        // TODO : Generate a random ID
        string id(1, c + i);
        Node newnode(id, NODE_FAST, NODE_CPU_FAST);

        nodes.push_back(newnode);
    }


    generateNetwork();
    

    return 0;
}
