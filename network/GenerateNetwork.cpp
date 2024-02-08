#include <vector>
#include <cstdlib>
#include <ctime>

#include "../headers/Network.h"
#include "../headers/Utility.h"

using namespace std;

void generateNetwork() {
    int n = NUMBER_OF_NODES;
    int maxp = MAX_PEERS_CONNECTED;
    int minp = MIN_PEERS_CONNECTED;

    vector<int> degrees(n, 0);
    vector<vector<int>> matrix(n, vector<int>(n, 0));

    srand(time(nullptr));

    for (int i = 0; i < n; i++) {
        if (degrees[i] == maxp)
            continue;

        int more_needed =  uniform_distribution(minp, maxp) - degrees[i];

        while (more_needed > 0) {
            int adj_node = uniform_distribution(0, n);

            if (i == adj_node || matrix[i][adj_node] || degrees[adj_node] == maxp)
                continue;
            
            matrix[i][adj_node] = 1;
            matrix[adj_node][i] = 1;

            degrees[i]++;
            degrees[adj_node]++;

            more_needed--;
        }
    }

}