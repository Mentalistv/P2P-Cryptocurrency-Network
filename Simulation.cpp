#include<iostream>
#include<vector>
#include <string>
#include <queue>

#include "constants/NodeConstants.h"
#include "constants/SimulationParameters.h"
#include "headers/Network.h"
#include "headers/Globals.h"

using namespace std;

vector<Node*> nodes;
priority_queue<Event*, std::vector<Event*>, EventComparator> event_queue;

int main(int argc, char const *argv[]) {    
    Transaction::txn_count = 0;
    
    int node_id = 0;

    if (argc != 3) {
        cout << "Usage: ./simulation <slow_nodes_%> <low_cpu_%>";
        return 1;
    }

    int slowp = atoi(argv[1]);
    int lowCPUp = atoi(argv[2]);
    int slow = NUMBER_OF_NODES * slowp/100;
    int lowCPU = NUMBER_OF_NODES * lowCPUp/100;

    vector<Node*> allnodes;

    for (int i = 0; i < NUMBER_OF_NODES; i++) {
        // TODO: Make this distribution random
        allnodes.push_back(new Node(node_id++, slow-- > 0 ? NODE_SLOW : NODE_FAST, lowCPU-- > 0 ? NODE_CPU_SLOW : NODE_CPU_FAST));
    }

    nodes = allnodes;

    generateNetwork(nodes);

    // TODO: initilize fast slow nodes and fast slow CPUs

    // TODO: Add initilial events
    for (Node* node: nodes) {
        event_queue.push(new GenerateTransaction(node, GENERATE_TRANSACTION, 0.0));
    }

    while (!event_queue.empty()) {

        Event* event = event_queue.top();
        event->processEvent();
        cout << "Size of event queue = " << event_queue.size() <<  endl;
        event_queue.pop();
    }

    return 0;
}
