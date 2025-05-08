#include <iostream>
#include <vector>
#include <stack>
#include <omp.h>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;

vector<vector<int>> graph;
vector<bool> visited;

// Parallel DFS function using OpenMP
void parallelDFS(int start) {
    stack<int> s;
    s.push(start);
    visited[start] = true;

    cout << "DFS Traversal: ";

    while (!s.empty()) {
        int u;

        // Safely pop from stack
        #pragma omp critical
        {
            if (!s.empty()) {
                u = s.top();
                s.pop();
                cout << u << " ";
            }
        }

        // Explore neighbors in parallel
        #pragma omp parallel for
        for (int i = 0; i < graph[u].size(); i++) {
            int v = graph[u][i];

            if (!visited[v]) {
                #pragma omp critical
                {
                    if (!visited[v]) {
                        visited[v] = true;
                        s.push(v);
                    }
                }
            }
        }
    }
}

int main() {
    int V, E;
    cout << "Enter number of vertices: ";
    cin >> V;
    cout << "Enter number of edges: ";
    cin >> E;

    graph.resize(V);
    visited.assign(V, false);

    cout << "Enter " << E << " edges (u v):\n";
    for (int i = 0; i < E; i++) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u); // Undirected graph
    }

    int start;
    cout << "Enter starting node for DFS: ";
    cin >> start;

    // Time measurement starts
    auto t1 = high_resolution_clock::now();
    parallelDFS(start);
    auto t2 = high_resolution_clock::now();
    // Time measurement ends

    double time_taken = duration_cast<nanoseconds>(t2 - t1).count() / 1e9;
    cout << "\nParallel DFS Execution Time: " << fixed << setprecision(10) << time_taken << " seconds" << endl;

    return 0;
}
