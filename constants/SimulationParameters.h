#ifndef SIMULATION_PARAMETERS_H
#define SIMULATION_PARAMETERS_H

const int NUMBER_OF_NODES = 100;
const int MIN_PEERS_CONNECTED = 3;
const int MAX_PEERS_CONNECTED = 6;

const int RANDOM_GENERATOR_SEED = 13;

const int LIGHT_SPEED_DELAY = 20; //TODO : to be taken from uniform districution

const double TRANSACTION_INTERARRIVAL_MEAN = 6;

const int INITIAL_BALANCE = 101;

const int TRANSACTION_SIZE_BYTES = 1024;

const double BLOCK_INTERARRIVAL_TIME = 600.0;

#endif // SIMULATION_PARAMETERS_H