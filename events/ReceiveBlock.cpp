#include "../headers/Event.h"
#include "../headers/Globals.h"



ReceiveBlock::ReceiveBlock(
    double time, event_type type, int receiver_id, int sender_id, Block incoming_block
    ) : Event(time, type), receiver_id(receiver_id), incoming_block(incoming_block), sender_id(sender_id) {}

ReceiveBlock::~ReceiveBlock() {}

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

void ReceiveBlock::selfishMinerReceives() const {
    Node *receiver = nodes[receiver_id];

    cout << "Lead of selfish Miner " << receiver->id << " " << receiver->lead << endl;

    if (receiver->lead == 0) {
        receiver->deepest_block_id = incoming_block.id;
        receiver->state_zero_dash = 0;
        

        double delay = getPoWDelay(receiver->hashing_power);
        Block new_block = receiver->createNewBlock(time);
        printf("\nSelfish miner %d mine event pushed because of lead 0 queue size = %d\n", receiver->id, receiver->private_chain.size());
        event_queue.push(new MineBlock(time + delay, MINE_BLOCK, receiver_id, new_block));
    }
    else if (receiver->lead == 1)
    {
        receiver->lead = 0;
        receiver->state_zero_dash = 1;        
        Block secret_block = receiver->private_chain.front();
        receiver->private_chain.pop();
        
        // Add receive block events to transmit the received block
        transmitBlock(secret_block);

        // MOD
        receiver->deepest_block_id = secret_block.id;
        double delay = getPoWDelay(receiver->hashing_power);
        Block new_block = receiver->createNewBlock(time);
        printf("\nSelfish miner %d mine event pushed because of lead 0 queue size = %d\n", receiver->id, receiver->private_chain.size());
        event_queue.push(new MineBlock(time + delay, MINE_BLOCK, receiver_id, new_block));

    } else if (receiver->lead == 2) {
        receiver->lead = 0;
        while (!receiver->private_chain.empty())
        {
            Block secret_block = receiver->private_chain.front();
            receiver->private_chain.pop();

            // Add receive block events to transmit the received block
            transmitBlock(secret_block);
            receiver->deepest_block_id = secret_block.id;
        }

        // MOD
        double delay = getPoWDelay(receiver->hashing_power);
        Block new_block = receiver->createNewBlock(time);
        printf("\nSelfish miner %d mine event pushed because of lead 0 queue size = %d\n", receiver->id, receiver->private_chain.size());
        event_queue.push(new MineBlock(time + delay, MINE_BLOCK, receiver_id, new_block));
    }
    else{
        cout << "Decreasing lead  private chain size " << receiver->private_chain.size() << endl;
        receiver->lead--;
        Block secret_block = receiver->private_chain.front();
        receiver->private_chain.pop();
        transmitBlock(secret_block);
    }
}

void ReceiveBlock::receiveBlock() const
{
    Node *receiver = nodes[receiver_id];

    // if block already received, do not process it again
    if (receiver->blocks.find(incoming_block.id) != receiver->blocks.end()) {
        return;
    }

    int prev_block_id = incoming_block.prev_block_id;

    vector<double> balances;
    if (receiver->deepest_block_id == incoming_block.prev_block_id) {
        balances = receiver->balances;
    } else {
        if (receiver->blocks.find(incoming_block.prev_block_id) == receiver->blocks.end()) {
            receiver->wait_queue.insert({incoming_block.prev_block_id, {incoming_block, time}});
            return;
        }
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
    receiver->blocks[incoming_block.id].arrival_time = time;

    checkChildrenInWaitQueue(incoming_block);


    if (receiver->node_character_type == HONEST && incoming_block.owner_id == 8 || incoming_block.owner_id == 9) {
        printf("dbi = %d prev id = %d block from = %d dbi height = %d inc height = %d\n", receiver->deepest_block_id, incoming_block.prev_block_id, incoming_block.owner_id, receiver->blocks[receiver->deepest_block_id].height, new_height);    
    }


    // Set deepest block to the incoming block if it has greatest height
    if (receiver->blocks[receiver->deepest_block_id].height < new_height) {
        receiver->deepest_block_id = incoming_block.id;
        receiver->balance = balances[receiver_id];
        receiver->balances = balances;

        if (receiver->node_character_type == SELFISH) {
            selfishMinerReceives();
            return;
        }

        double delay = getPoWDelay(receiver->hashing_power);
        Block new_block = receiver->createNewBlock(time);
        event_queue.push(new MineBlock(time + delay, MINE_BLOCK, receiver_id, new_block));
    
    } else {
        // this means there is a fork
    }

    // Add receive block events to transmit the received block
    for (int neighbour_id: receiver->links) {
        if (neighbour_id != sender_id) {
            Node *neighbour = nodes.at(neighbour_id);
            int message_size_bytes = incoming_block.getMessageSizeBytes();
            double latency = receiver->calculateLatencyToNode(neighbour, message_size_bytes);

            event_queue.push(new ReceiveBlock(time + latency, RECEIVE_BLOCK, neighbour_id, receiver_id, incoming_block));
        }
    }
}

void ReceiveBlock::checkChildrenInWaitQueue(Block incoming_block) const {
    Node *receiver = nodes[receiver_id];

    if (receiver->wait_queue.find(incoming_block.id) != receiver->wait_queue.end()) {
        Block child = receiver->wait_queue[incoming_block.id].first;
        double rcv_time = receiver->wait_queue[incoming_block.id].second;

        int new_height = incoming_block.height + 1;

        receiver->blocks.insert({child.id, child});
        receiver->blocks[child.id].height = new_height;
        receiver->blocks[child.id].arrival_time = rcv_time;
        receiver->wait_queue.erase(child.id);

        cout << "Child reached first" << endl;

        if (new_height > receiver->blocks[receiver->deepest_block_id].height) {
            receiver->deepest_block_id = child.id;
            vector<double> balances = receiver->calculateBalancesFromBlock(child.id);
            receiver->balances = balances;
            receiver->balance = balances[child.id];

            double delay = getPoWDelay(receiver->hashing_power);
            Block new_block = receiver->createNewBlock(time);
            event_queue.push(new MineBlock(time + delay, MINE_BLOCK, receiver_id, new_block));
        }
    }
}

void ReceiveBlock::transmitBlock(Block block) const {
    Node *receiver = nodes[receiver_id];
    // Add receive block events to transmit the received block
    for (int neighbour_id : receiver->links) {
        if (neighbour_id != sender_id) {
            Node *neighbour = nodes.at(neighbour_id);
            int message_size_bytes = block.getMessageSizeBytes();
            double latency = receiver->calculateLatencyToNode(neighbour, message_size_bytes);

            event_queue.push(new ReceiveBlock(time + latency, RECEIVE_BLOCK, neighbour_id, receiver_id, block));
        }
    }
}