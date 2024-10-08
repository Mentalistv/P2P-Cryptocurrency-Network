#include<iostream>
#include<vector>
#include <string>
#include <queue>
#include <fstream>

#include "constants/NodeConstants.h"
#include "constants/SimulationParameters.h"
#include "headers/Network.h"
#include "headers/Globals.h"

using namespace std;

vector<Node*> nodes;
priority_queue<Event*, std::vector<Event*>, EventComparator> event_queue;
int BLOCK_ID_GENERATOR = 0;
double LIGHT_SPEED_DELAY = 0;
vector<vector<double>> propagation_delays;
int NUMBER_OF_HONEST_NODES = NUMBER_OF_NODES - 2;


void initilizeHashingPower(
    vector<Node*> &all_nodes, float slow, float lowCPU, float selfish1CPUp, float selfish2CPUp
) {
    int slow_nodes = slow;
    int fast_nodes = NUMBER_OF_HONEST_NODES - slow_nodes;
    int low_CPU_nodes = lowCPU;
    int high_CPU_nodes = NUMBER_OF_HONEST_NODES - low_CPU_nodes;

    double hp_slow = (1.0 - selfish1CPUp - selfish2CPUp)/(10 * high_CPU_nodes + low_CPU_nodes);
    double hp_fast = 10 * hp_slow;

    while (slow_nodes > 0) {
        int rand_id = uniformDistributionInt(0, NUMBER_OF_HONEST_NODES - 1);
        if (all_nodes[rand_id]->node_type == NODE_FAST) {
            all_nodes[rand_id]->node_type = NODE_SLOW;
            slow_nodes--;
        }
    }

    while (low_CPU_nodes > 0) {
        int rand_id = uniformDistributionInt(0, NUMBER_OF_HONEST_NODES - 1);

        if (all_nodes[rand_id]->node_cpu_type == NODE_CPU_FAST) {
            all_nodes[rand_id]->node_cpu_type = NODE_CPU_SLOW;
            all_nodes[rand_id]->hashing_power = hp_slow;
            low_CPU_nodes--;
        }
    }

    for (int i = 0; i < NUMBER_OF_HONEST_NODES; i++) {
        if (all_nodes[i]->node_cpu_type == NODE_CPU_FAST)
            all_nodes[i]->hashing_power = hp_fast;
    }
}

void initilizePropagationDelays() {
    propagation_delays = vector<vector<double>>(NUMBER_OF_NODES, vector<double>(NUMBER_OF_NODES, 0.0));
    for (int i = 0; i < NUMBER_OF_NODES; i++) {
        for (int j = i; j < NUMBER_OF_NODES; j++) {
            double delay = uniformDistributionDouble(LIGHT_SPEED_DELAY_MIN, LIGHT_SPEED_DELAY_MAX);
            propagation_delays[i][j] = delay;
            propagation_delays[j][i] = delay;
        }
    }
}

void printFinalState() {
    char opfilename[100];
    sprintf(opfilename, "outputs/final_state.txt");
    ofstream outfile(opfilename, ios::out | ios::trunc);

    vector<int> blocks_mined(NUMBER_OF_NODES, 0);
    vector<int> total_mined(NUMBER_OF_NODES, 0);

    Node* node0 = nodes[0];
    Block temp = node0->blocks[node0->deepest_block_id];
    // int lc_length = temp.height;
    int lc_length = nodes[0]->blocks[nodes[0]->deepest_block_id].height + 1;

    while (temp.id != GENESIS_BLOCK_ID) {
        blocks_mined[temp.owner_id]++;
        temp = node0->blocks[temp.prev_block_id];
    }

    for (Node* node: nodes) {
        char line[1000];
        outfile << "Node ID = " << node->id << " Hashing power = " << node->hashing_power << " Blocks = "
            << node->blocks.size() << " Longest Chain = " << node->blocks[node->deepest_block_id].height + 1;


        for (auto itr: node->blocks) {
            if (itr.second.owner_id == node->id && itr.second.id != -1)
                total_mined[node->id]++;
        }


        if (node->node_cpu_type == NODE_CPU_FAST) {
            outfile << " CPU_HIGH";  
        } else {
            outfile << " CPU_LOW";
        }

        if (node->node_type == NODE_FAST) {
            outfile << " NODE_FAST";
        } else {
            outfile << " NODE_SLOW";
        }

        float mpu = 0;
        
        if (total_mined[node->id] != 0) {
            mpu = (float) blocks_mined[node->id]/ (float) total_mined[node->id];
        }

        outfile <<  " Blocks mined in LC = " << blocks_mined[node->id] << " Total Mined = " << total_mined[node->id] << " MPU = " << mpu << " Ratio of blocks mined = " << (double) blocks_mined[node->id] / (double) lc_length  << " PC = " << node->private_chain.size() << " WQ = " << node->wait_queue.size() << endl;
    }

    int total_blocks = 0;
    for (int mined: total_mined)
        total_blocks += mined;

    float mpu_overall = 0;

    if (total_blocks != 0)
        mpu_overall = (float) (nodes[0]->blocks[nodes[0]->deepest_block_id].height)/ (float)total_blocks;

    cout << "LC = " << (nodes[0]->blocks[nodes[0]->deepest_block_id].height) << " tb = " << total_blocks << endl;

    outfile << "MPU overall = " << mpu_overall << endl;
}

