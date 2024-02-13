#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <unordered_set>

#include "../headers/Network.h"
#include "../headers/Utility.h"

using namespace std;

void dfsCheck(vector<vector<bool>>& matrix, vector<bool>& visited, int v=0){
    visited[v] = true;

    for(int i=0; i<matrix[v].size(); i++){
        if(matrix[v][i] && !visited[i]){
            dfsCheck(matrix, visited, i);
        }
    }
}

bool checkConnected(vector<vector<bool>> matrix){
    int n = matrix.size();

    vector<bool> visited;
    visited.resize(n);

    dfsCheck(matrix, visited);

    for(auto x: visited){
        if(!x)  return false;
    }

    return true;
}

void reinitialize(vector<vector<bool>>& matrix, vector<int>& degrees) {
    for (int i = 0; i < matrix.size(); i++) {
        fill(matrix[i].begin(), matrix[i].end(), false);
    }
    fill(degrees.begin(), degrees.end(), 0);
}


void showGraphDegrees(vector<int> degrees) {
    for (int deg: degrees)
        cout << deg << " ";

    cout << endl;
}

void initilializeNodeLinks(vector<Node*> &nodes, vector<vector<bool>> &matrix) {
    for (int i = 0; i < matrix.size(); i++) {
        vector<int> links;
        for (int j = 0; j < matrix[0].size(); j++) {
            if (matrix[i][j])
                links.push_back(j);
        }
        nodes[i]->links = links;
    }
}

void generateNetwork(vector<Node*> &nodes) {
    int n = NUMBER_OF_NODES;
    int maxp = MAX_PEERS_CONNECTED;
    int minp = MIN_PEERS_CONNECTED;

    vector<int> degrees(n, 0);
    vector<vector<bool>> matrix(n, vector<bool>(n, false));

    do {
        srand(time(nullptr));
        reinitialize(matrix, degrees);

        for (int i = 0; i < n; i++) {
            if (degrees[i] == maxp)
                continue;

            int more_needed =  uniformDistributionInt(minp, maxp) - degrees[i];

            while (more_needed > 0) {
                int adj_node = uniformDistributionInt(0, n - 1);
                if (i == adj_node || matrix[i][adj_node] || degrees[adj_node] == maxp)
                    continue;
                
                matrix[i][adj_node] = 1;
                matrix[adj_node][i] = 1;

                degrees[i]++;
                degrees[adj_node]++;

                more_needed--;
            }
        }


    } while (!checkConnected(matrix));

    // showGraphDegrees(degrees);
    initilializeNodeLinks(nodes, matrix);
}