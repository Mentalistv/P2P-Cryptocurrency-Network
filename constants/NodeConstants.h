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

enum NodeCharacterType {
    HONEST,
    SELFISH
};

const int GENESIS_BLOCK_ID = -1;
const int NO_PREVIOUS_BLOCK = -2;

const int COINBASE_TXN_SENDER_ID = -3;

#endif // NODE_CONSTANTS_H