#include "../headers/Utility.h"
#include "../constants/SimulationParamenters.h"

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
    uniform_int_distribution<double> distribution(min, max);
    double random_double = distribution(gen);
    return random_double;
}

double exponentialDistribution(double lambda){
    random_device rd;
    mt19937 gen(rd());
    exponential_distribution<int> expt_dist(lambda);
    double random = expt_dist(gen);
    return random;
}