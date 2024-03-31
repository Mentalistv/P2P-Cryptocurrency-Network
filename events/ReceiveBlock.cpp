#include "../headers/Event.h"
#include "../headers/Globals.h"
#include <algorithm>



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
    
    Node *receiver = nodes[receiver_id];

    if (receiver->node_character_type == HONEST) {
        // cout << "honest miner" << endl;
        receiveBlock();
    } else {
        // cout << "selfish miner" << endl;
        selfishMinerReceiveBlock();
    }

}

void ReceiveBlock::printEvent() const {
    cout << "Event ReceiveBlock at time = " << time << endl;
}

void ReceiveBlock::selfishMinerReceives() const {
    Node *receiver = nodes[receiver_id];

    // cout << "Lead of selfish Miner " << receiver->id << " " << receiver->lead << endl;
    // printf("Selfish Miner %d private chain size %d\n", receiver->id, receiver->private_chain.size());

    if (receiver->lead == 0) {
        receiver->state_zero_dash = 0;
        // printf ("LEAD = 0 inc height = %d dbi height = %d\n",
            // incoming_block.height, receiver->blocks[receiver->deepest_block_id].height);
        if (incoming_block.height > receiver->blocks[receiver->deepest_block_id].height) {
            receiver->deepest_block_id = incoming_block.id;

            double delay = getPoWDelay(receiver->hashing_power);
            Block new_block = receiver->createNewBlock(time);
            // printf("\nSelfish miner %d mine event pushed because of lead 0 queue size = %d\n", receiver->id, receiver->private_chain.size());
            // printf("Selfish Miner started mining on %d and dbi = %d\n", new_block.prev_block_id, receiver->deepest_block_id);
            event_queue.push(new MineBlock(time + delay, MINE_BLOCK, receiver_id, new_block));
        }
    }
    else if (receiver->lead == 1)
    {
        receiver->lead = 0;
        receiver->state_zero_dash = 1;
        // printf("Setting state 0'***************\n");       
        Block secret_block = receiver->private_chain.front();
        receiver->private_chain.pop();
        
        // printf("selfish miner %d is releasing block %d\n", receiver->id, secret_block.id);
        // Add receive block events to transmit the received block
        transmitBlock(secret_block);

        // MOD
        receiver->deepest_block_id = secret_block.id;

    } else if (receiver->lead == 2) {
        receiver->lead = 0;
        // printf("receiver %d lead = 2 queue size = %d\n", receiver->id, receiver->private_chain.size());
        // printf("LEAD = 2 *** dbi = %d height = %d first in pc = %d Height = %d\n", receiver->deepest_block_id, receiver->blocks[receiver->deepest_block_id].height, receiver->private_chain.front().id, receiver->blocks[receiver->private_chain.front().id].height);
        while (!receiver->private_chain.empty())
        {
            Block secret_block = receiver->private_chain.front();
            receiver->private_chain.pop();

            // Add receive block events to transmit the received block
            transmitBlock(secret_block);
            receiver->deepest_block_id = secret_block.id;
        }
    }
    else{
        // printf("LEAD > 2 *** dbi = %d height = %d first in pc = %d Height = %d\n", receiver->deepest_block_id, receiver->blocks[receiver->deepest_block_id].height, receiver->private_chain.front().id, receiver->blocks[receiver->private_chain.front().id].height);
        // cout << "Decreasing lead  private chain size " << receiver->private_chain.size() << endl;
    
        receiver->lead--;
        Block secret_block = receiver->private_chain.front();
        receiver->private_chain.pop();
        transmitBlock(secret_block);
        receiver->deepest_block_id = secret_block.id;
    }
}


void addBlockToReceiver(Node* receiver, Block incoming_block, double time, int height) {
    receiver->blocks.insert({incoming_block.id, incoming_block});
    receiver->blocks[incoming_block.id].height = height;
    receiver->blocks[incoming_block.id].arrival_time = time;
}

