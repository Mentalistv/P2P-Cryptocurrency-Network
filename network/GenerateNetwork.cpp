#include <vector>
#include <cstdlib>
#include <ctime>

#include "../headers/Network.h"
#include "../headers/Utility.h"

using namespace std;

void dfsCheck(vector<vector<int>>& matrix, vector<bool>& visited, int v=0){
    visited[v] = true;

    for(auto u:matrix[v]){
        if(!visited[u]){
            dfsCheck(matrix, visited, u);
        }
    }
}

bool checkConnected(vector<vector<int>> matrix){
    int n = matrix.size();

    vector<bool> visited;
    visited.resize(n);

    dfsCheck(matrix, visited);

    for(auto x: visited){
        if(!x)  return false;
    }

    return true;
}

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