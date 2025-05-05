// 10) Design and implement Parallel Breadth First Search and Depth First Search based on
// existing algorithms using OpenMP. Use a Tree or an undirected graph for BFS and DFS .
// Measure the performance of sequential and parallel algorithms.

#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

class Graph {
    int V;
    vector<vector<int>> adj;

public:
    Graph(int V) : V(V), adj(V) {}

    void addEdge(int v, int w) {
        adj[v].push_back(w);
    }

    // ================== Sequential DFS ==================
    void sequentialDFS(int v, vector<bool>& visited) {
        visited[v] = true;
        cout << v << " ";
        for (int n : adj[v]) {
            if (!visited[n])
                sequentialDFS(n, visited);
        }
    }

    // ================== Parallel DFS using Tasks ==================
    void parallelDFSUtil(int v, vector<bool>& visited) {
        visited[v] = true;
        cout << v << " ";

        for (int i = 0; i < adj[v].size(); ++i) {
            int n = adj[v][i];

            #pragma omp task firstprivate(n)
            {
                if (!visited[n]) {
                    parallelDFSUtil(n, visited);
                }
            }
        }

        #pragma omp taskwait
    }

    void parallelDFS(int startVertex) {
        vector<bool> visited(V, false);
        #pragma omp parallel
        {
            #pragma omp single
            {
                parallelDFSUtil(startVertex, visited);
            }
        }
    }

    // ================== Sequential BFS ==================
    void sequentialBFS(int startVertex) {
        vector<bool> visited(V, false);
        queue<int> q;
        visited[startVertex] = true;
        q.push(startVertex);

        while (!q.empty()) {
            int v = q.front();
            q.pop();
            cout << v << " ";

            for (int n : adj[v]) {
                if (!visited[n]) {
                    visited[n] = true;
                    q.push(n);
                }
            }
        }
    }

    // ================== Parallel BFS (simplified) ==================
    void parallelBFS(int startVertex) {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[startVertex] = true;
        q.push(startVertex);

        while (!q.empty()) {
            int size = q.size();

            #pragma omp parallel for
            for (int i = 0; i < size; ++i) {
                int v;
                
                #pragma omp critical
                {
                    v = q.front();
                    q.pop();
                }

                cout << v << " ";

                for (int n : adj[v]) {
                    if (!visited[n]) {
                        #pragma omp critical
                        {
                            if (!visited[n]) {
                                visited[n] = true;
                                q.push(n);
                            }
                        }
                    }
                }
            }
        }
    }
};

// ============ Main with Timing ============
int main() {
    Graph g(7);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);
    g.addEdge(2, 6);

    cout << "===== Depth-First Search =====\n";

    auto start = omp_get_wtime();
    cout << "Sequential DFS: ";
    vector<bool> visitedDFS(7, false);
    g.sequentialDFS(0, visitedDFS);
    auto end = omp_get_wtime();
    cout << "\nTime: " << (end - start) << " seconds\n";

    start = omp_get_wtime();
    cout << "Parallel DFS: ";
    g.parallelDFS(0);
    end = omp_get_wtime();
    cout << "\nTime: " << (end - start) << " seconds\n";

    cout << "\n===== Breadth-First Search =====\n";

    start = omp_get_wtime();
    cout << "Sequential BFS: ";
    g.sequentialBFS(0);
    end = omp_get_wtime();
    cout << "\nTime: " << (end - start) << " seconds\n";

    start = omp_get_wtime();
    cout << "Parallel BFS: ";
    g.parallelBFS(0);
    end = omp_get_wtime();
    cout << "\nTime: " << (end - start) << " seconds\n";

    return 0;
}