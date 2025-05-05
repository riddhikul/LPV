#include<iostream>
#include<vector>
#include<queue>

#include<omp.h>
#include<chrono>

using namespace std;
using namespace std::chrono;

class Graph{
    int V;
    vector<vector<int>> adj;

    public:
    Graph(int V): V(V), adj(V){}

    void addEdge(int v, int w){
        adj[v].push_back(w); //undirected graph
    }

// ==============Sequential DFS=======================
void SequentialDFS(int v, vector<bool>& visited){
    visited[v]=true;
    cout<<v<<" ";
    for(int n:adj[v]){
        if(!visited[n]){
            SequentialDFS(n,visited);
        }
    }
}

// ============ParallelDFSUtil================
void ParallelDFSUtil(int v,vector<bool> &visited){
    visited[v]=true;
    cout<<v<<" ";

    for(int i=0;i<adj[v].size();++i){
        int n=adj[v][i];

        #pragma omp task firstprivate(n){
            if(!visited[n]){
                ParallelDFSUtil(n,visited);
            }
        }
    }

    #pragma omp taskwait
}


// ==============Parallel DFS==========================
void ParallelDFS(int startvertex){
    vector<bool> visited(V,false);
    #pragma omp parallel{
        #pragma omp single{
            parallelDFSUtil(startvertex,visited);
        }
    } 
}

// ============SequentialBFS==========================

void sequentialBFS(int startVertex){
    vector<bool> visited(V, false);
    queue<int> q;
    visited[startVertex]= true;
    q.push(startVertex);

    while(!q.empty()){
        int v=q.front();
        q.pop();
        cout<<v<<" ";

        for(int n:adj[v]){
            if(!visited[n]){
                visited[n]=true;
                q.push(n);
            }
        }
    }
}

// ==================ParallelBFS============================
void ParallelBFS(int startVertex){
    vector<bool> visited(V,false);
    queue<int> q;

    visited[startVertex]=true;
    q.push(startVertex);

    while(!q.empty()){
      int size=q.size();

      #pragma omp parallel for 
      for(int i=0;i<size;++i){
        int v;

        #pragma omp critical{
            v=q.front();
            q.pop();
        }

        cout<<v<<" ";

        for(int )
      }
    }
}

}