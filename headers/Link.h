#include <iostream>
#include <string>
#include <random>

#include "Node.h"

using namespace std;

class Link
{
public:
    int node_1_ID;
    int node_2_ID;
    double capacity;
    double light_speed;

    Link(int node_1_ID, int node_2_ID);

    // ------------------- Function to get latency between two nodes of a link ----------------------------
    double getLatency(string message);

    // ------------------ Function to get adjacent node from a given node of the link ------------------------
    int getAdjacentNode(int node_ID);
};