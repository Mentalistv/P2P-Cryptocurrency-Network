#ifndef NODE_CONSTANTS_H
#define NODE_CONSTANTS_H

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

#endif // NODE_CONSTANTS_H