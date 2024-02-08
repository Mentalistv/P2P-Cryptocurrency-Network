#include <iostream>
#include <string>
#include <random>

#include "Node.h"

using namespace std;

class Link
{
public:
    Node *node_1;
    Node *node_2;
    double capacity;
    double light_speed;

    Link(Node *node_1, Node *node_2);

    // ------------------- Function to get latency between two nodes of a link ----------------------------
    double get_latency(string message);

    // ------------------ Function to get adjacent node from a given node of the link ------------------------
    Node* get_adjacent_node(Node *node);
};