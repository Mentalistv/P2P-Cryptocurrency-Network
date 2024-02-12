#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <queue>

using namespace std;

long txn_count = 0;


////////////////////////////////////// EventConstants.h //////////////////////////////////////////////////////////////////

enum event_type{
    GENERATE_TRANSACTION,
    RECEIVE_TRANSACTION,
    MINE_BLOCK,
    RECEIVE_BLOCK
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////// NodeConstants.h //////////////////////////////////////////////////////////////////

enum NodeType{
    NODE_FAST,
    NODE_SLOW
};

enum NodeCPUType {
    NODE_CPU_FAST,
    NODE_CPU_SLOW
};

const int GENESIS_BLOCK_ID = -1;
const int NO_PREVIOUS_BLOCK = -2;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////// SimulationParameters.h //////////////////////////////////////////////////////////////////

const int NUMBER_OF_NODES = 50;
const int MIN_PEERS_CONNECTED = 3;
const int MAX_PEERS_CONNECTED = 6;

const int RANDOM_GENERATOR_SEED = 13;

const int LIGHT_SPEED_DELAY = 20; //TODO : to be taken from uniform districution

const double TRANSACTION_INTERARRIVAL_MEAN = 5000;

const int INITIAL_BALANCE = 101;

const int TRANSACTION_SIZE_BYTES = 1024;

const int BLOCK_INTERARRIVAL_TIME = 600000;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////// Transaction.h //////////////////////////////////////////////////////////////////

class Transaction
{
private:
    long txn_ID;

public:
    int sender;
    int receiver;
    double amount;
    double time;


//    inline static long txn_count;
    
    Transaction();

    Transaction(int sender, int receiver, double amount, double time);

    long getTxnID();

};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////// Block.h //////////////////////////////////////////////////////////////////

class Block {
    public:
        Block();
        Block(int block_id, int owner_id, int prev_blk_id, double time, vector<Transaction> i_txn);
        int id{};
        int owner_id{};
        int prev_block_id{};


        double timestamp{};
        vector <Transaction> transactions;
        
        
        // for local storage at node
        int height{};
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////// Node.h //////////////////////////////////////////////////////////////////

class Node {
    public:
        Node(int id, NodeType node_type, NodeCPUType node_cpu_type);
        
        int id;
        double balance;
        NodeType node_type;
        NodeCPUType node_cpu_type;

        int nof_links;
        vector<int> links;
        unordered_map<int, Block> blocks;
        int deepest_block_id;

        // attributes useful for simulation
        double hashing_power;

        unordered_map<long, Transaction> transaction_pool; 

        vector<double> calculateBalancesFromBlock(int block_id);

        double calculateLatencyToNode(Node* node, int message_size);

        // create a new block to add over the longest chain
//        Block createNewBlock();
        
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////// Utility.h //////////////////////////////////////////////////////////////////

int uniformDistributionInt(int min, int max);

double uniformDistributionDouble(double min, double max);

double exponentialDistribution(double lambda);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////// Event.h //////////////////////////////////////////////////////////////////

class Event{
    public:
    Event();
    Event(vector<Node*> nodes, double time, event_type type) : nodes(nodes), time(time), type(type) {}

    vector<Node*> nodes;
    event_type type;
    priority_queue<Event> event_queue;

    double time;
};


class EventComparator {
public:
    bool operator()(Event& a, Event& b)
    {
        if (a.time < b.time) {
            return true;
        }
        return false;
    }
};

class MineBlock : public Event {
    public:
    int miner_id;
    int prev_block_id;

    MineBlock(vector<Node*> &nodes, double time, event_type type, int miner_id, int mine_on_block_id) : Event(nodes, time, type), miner_id(miner_id), prev_block_id(prev_block_id){}

    void processEvent();

};


class ReceiveBlock : public Event {
    public:
    int receiver_id;
    Block incoming_block;
    int sender_id;

    ReceiveBlock();
    ReceiveBlock(vector<Node*> &nodes, double time, event_type type, int receiver_id, int sender_id, Block incoming_block) : Event(nodes, time, type), receiver_id(receiver_id), incoming_block(incoming_block) {}