void ReceiveBlock::receiveBlock() const {
    Node *receiver = nodes[receiver_id];

    // if block already received, do not process it again
    if (receiver->blocks.find(incoming_block.id) != receiver->blocks.end()) {
        return;
    }

    // printf("Block with ID %d received at receiver %d -----\n", incoming_block.id, receiver->id);

    int prev_block_id = incoming_block.prev_block_id;

    vector<double> balances;
    if (receiver->deepest_block_id == incoming_block.prev_block_id) {
        balances = receiver->balances;
    } else {
        auto res = receiver->blocks.find(incoming_block.prev_block_id);

        if (res == receiver->blocks.end() || receiver->blocks[prev_block_id].height == -1) {
            receiver->wait_queue.push_back(incoming_block.id);
            addBlockToReceiver(receiver, incoming_block, time, -1);
            transmitBlock(incoming_block);
            // cout << "Child received first" << endl;
            return;
        }
        balances = receiver->calculateBalancesFromBlock(prev_block_id);
    }

    // verifyBlock also updates the balances
    if (!verifyBlock(balances)) {
        // printf("Rejecting Block %d at receiver %d\n", incoming_block.id, receiver->id);
        return;
    }
    
    // Add the block to the tree
    int prev_height = receiver->blocks[prev_block_id].height;
    int new_height = prev_height + 1;

    addBlockToReceiver(receiver, incoming_block, time, new_height);

    // Check if any block in wait queue now has a new parent
    checkChildrenInWaitQueue();

    // if (receiver->node_character_type == HONEST && incoming_block.owner_id == 8 || incoming_block.owner_id == 9) {
    //     printf("receiver = %d inc block id = %d dbi = %d prev id = %d block from = %d dbi height = %d inc height = %d\n", receiver->id, incoming_block.id, receiver->deepest_block_id, incoming_block.prev_block_id, incoming_block.owner_id, receiver->blocks[receiver->deepest_block_id].height, new_height);    
    // }

    // Set deepest block to the incoming block if it has greatest height
    if (receiver->blocks[receiver->deepest_block_id].height < new_height) {
        receiver->deepest_block_id = incoming_block.id;
        receiver->balance = balances[receiver_id];
        receiver->balances = balances;
        
        double delay = getPoWDelay(receiver->hashing_power);
        Block new_block = receiver->createNewBlock(time);
        event_queue.push(new MineBlock(time + delay, MINE_BLOCK, receiver_id, new_block));

    } else {
        // this means there is a fork
    }

    // Add receive block events to transmit the received block
    // for (int neighbour_id: receiver->links) {
    //     if (neighbour_id != sender_id) {
    //         Node *neighbour = nodes.at(neighbour_id);
    //         int message_size_bytes = incoming_block.getMessageSizeBytes();
    //         double latency = receiver->calculateLatencyToNode(neighbour, message_size_bytes); 

    //         event_queue.push(new ReceiveBlock(time + latency, RECEIVE_BLOCK, neighbour_id, receiver_id, incoming_block));
    //     }       
    // }

    transmitBlock(incoming_block);

}

void ReceiveBlock::selfishMinerReceiveBlock() const {
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
        auto res = receiver->blocks.find(incoming_block.prev_block_id);

        if (res == receiver->blocks.end() || receiver->blocks[prev_block_id].height == -1) {
            receiver->wait_queue.push_back(incoming_block.id);
            addBlockToReceiver(receiver, incoming_block, time, -1);

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

    addBlockToReceiver(receiver, incoming_block, time, new_height);

    // Check if any block in wait queue now has a new parent
    checkChildrenInWaitQueue();


    // Set deepest block to the incoming block if it has greatest height
    if (receiver->blocks[receiver->deepest_block_id].height < new_height) {
        receiver->deepest_block_id = incoming_block.id;
        receiver->balance = balances[receiver_id];
        receiver->balances = balances;

        // SELFISH MINER CODE
        selfishMinerReceives();

    } else {
        // this means there is a fork
    }
}

void ReceiveBlock::checkChildrenInWaitQueue() const {
    Node *receiver = nodes[receiver_id];
    int deepest_height = receiver->blocks[receiver->deepest_block_id].height;
    int new_deepest = -1;
    vector<int> to_be_deleted;

    for (int id: receiver->wait_queue) {
        Block waiting_block = receiver->blocks[id];
        int height = setBlockHeights(waiting_block.id);

        if (height > deepest_height) {
            new_deepest = waiting_block.id;
        }

        if (height != -1) {
            to_be_deleted.push_back(waiting_block.id);
            receiver->blocks[id].height = height;
        }
    }

    if (new_deepest != -1) {
        receiver->deepest_block_id = new_deepest;
        vector<double> balances = receiver->calculateBalancesFromBlock(new_deepest);
        receiver->balances = balances;
        receiver->balance = balances[receiver->id];
    }

    for (int tbd: to_be_deleted) {
        auto itr = std::find(receiver->wait_queue.begin(), receiver->wait_queue.end(), tbd);


        if (itr != receiver->wait_queue.end())
            receiver->wait_queue.erase(itr);
    }

}

int ReceiveBlock::setBlockHeights(int block_id) const {
    Node *receiver = nodes[receiver_id];
    Block incoming_block = receiver->blocks[block_id];

    if (receiver->blocks.find(incoming_block.prev_block_id) == receiver->blocks.end()) {
        // printf("Block with ID %d not found at receiver %d\n", incoming_block.prev_block_id, receiver->id);
        return -1;
    } else if (incoming_block.height != -1) {
        return incoming_block.height;
    }

    int parent_height = setBlockHeights(incoming_block.prev_block_id);

    if (parent_height == -1) {
        return -1;
    }

    int new_height = parent_height + 1;
    // receiver->blocks[incoming_block.id].height = new_height;

    // TODO: Add logic to verify block

    return new_height;

}


void ReceiveBlock::transmitBlock(Block block) const {
    Node *receiver = nodes[receiver_id];
    // Add receive block events to transmit the received block
    for (int neighbour_id : receiver->links) {
        if (neighbour_id != sender_id) {
            Node *neighbour = nodes.at(neighbour_id);
            int message_size_bytes = block.getMessageSizeBytes();
            double latency = receiver->calculateLatencyToNode(neighbour, message_size_bytes);


            // if (receiver->node_character_type == HONEST && block.owner_id == 8 || block.owner_id == 9) {
            //     printf("Hosnest node %d transmitting selfish block %d to %d\n", receiver->id, block.id, neighbour->id);
            // }


            event_queue.push(new ReceiveBlock(time + latency, RECEIVE_BLOCK, neighbour_id, receiver_id, block));
        }
    }
}