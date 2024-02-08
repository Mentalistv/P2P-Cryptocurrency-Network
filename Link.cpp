#include "headers/Link.h"

Link::Link(Node *node_1, Node *node_2)
{
    this->node_1 = node_1;
    this->node_2 = node_2;
    if (node_1->node_type == NODE_SLOW || node_2->node_type == NODE_SLOW)
    {
        capacity = 5;
    }
    else
    {
        capacity = 100;
    }

    // get light speed from uniform distribution
    random_device rd;
    mt19937 gen(rd());
    // range 10ms to 500ms
    double lower_bound = 10.0;
    double upper_bound = 500.0;

    std::uniform_real_distribution<double> distribution(lower_bound, upper_bound);
    light_speed = distribution(gen);
}

// ------------------- Function to get latency between two nodes of a link ----------------------------
double Link::get_latency(string message)
{
    // get size of message in bits
    int msg_size = sizeof(message) * 8;

    // get queuing delay
    random_device rd;
    mt19937 gen(rd());
    double lambda = 96 * 1000 / capacity;
    exponential_distribution<int> expt_dist(lambda);
    double q_delay = expt_dist(gen);

    return msg_size / capacity + q_delay + light_speed;
}

// ------------------ Function to get adjacent node from a given node of the link --------------------------
Node *Link::get_adjacent_node(Node *node)
{
    return node == node_1 ? node_2 : node_1;
}