    void processEvent();
    

    private:
        bool verifyBlock();
        void updateTransactionPool();
        void receiveBlock();

};


class ReceiveTransaction : public Event
{
public:
    Node* receiver;
    Node* sender;
    Transaction txn;

    ReceiveTransaction(Node* receiver, Node* sender, Transaction txn, vector<Node*> &nodes, event_type type, double time) : Event(nodes, time, type), receiver(receiver), sender(sender), txn(txn) {}
    
    //
    void processEvent();
    

    // ----------------------------- This funtiom receives transation and updates Node's local data --------------------------
    int receive();

    // ------------------------------------------------------------------------------------------------
    // This function will check all the neighbours of the transmitter,(exclude the sender)
    // compute latency delay for each,
    // and push "ReceiveTransaction" event in the event queue 
    // ------------------------------------------------------------------------------------------------
    void transmit();

};


class GenerateTransaction : public Event
{

public:
    Node *generator;

    GenerateTransaction(Node *generator, vector<Node*> &nodes, event_type type, double time) : Event (nodes, time, type), generator(generator) {}

    //
    void processEvent();
    
    // ------------------------------------------------------------------------------------------------ 
    //this function creates transaction object and add it to generator node's transaction pool 
    //-------------------------------------------------------------------------------------------------
    Transaction generate();


    // ------------------------------------------------------------------------------------------------
    // This function will check all the neighbours of the transmitter,
    // compute latency delay for each 
    // and push "ReceiveTransaction" event in the event queue 
    // ------------------------------------------------------------------------------------------------
    void transmit(Transaction txn);

