#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

int main() {
    int V, E;
    cout << "Enter number of vertices: ";
    cin >> V;
    cout << "Enter number of edges: ";
    cin >> E;

    vector<vector<int>> graph(V);
    vector<bool> visited(V, false);

    cout << "Enter " << E << " edges (u v):\n";
    for (int i = 0; i < E; ++i) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u); // undirected graph
    }

    int start;
    cout << "Enter starting node for BFS: ";
    cin >> start;

    queue<int> q;
    visited[start] = true;
    q.push(start);

    cout << "\nBFS Traversal: ";

    while (!q.empty()) {
        int size = q.size();
        vector<int> nextLevel;

        // Process current level in parallel
        #pragma omp parallel for
        for (int i = 0; i < size; ++i) {
            int u;

            // Use critical section to safely dequeue
            #pragma omp critical
            {
                if (!q.empty()) {
                    u = q.front();
                    q.pop();
                    cout << u << " ";
                }
            }

            #pragma omp parallel for
            for (int j = 0; j < graph[u].size(); ++j) {
                int v = graph[u][j];

                if (!visited[v]) {
                    #pragma omp critical
                    {
                        if (!visited[v]) {
                            visited[v] = true;
                            nextLevel.push_back(v);
                        }
                    }
                }
            }
        }

        for (int v : nextLevel) {
            q.push(v);
        }
    }

    return 0;
}
