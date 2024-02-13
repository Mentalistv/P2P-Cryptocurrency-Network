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
int BLOCK_ID_GENERATOR = 0;
double LIGHT_SPEED_DELAY = 0;


void initilizeHashingPower(vector<Node*> &all_nodes, float slow, float lowCPU) {
    int slow_nodes = slow;
    int fast_nodes = NUMBER_OF_NODES - slow_nodes;
    int low_CPU_nodes = lowCPU;

    double hp_slow = 1.0/(10*fast_nodes + slow_nodes);
    double hp_fast = 10*hp_slow;

    cout << "Here " << hp_slow << " " << hp_fast << " " << slow_nodes << " " << fast_nodes << endl;

    while (slow_nodes > 0) {
        int rand_id = uniformDistributionInt(0, NUMBER_OF_NODES - 1);
        if (all_nodes[rand_id]->node_type == NODE_FAST) {
            all_nodes[rand_id]->node_type = NODE_SLOW;
            slow_nodes--;
        }
    }

    while (low_CPU_nodes > 0) {
        int rand_id = uniformDistributionInt(0, NUMBER_OF_NODES - 1);

        if (all_nodes[rand_id]->node_cpu_type == NODE_CPU_FAST) {
            all_nodes[rand_id]->node_cpu_type = NODE_CPU_SLOW;
            all_nodes[rand_id]->hashing_power = hp_slow;
            low_CPU_nodes--;
        }
    }

    for (Node* node: all_nodes) {
        if (node->node_cpu_type == NODE_CPU_FAST)
            node->hashing_power = hp_fast;
    }
}

int main(int argc, char const *argv[]) {    
    Transaction::txn_count = 0;
    
    int node_id = 0;

    if (argc != 3) {
        cout << "Usage: ./simulation <slow_nodes_%> <low_cpu_%>";
        return 1;
    }

    int slowp = atoi(argv[1]);
    int lowCPUp = atoi(argv[2]);
    float slow = NUMBER_OF_NODES * (float) slowp/100.0;
    float lowCPU = NUMBER_OF_NODES * (float) lowCPUp/100.0;

    vector<Node*> allnodes;

    for (int i = 0; i < NUMBER_OF_NODES; i++) {
        // TODO: Make this distribution random
        allnodes.push_back(new Node(node_id++, NODE_FAST, NODE_CPU_FAST));
    }

    initilizeHashingPower(allnodes, slow, lowCPU);

    nodes = allnodes;

    generateNetwork(nodes);
    // LIGHT_SPEED_DELAY = uniformDistributionDouble(0.01, 0.5);
    LIGHT_SPEED_DELAY = 10;

    for (Node* node: nodes) {
        event_queue.push(new GenerateTransaction(node, GENERATE_TRANSACTION, 0.0));
        double delay = getPoWDelay(node->hashing_power);
        event_queue.push(new MineBlock(0.0 + delay, MINE_BLOCK, node->id, GENESIS_BLOCK_ID));
    }


    double simulation_stop_time = BLOCK_INTERARRIVAL_TIME * 10;

    while (!event_queue.empty()) {
        Event* event = event_queue.top();
        // cout << "Size of event queue = " << event_queue.size() << endl;
        if (event->time > simulation_stop_time) {
            if (event->type == GENERATE_TRANSACTION || event->type == MINE_BLOCK) {
                event_queue.pop();
                continue;
            }
        }

        // event->printEvent();
        event->processEvent();
        // if (event->type == MINE_BLOCK)

        event_queue.pop();
    }

    int lowest = 0;

    for (Node* node: nodes) {
        printf("Node ID = %d Blocks = %d  LC = %d DBI = %d\t", node->id, node->blocks.size(), node->blocks[node->deepest_block_id].height + 1, node->deepest_block_id);
        if (node->node_cpu_type == NODE_CPU_FAST)
            printf("CPU_FAST ");
        else 
            printf("CPU_SLOW ");

        if (node->node_type == NODE_FAST)
            printf("NODE_FAST \n");
        else 
            printf("NODE_SLOW \n");

        if (node->blocks[node->deepest_block_id].height + 1 < 25)
            lowest = node->id;
        
    }
    cout << endl;


    for (Node* node: nodes) {
        Block temp = node->blocks[node->deepest_block_id];
        int count = 0;
        while (temp.id != -1) {
            // printf("%d ", temp.id);
            count++;
            temp = nodes[lowest]->blocks[temp.prev_block_id];
        }
        count++;
        printf("printing node id %d with LC = %d count = %d\n", node->id, node->blocks[node->deepest_block_id].height + 1, count);

    }

    cout << endl;

    return 0;
}
