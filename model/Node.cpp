#include "../headers/Event.h"
#include "../headers/Utility.h"
#include "../constants/SimulationParameters.h"
#include "../headers/Globals.h"


Node::Node(int id, NodeType node_type, NodeCPUType node_cpu_type) {
    this->id = id;
    this->node_type = node_type;
    this->node_cpu_type = node_cpu_type;
    
    deepest_block_id = GENESIS_BLOCK_ID;
    this->balances = vector<double>(NUMBER_OF_NODES, INITIAL_BALANCE);
    this->balance = INITIAL_BALANCE;

    blocks.insert({GENESIS_BLOCK_ID, Block(GENESIS_BLOCK_ID, id, NO_PREVIOUS_BLOCK, 0.0, {})});   
}

vector<double> Node::calculateBalancesFromBlock(int block_id) {
    int temp_id = block_id;

    vector<double> balances(NUMBER_OF_NODES, 0);

    while(blocks[temp_id].id != GENESIS_BLOCK_ID) {
        vector<Transaction> transactions = blocks[temp_id].transactions;

        for (Transaction txn: transactions) {
            if (txn.sender != COINBASE_TXN_SENDER_ID)
                balances[txn.sender] -= txn.amount;
            
            balances[txn.receiver] += txn.amount;
        }
        temp_id = blocks[temp_id].prev_block_id;
    }

    for (int i = 0; i < balances.size(); i++) {
        balances[i] += INITIAL_BALANCE;
    }

    return balances;
}

// This updates the balances when adding a block to the longest chain
void Node::updateBalances(Block block) {
    for (Transaction txn: block.transactions) {
        if (txn.sender != COINBASE_TXN_SENDER_ID)
            balances[txn.sender] -= txn.amount;

        balances[txn.receiver] += txn.amount;
    }
}

double Node::calculateLatencyToNode(Node* neighbour, int message_size_bytes) {
        // -------------------------- Calculate latency ---------------------------------
        // 1. Calculate capacity
        double capacity;
        if (this->node_type == NODE_SLOW || neighbour->node_type == NODE_SLOW) {
            capacity = 5;
        } else {
            capacity = 100;
        }


        // 2. Size of transaction to be transmitted (in bits)
        int size_of_txn = message_size_bytes*8; // 1KB

        // 3. Generate queuing delay from exponential distribution (in ms)
        double queue_delay;
        double mean = 96 / (capacity * 1000);
        queue_delay = exponentialDistribution(mean);
        
        // 4. Compute latency
        double latency = size_of_txn / (capacity * 1000000) 
            + queue_delay + propagation_delays[this->id][neighbour->id];

        return latency; 
}

int Node::getTransactionPoolSize() {
    return transaction_pool.size();  
}

Block Node::createNewBlock(double time) {
    int count = 1023;
    vector <Transaction> txns;

    // Mark the used transactions
    unordered_map<long, bool> used_txns;

    Block temp = blocks[deepest_block_id];
    while (temp.id != -1) {
        for (Transaction t: temp.transactions) {
            used_txns[t.txn_ID] = true;
        }
        temp = blocks[temp.prev_block_id];
    }

    // Add transactions except the used ones
    for (auto it = transaction_pool.begin(); it != transaction_pool.end(); ++it) {
        const pair<long, Transaction>& element = *it;
        Transaction txn = element.second;
        if (!used_txns[element.first] && txn.amount <= balances[txn.sender]) {
            txns.push_back(element.second);
            count--;
        }
        if (count == 0)
            break;
    }

    Transaction coinbase(COINBASE_TXN_SENDER_ID, id, 50.0, time);
    txns.push_back(coinbase);

    Block new_block = Block(BLOCK_ID_GENERATOR++, id, deepest_block_id, time, txns);
    int prev_height = blocks[deepest_block_id].height;
    new_block.height = prev_height + 1;
    new_block.arrival_time = time;

    return new_block;
}