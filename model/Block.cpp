#include "../headers/Block.h"
#include "../constants/SimulationParameters.h"


Block::Block(int block_id, int owner_id, int prev_blk_id, double time, vector<Transaction> i_txn) {
    this->id = block_id;
    this->owner_id = owner_id;
    this->prev_block_id = prev_blk_id;
    this->timestamp = time;
    this->transactions = i_txn;
}

Block::Block() {
    // default constructor
}

int Block::getMessageSizeBytes() const {
    return transactions.size() * TRANSACTION_SIZE_BYTES + 1; // Empty block is considered to be 1kb
}