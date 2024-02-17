#ifndef SIMULATION_PARAMETERS_H
#define SIMULATION_PARAMETERS_H

const int NUMBER_OF_NODES = 4;
const int MIN_PEERS_CONNECTED = 3;
const int MAX_PEERS_CONNECTED = 6;

const double LIGHT_SPEED_DELAY_MIN = 0.01; // secs
const double LIGHT_SPEED_DELAY_MAX = 0.5; //secs

const double TRANSACTION_INTERARRIVAL_MEAN = 6; //secs

const int INITIAL_BALANCE = 100;

const int TRANSACTION_SIZE_BYTES = 1024; //bytes

const double BLOCK_INTERARRIVAL_TIME = 600; //secs

const double SPEND_FRACTION_OF_BALANCE = 10.0;

const double SIMULATION_TIME_MULTIPLIER = 100;

#endif // SIMULATION_PARAMETERS_H