    // -------------------- Generate next transaction after some delay ---------------------------------- 
    void initializeNextTransaction();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////// Comparators.h //////////////////////////////////////////////////////////////////

class CompareEvents {
public:
    bool operator()(Event& a, Event& b)
    {
        if (a.time < b.time) {
            return true;
        }
        return false;
    }
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////// Network.h //////////////////////////////////////////////////////////////////

void generateNetwork(vector<Node*> &nodes);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   INITIALIZATION     /////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vector<Node*> nodes;
priority_queue<Event, vector<Event>, EventComparator> event_queue;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////// node.cpp //////////////////////////////////////////////////////////////////

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

//Block Node::createNewBlock() {
//    
//}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////// RandomGenerator.cpp //////////////////////////////////////////////////////////////////

int uniformDistributionInt(int min, int max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distribution(min, max);
    int random_int = distribution(gen);
    return random_int;
}

double uniformDistributionDouble(double min, double max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> distribution(min, max);
    double random_double = distribution(gen);
    return random_double;
}

double exponentialDistribution(double lambda){
    random_device rd;
    mt19937 gen(rd());
    exponential_distribution<double> expt_dist(lambda);
    double random = expt_dist(gen);
    return random;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////// Comparators.cpp //////////////////////////////////////////////////////////////////

//bool EventComparator::operator()(Event& a, Event& b) {
//    if (a.time < b.time) {
//        return true;
//    }
//    return false;
//}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////// GenerateNetwork.cpp //////////////////////////////////////////////////////////////////

void dfsCheck(vector<vector<bool>>& matrix, vector<bool>& visited, int v=0){
    visited[v] = true;

    for(int i=0; i<matrix[v].size(); i++){
        if(matrix[v][i] && !visited[i]){
            dfsCheck(matrix, visited, i);
        }
    }
}

bool checkConnected(vector<vector<bool>> matrix){
    int n = matrix.size();

    vector<bool> visited;
    visited.resize(n);

    dfsCheck(matrix, visited);

    for(auto x: visited){
        if(!x)  return false;
    }

    return true;
}

void reinitialize(vector<vector<bool>>& matrix, vector<int>& degrees) {
    for (int i = 0; i < matrix.size(); i++) {
        fill(matrix[i].begin(), matrix[i].end(), false);
    }
    fill(degrees.begin(), degrees.end(), 0);
}


void showGraphDegrees(vector<int> degrees) {
    for (int deg: degrees)
        cout << deg << " ";

    cout << endl;
}

void initilializeNodeLinks(vector<Node*> &nodes, vector<vector<bool>> &matrix) {
    for (int i = 0; i < matrix.size(); i++) {
        vector<int> links;
        for (int j = 0; j < matrix[0].size(); j++) {
            if (matrix[i][j])
                links.push_back(j);
        }
        nodes[i]->links = links;
    }
}

void generateNetwork(vector<Node*> &nodes) {
    int n = NUMBER_OF_NODES;
    int maxp = MAX_PEERS_CONNECTED;
    int minp = MIN_PEERS_CONNECTED;

    vector<int> degrees(n, 0);
    vector<vector<bool>> matrix(n, vector<bool>(n, false));

    do {
        srand(time(nullptr));
        reinitialize(matrix, degrees);

        for (int i = 0; i < n; i++) {
            if (degrees[i] == maxp)
                continue;

            int more_needed =  uniformDistributionInt(minp, maxp) - degrees[i];

            while (more_needed > 0) {
                int adj_node = uniformDistributionInt(0, n - 1);
                if (i == adj_node || matrix[i][adj_node] || degrees[adj_node] == maxp)
                    continue;
                
                matrix[i][adj_node] = 1;
                matrix[adj_node][i] = 1;

                degrees[i]++;
                degrees[adj_node]++;

                more_needed--;
            }
        }


    } while (!checkConnected(matrix));

    showGraphDegrees(degrees);
    initilializeNodeLinks(nodes, matrix);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////// Block.cpp //////////////////////////////////////////////////////////////////

Block::Block(int block_id, int owner_id, int prev_blk_id, double time, vector<Transaction> i_txn) {
    this->id = block_id;
    this->owner_id = owner_id;
    this->prev_block_id = prev_blk_id;
    this->timestamp = time;
    this->transactions = i_txn;
}

Block::Block() {
    // default constructor
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////// Transaction.cpp //////////////////////////////////////////////////////////////////

Transaction::Transaction() {
    // default constructor
}

Transaction::Transaction(int sender, int receiver, double amount, double time){
    this->sender = sender;
    this->receiver = receiver;
    this->amount = amount;
    this->time = time;

    this->txn_ID = txn_count++;
}

long Transaction::getTxnID(){
    return txn_ID;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////// Event.cpp //////////////////////////////////////////////////////////////////

Event::Event() {
    // default constructor
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////// GenerateTransaction.cpp //////////////////////////////////////////////////////////////////

void GenerateTransaction::processEvent(){
    Transaction txn = generate();
    transmit(txn);
    initializeNextTransaction();
}

// ------------------------------------------------------------------------------------------------
// this function creates transaction objectand add it to generator node's transaction pool
//-------------------------------------------------------------------------------------------------
Transaction GenerateTransaction::generate()
{

    double amount;
    int receiver_ID;

    // Taking a random amount from range (1 to user's balance)
    double lower_bound_amount = 1;
    double upper_bound_amount = generator->balance;
    amount = uniformDistributionDouble(lower_bound_amount, upper_bound_amount);

    // Taking a random node id from range (1 to #nodes - except the generator's id)
    int lower_bound_id = 0;
    int upper_bound_id = NUMBER_OF_NODES;
    receiver_ID = generator->id;
    while (receiver_ID == generator->id)
    { 
        // ensure randomly generated "id" is not equal to the generator's id
        receiver_ID = uniformDistributionInt(lower_bound_id, upper_bound_id);
    }

    // create object of Transaction
    Transaction txn(generator->id, receiver_ID, amount, time);

    // Update transaction pool and balance
    generator->transaction_pool[txn.getTxnID()] = txn;
    
    return txn;
}

// ------------------------------------------------------------------------------------------------
// This function will check all the neighbours of the transmitter,
// compute latency delay for each
// and push "ReceiveTransaction" event in the event queue
// ------------------------------------------------------------------------------------------------
void GenerateTransaction::transmit(Transaction txn)
{
    int nof_links = generator->nof_links;
    for (int i = 0; i < nof_links; i++)
    {
        // -------------------------- get the neighbour node ----------------------------
        int neighbour_id = generator->links.at(i);
        Node *neighbour = nodes.at(neighbour_id);

        // // -------------------------- Calculate latency ---------------------------------
        double latency = generator->calculateLatencyToNode(neighbour, TRANSACTION_SIZE_BYTES);
        
        
        ReceiveTransaction rb(neighbour, generator, txn, nodes, type, time + latency);

        event_queue.push(rb);
        
    }
}

// -------------------- Generate next transaction after some delay ----------------------------------
void GenerateTransaction::initializeNextTransaction()
{

    // after a delay this node again generates a new transaction
    double delay;
    delay = exponentialDistribution(1.0 / TRANSACTION_INTERARRIVAL_MEAN);
    
    GenerateTransaction gt(generator, nodes, type, time + delay);
    event_queue.push(gt);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////// MineBlock.cpp //////////////////////////////////////////////////////////////////

// void MineBlock::processEvent() {
//     // Node *miner = &nodes[miner_id];
//     int new_prev_block_id;
    
//     int curr_deepest_block_id = nodes[miner_id]->deepest_block_id;

//     // checking if its the part of the longest chain
//     if(curr_deepest_block_id != prev_block_id){
//         int curr_deepest_block_height = nodes[miner_id]->blocks[curr_deepest_block_id].height;

//         int temp = prev_block_id;
//         int given_chain_height = 0;

//         while(nodes[miner_id]->blocks[temp].id != -1){
//             given_chain_height++;
//             temp = nodes[miner_id]->blocks[temp].id;
//         }

//         if(given_chain_height < curr_deepest_block_height)  return;
//         new_prev_block_id = prev_block_id;
//     }
//     else{
//         new_prev_block_id = curr_deepest_block_id;
//     }

//     // MOD

//     auto itr = nodes[miner_id]->txn_pool.begin();
//     int count = 1023;
//     vector <Transaction> txn_to_be_included;

//     while(count-- && itr != txn_pool.end()){
//         txn_to_be_included.push_back(*itr);
//         itr++;
//     }

//     for(auto x: txn_to_be_included){
//         nodes[miner_id]->txn_pool.erase(x.TXNID);
//     }

//     Block* new_block = new Block("id", miner_id, new_prev_block_id, time, txn_to_be_included);

// }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////// ReceiveBlock.cpp //////////////////////////////////////////////////////////////////

// bool ReceiveBlock::verifyBlock() {
//     Node *receiver = nodes[receiver_id];
//     int prev_block_id = incoming_block.prev_block_id;
//     // Verify transactions in the block
//     vector<double> balances = receiver->calculateBalancesFromBlock(prev_block_id);

//     for (Transaction txn : incoming_block.transactions) {
//         balances[txn.sender] -= txn.amount;
//         balances[txn.receiver] += txn.amount;
//     }

//     // TODO : consider the initial balances

//     // if any of the balances is negative, its invalid
//     for (double balance: balances) {
//         if (balance < 0)
//             return false;
//     }

//     return true;
// }


// void ReceiveBlock::updateTransactionPool() {
//     Node *receiver = nodes[receiver_id];

//     for (Transaction txn: incoming_block.transactions) {
//         if (receiver->transaction_pool.find(txn.getTxnID()) != receiver->transaction_pool.end())
//             receiver->transaction_pool.erase(txn.getTxnID());
//     }
// }

// void ReceiveBlock::processEvent() {
//     receiveBlock();
// }

// void ReceiveBlock::receiveBlock() {
//     Node *receiver = nodes[receiver_id];

//     // if the block is already present, then return
//     if (receiver->blocks.find(incoming_block.id) != receiver->blocks.end())
//         return;

//     int prev_block_id = incoming_block.prev_block_id;

//     if (!verifyBlock())
//         return;
    
//     // Add the block to the tree
//     int prev_height = receiver->blocks[prev_block_id].height;
//     int new_height = prev_height + 1;
//     receiver->blocks[incoming_block.id] = incoming_block;
//     receiver->blocks[incoming_block.id].height = prev_height + 1;


//     // Set deepest block to the incoming block if it has greatest height
//     if (receiver->blocks[receiver->deepest_block_id].height < new_height) {
//         receiver->deepest_block_id = incoming_block.id;

//         // // remove the txns in the block from the receiver's TXN pool
//         // updateTransactionPool();

        
//         double delay = 13; // TODO: Mine block delay

//         MineBlock mb(nodes, time + delay, MINE_BLOCK, receiver_id, incoming_block.id);
//         eventQueue.push(mb);
//     }

//     // Add receive block events to transmit the received block
//     for (int neighbour_id: receiver->links) {
//         if (neighbour_id != sender_id) {
//             double latency = 13; //TODO
//             ReceiveBlock newRcvBlkEvent(nodes, time + latency, RECEIVE_BLOCK, neighbour_id, receiver_id, incoming_block);
//             eventQueue.push(newRcvBlkEvent);
//         }       
//     }

//     // Add mine block events
// }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////// ReceiveTransaction.cpp //////////////////////////////////////////////////////////////////

void ReceiveTransaction::processEvent(){

    int status = receive();
    if(status){
       return; // transaction already exists or some other error 
    }

    transmit();

}

// ----------------------------- This funtiom receives transation and updates Node's local data --------------------------
int ReceiveTransaction::receive(){
    
    // add transaction to the transaction pool if it does not already exists
    if(receiver->transaction_pool.count(txn.getTxnID())){
        return 1;
    }
    else{
        receiver->transaction_pool[txn.getTxnID()] = txn;
        return 0;
    }

}

// ------------------------------------------------------------------------------------------------
// This function will check all the neighbours of the transmitter,(exclude the sender)
// compute latency delay for each,
// and push "ReceiveTransaction" event in the event queue 
// ------------------------------------------------------------------------------------------------
void ReceiveTransaction::transmit(){
    int nof_links = receiver->nof_links;
    for (int i = 0; i < nof_links; i++)
    {

        // -------------------------- get the neighbour node ----------------------------
        int neighbour_id = receiver->links.at(i);
        if (neighbour_id = sender->id) // not transmit to sender again
        {
            continue;
        }
        Node *neighbour = nodes.at(neighbour_id);

        
        // -------------------------- Calculate latency ---------------------------------
        // 1. Calculate capacity (in Mbps)
        double capacity;
        if (receiver->node_type == NODE_SLOW || neighbour->node_type == NODE_SLOW)
        {
            capacity = 5;
        }
        else
        {
            capacity = 100;
        }

        // 2. Size of transaction to be transmitted (in bits)
        int size_of_txn = 1024*8; // 1KB

        // 3. Generate queuing delay from exponential distribution (in ms)
        double queue_delay;
        double mean = 96 / capacity;
        queue_delay = exponentialDistribution(1 / mean);
        
        // 4. Compute latency
        double latency = size_of_txn / (capacity * 1000) + queue_delay + LIGHT_SPEED_DELAY; 

        
        
        ReceiveTransaction rb(neighbour, receiver, txn, nodes, type, time);

        event_queue.push(rb);
    }
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char const *argv[]) {        
    int node_id = 0;

    if (argc != 3) {
        cout << "Usage: ./simulation <slow_nodes_%> <low_cpu_%>";
        return 1;
    }

    int slowp = atoi(argv[1]);
    int lowCPUp = atoi(argv[2]);
    int slow = NUMBER_OF_NODES * slowp/100;
    int lowCPU = NUMBER_OF_NODES * lowCPUp/100;

    for (int i = 0; i < NUMBER_OF_NODES; i++) {
        // TODO: Make this distribution random
        Node newnode(node_id++, slow-- > 0 ? NODE_SLOW : NODE_FAST, lowCPU-- > 0 ? NODE_CPU_SLOW : NODE_CPU_FAST);
        nodes.push_back(&newnode);
    }

    generateNetwork(nodes);


    // TODO: initilize fast slow nodes and fast slow CPUs

    // TODO: Add initilial events


    return 0;
}
