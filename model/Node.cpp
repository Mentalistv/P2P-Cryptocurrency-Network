#include "../headers/Node.h"
#include "../headers/Utility.h"
#include "../constants/SimulationParameters.h"


Node::Node(int id, NodeType node_type, NodeCPUType node_cpu_type) {
    this->id = id;
    this->node_type = node_type;
    this->node_cpu_type = node_cpu_type;


    blocks[GENESIS_BLOCK_ID] = Block(GENESIS_BLOCK_ID, id, NO_PREVIOUS_BLOCK, 0, {});   
}

vector<double> Node::calculateBalancesFromBlock(int block_id) {
    int temp_id = block_id;

    vector<double> balances;

    while(blocks[temp_id].prev_block_id != -1) {
        vector<Transaction> transactions = blocks[temp_id].transactions;

        for (Transaction txn: transactions) {
            balances[txn.sender] -= txn.amount;
            balances[txn.receiver] += txn.amount;
        }
    }

    return balances;
}

double Node::calculateLatencyToNode(Node* neighbour, int message_size_bytes) {
        // -------------------------- Calculate latency ---------------------------------
        // 1. Calculate capacity
        double capacity;
        if (this->node_type == NODE_SLOW || neighbour->node_type == NODE_SLOW)
        {
            capacity = 5;
        }
        else
        {
            capacity = 100;
        }

        // 2. Size of transaction to be transmitted (in bits)
        int size_of_txn = message_size_bytes*8; // 1KB

        // 3. Generate queuing delay from exponential distribution (in ms)
        double queue_delay;
        double mean = 96 / capacity;
        queue_delay = exponentialDistribution(1 / mean);
        
        // 4. Compute latency
        double latency = size_of_txn / (capacity * 1000) + queue_delay + LIGHT_SPEED_DELAY;

        return latency; 
}

Block Node::createNewBlock() {
    
}