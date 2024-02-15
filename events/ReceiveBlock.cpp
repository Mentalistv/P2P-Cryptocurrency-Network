#include "../headers/Event.h"
#include "../headers/Globals.h"



ReceiveBlock::ReceiveBlock(
    double time, event_type type, int receiver_id, int sender_id, Block incoming_block
) : Event(time, type), receiver_id(receiver_id), incoming_block(incoming_block), sender_id(sender_id) {}

ReceiveBlock::~ReceiveBlock() {}

// TODO: Optkize if possible, maintain the blances for longest chain, update it if the longest chain changes 
bool ReceiveBlock::verifyBlock(vector<double> &balances) const {
    Node *receiver = nodes[receiver_id];

    int prev_block_id = incoming_block.prev_block_id;
    // Verify transactions in the block

    for (Transaction txn : incoming_block.transactions) {
        if (txn.sender != COINBASE_TXN_SENDER_ID)
            balances[txn.sender] -= txn.amount;
        
        balances[txn.receiver] += txn.amount;
    }

    // if any of the balances is negative, its invalid
    for (double balance: balances) {
        // consider the initial balances
        if (balance < 0){
            return false;
        }
    }

    return true;
}


void ReceiveBlock::updateTransactionPool() const {
    Node *receiver = nodes[receiver_id];

    for (Transaction txn: incoming_block.transactions) {
        if (receiver->transaction_pool.find(txn.txn_ID) != receiver->transaction_pool.end())
            receiver->transaction_pool.erase(txn.txn_ID);
    }
}

void ReceiveBlock::processEvent() const {
    // cout << "Inside ReceiveBlock event at time " << time << " for receiver " << receiver_id << endl;
    receiveBlock();
}

void ReceiveBlock::printEvent() const {
    cout << "Event ReceiveBlock at time = " << time << endl; 
}

void ReceiveBlock::receiveBlock() const {
    Node *receiver = nodes[receiver_id];

    // printf("\n------------- Node %d receiving block %d from Node %d at time %f ----------------------\n", receiver->id, incoming_block.id, sender_id, time);
    // if the block is already present, then return
    if (receiver->blocks.find(incoming_block.id) != receiver->blocks.end()) {
        // printf("Block already exists in the node\n");          
        return;
    }

    int prev_block_id = incoming_block.prev_block_id;

    vector<double> balances;
    if (receiver->deepest_block_id == incoming_block.prev_block_id) {
        balances = receiver->balances;
    } else {
        balances = receiver->calculateBalancesFromBlock(prev_block_id);
    }


    // verifyBlock also updates the balances
    if (!verifyBlock(balances)) {
        return;
    }
    
    // Add the block to the tree
    int prev_height = receiver->blocks[prev_block_id].height;
    int new_height = prev_height + 1;

    receiver->blocks.insert({incoming_block.id, incoming_block});
    receiver->blocks[incoming_block.id].height = new_height;

    if (receiver->id == 0)
        cout << "Node 0 just received block"  << incoming_block.id << endl;

    // Set deepest block to the incoming block if it has greatest height
    if (receiver->blocks[receiver->deepest_block_id].height < new_height) {
        receiver->deepest_block_id = incoming_block.id;
        receiver->balance = balances[receiver_id];
        receiver->balances = balances;

        // cout << "updating the balances for node " << receiver->id << endl;
        
        double delay = getPoWDelay(receiver->hashing_power);
        event_queue.push(new MineBlock(time + delay, MINE_BLOCK, receiver_id, incoming_block.id));

    } else {
        printf("not updating for node %d while receiving block %d and dbi = %d dbi height = %d\n", receiver->id, incoming_block.id, receiver->deepest_block_id, receiver->blocks[receiver->deepest_block_id].height);
        // cout << "not updating for node  " << receiver->id << " dbi = " << receiver->blocks[receiver->deepest_block_id].height << " height " << new_height << endl;
    }

    // printf("Re-transmitted to ...\n");
    // Add receive block events to transmit the received block
    for (int neighbour_id: receiver->links) {
        if (neighbour_id != sender_id) {
            Node *neighbour = nodes.at(neighbour_id);
            int message_size_bytes = incoming_block.getMessageSizeBytes();
            double latency = receiver->calculateLatencyToNode(neighbour, message_size_bytes); 
            
            event_queue.push(new ReceiveBlock(time + latency, RECEIVE_BLOCK, neighbour_id, receiver_id, incoming_block));


            // printf("Node %d with delay %f\n", neighbour_id, latency);
        }       
    }

    // printf("\n");

}