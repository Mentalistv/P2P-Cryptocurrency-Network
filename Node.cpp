#include "headers/Node.h"


Node::Node(int node_id, NodeType nt, NodeCPUType nct) {
    id = node_id;
    node_type = nt;
    node_cpu_type = nct;
}