#include "../headers/Utility.h"
#include "../constants/SimulationParameters.h"

#include <iostream>

using namespace std;

int uniformDistributionInt(int min, int max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distribution(min, max);
    int random_int = distribution(gen);
    return random_int;
}

double uniformDistributionDouble(double min, double max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> distribution(min, max);
    double random_double = distribution(gen);
    return random_double;
}

double exponentialDistribution(double mean){
    random_device rd;
    mt19937 gen(rd());
    double lambda = 1.0/mean;
    exponential_distribution<double> distribution(lambda);
    double random = distribution(gen);
    return random;
}

double getPoWDelay(double hashing_power) {
    cout << "hashing power = " << hashing_power << endl;
    double delay = exponentialDistribution((double) BLOCK_INTERARRIVAL_TIME/ (double) hashing_power);
    return delay;
}

long double exponentialDistributionLongDouble(double mean){
    random_device rd;
    mt19937 gen(rd());
    double lambda = 1.0/mean;
    exponential_distribution<long double> distribution(lambda);
    double random = distribution(gen);
    return random;
}