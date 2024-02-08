#include "../headers/Utility.h"
#include "../constants/SimulationParamenters.h"

using namespace std;

int uniform_distribution(int min, int max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distribution(min, max);
    int random_int = distribution(gen);
    return random_int;
}