void printBlockTree() {
    char opfilename[100];
    for (Node* node: nodes) {
        sprintf(opfilename, "outputs/tree%d.csv", node->id);
        ofstream outfile(opfilename, ios::out | ios::trunc);
        
        if (outfile.is_open()) {
            for (auto itr: node->blocks) {
                Block b = itr.second;
                string type = nodes[b.owner_id]->node_character_type == HONEST ? "h" : "s";
                int selfish_miner_no = 0;
                
                if (nodes[b.owner_id]->node_character_type == SELFISH) {
                    selfish_miner_no = b.owner_id - NUMBER_OF_HONEST_NODES + 1;
                }

                if (itr.second.prev_block_id != NO_PREVIOUS_BLOCK)
                    outfile << b.id << "," << b.prev_block_id << "," << b.arrival_time << "," << b.transactions.size() << "," << type << "," << selfish_miner_no << "," << b.owner_id << endl;
            }

            outfile.close();
        }
    }
}

double printRatioForNumberOfBlocksGeneratedByCPUType() {
    Node* node0 = nodes[0];
    Block temp = node0->blocks[node0->deepest_block_id];

    int lowCPU = 0;
    int slow = 0;

    while (temp.id != GENESIS_BLOCK_ID) {
        if (nodes[temp.owner_id]->node_cpu_type == NODE_CPU_SLOW)
            lowCPU++;
        
        if (nodes[temp.owner_id]->node_type == NODE_SLOW)
            slow++;

        temp = node0->blocks[temp.prev_block_id];
    }


    int highCPU = NUMBER_OF_NODES - lowCPU;
    int fast = NUMBER_OF_NODES - slow;

    double lowCPURatio = (double)lowCPU/(double) node0->blocks.size();
    double highCPURatio = 1 - lowCPURatio;

    double slowRatio = (double) slow / (double) node0->blocks.size();
    double fastRatio = 1 - slowRatio;

    char opfilename[100];
    sprintf(opfilename, "outputs/ratios.txt");
    ofstream outfile(opfilename, ios::out | ios::trunc);

    outfile << "Ratio of blocks generated by: Low CPU = " << lowCPURatio << " High CPU = " <<  highCPURatio << endl;
    
    outfile << "Ratio of blocks generated by: Slow Nodes = " << slowRatio << " Fast Nodes = " <<  fastRatio << endl;
}


void printBalancesFromLongestChain() {
    cout << "Balances from longest chain..." << endl;
    for (Node* node: nodes) {
        vector<double> balances = node->calculateBalancesFromBlock(node->deepest_block_id);
        printf ("dbi = %d ", node->deepest_block_id);
        for (double balance: balances) {
            cout << balance << " ";
        }
        cout << endl;
    }
}

void printBalancesFromBalanceVectors() {
    cout << "balances from balance vectors..." << endl;
    for (Node* node: nodes) {
        vector<double> balances = node->balances;
        for (double balance: balances) {
            cout << balance << " ";
        }
        cout << endl;
    }
}

double getRatioForNumberOfBlocksGeneratedByCPUType() {

}

int main(int argc, char const *argv[]) {    
    Transaction::txn_count = 0;
    
    int node_id = 0;

    if (argc != 5) {
        cout << "Usage: ./simulation <slow_nodes_%> <low_cpu_%> <selfish1_hp%> <selfish2_hp%>";
        return 1;
    }

    int slowp = atoi(argv[1]);
    int lowCPUp = atoi(argv[2]);
    int selfish1 = atoi(argv[3]); 
    int selfish2 = atoi(argv[4]); 

    vector<Node*> allnodes;


    for (int i = 0; i < NUMBER_OF_HONEST_NODES; i++) {
        allnodes.push_back(new Node(node_id++, NODE_FAST, NODE_CPU_FAST, HONEST));
    }
    float selfish1CPUp = selfish1 / 100.0;
    float selfish2CPUp = selfish2 / 100.0;

    Node* selfish_node_1 = new Node(node_id++, NODE_FAST, NODE_CPU_FAST, SELFISH);
    selfish_node_1->hashing_power = selfish1CPUp; 
    
    Node* selfish_node_2 = new Node(node_id++, NODE_FAST, NODE_CPU_FAST, SELFISH);
    selfish_node_2->hashing_power = selfish2CPUp; 
      
    allnodes.push_back(selfish_node_1);
    allnodes.push_back(selfish_node_2);


    float slow = NUMBER_OF_HONEST_NODES * (float) slowp/100.0;
    float lowCPU = NUMBER_OF_HONEST_NODES * (float) lowCPUp/100.0;
    initilizeHashingPower(allnodes, slow, lowCPU, selfish1CPUp, selfish2CPUp);

    nodes = allnodes;

    generateNetwork(nodes);
    initilizePropagationDelays();

    for (Node* node: nodes) {
        event_queue.push(new GenerateTransaction(node, GENERATE_TRANSACTION, 0.0));
        double delay = getPoWDelay(node->hashing_power);
        Block new_block = node->createNewBlock(0.0);
        event_queue.push(new MineBlock(0.0 + delay, MINE_BLOCK, node->id, new_block));
    }


    double simulation_stop_time = BLOCK_INTERARRIVAL_TIME * SIMULATION_TIME_MULTIPLIER;

    while (!event_queue.empty()) {
        Event* event = event_queue.top();

        if (event->time > simulation_stop_time) {
            // nodes[NUMBER_OF_NODES - 2]->releasePrivateChain(event->time);
            // nodes[NUMBER_OF_NODES - 1]->releasePrivateChain(event->time);
            if (event->type == GENERATE_TRANSACTION || event->type == MINE_BLOCK) {
                event_queue.pop();
                continue;
            }
        }

        event->printEvent();
        event->processEvent();

        event_queue.pop();
    }

    printFinalState();
    printBlockTree();
    printRatioForNumberOfBlocksGeneratedByCPUType();
    
    return 0;
}



