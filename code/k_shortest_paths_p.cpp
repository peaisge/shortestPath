#include <iostream>
#include <vector>
#include "k_shortest_paths_p.hpp"
#include "mpi.h"
#include <stdlib.h>


using namespace std;

int main(int argc, char *argv[]){
  int id, p;

  Graph* g;

  if (argc == 1)
    g = Graph::makeGraph2();
  else if (argc == 2)
    g = Graph::ReadGraphFromFile(argv[1]);

  // Initialize MPI.
  MPI_Init(&argc, &argv);

  // Get the number of processes.
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  // Get the individual process ID.
  MPI_Comm_rank(MPI_COMM_WORLD, &id);

  k_shortest_paths_p kShortestPar = k_shortest_paths_p(*g, 0, g->n-1, atoi(argv[1]), id, p);

  vector< vector<int> > kShortestPaths = kShortestPar.kShortestPathsPar();

  if (id == 0){
    cout << "The " << atoi(argv[1]) << " best paths are : " << endl;
    for (int i = 0; i < kShortestPaths.size(); i++){
        cout << i+1 << "th path : ";
        for (int j = 0; j<kShortestPaths[i].size(); j++){
          cout << kShortestPaths[i][j] << " ";
        }
        cout << "; whose cost is : " << kShortestPar.G.computeCost(kShortestPaths[i]) << endl;
    }

  }

}

k_shortest_paths_p::k_shortest_paths_p(Graph graph, int source, int dest, int nb, int id, int p){
  G = graph;
  s = source;
  t = dest;
  k = nb;
  idproc = id;
  nbproc = p;
}


int* k_shortest_paths_p::vectorToTable(vector<int> vec){
  int n = vec.size();
  int *res = new int[n];
  for (unsigned int k= 0; k<n; k++)
    res[k] = vec[k];
  return res;
}

vector<int> k_shortest_paths_p::tableToVector(int* table, int size){
  vector<int> res = vector<int>();
  for (unsigned int k= 0; k<size; k++)
    res.push_back(table[k]);
  return res;
}

vector<int> k_shortest_paths_p::subVector(vector<int> vec, int i, int j){
  vector <int> res;
  for (unsigned int k = i; k<=j; k++)
    res.push_back(vec[k]);
  return res;
}

void k_shortest_paths_p::kShortestPathsPar_stagej(vector<int> previous_jBestPath){

  const int root = 0;

  int nPath = previous_jBestPath.size();

  int size_of_heap;
  if (idproc < nbproc-1)
    size_of_heap = nPath/nbproc;
  else
    size_of_heap = nPath - (nPath / nbproc * (nbproc-1));
  int start = nPath / nbproc * idproc;

  int minCost = infinity;
  vector<int> minimalPath = vector<int>();

  for (unsigned int i = start; i < start + size_of_heap; i++){
    //Spur node is retrieved from the previous k-shortest path, k − 1
    int spurNode = previous_jBestPath[i];
    vector<int> rootPath;
    for (unsigned int q = 0; q<=i; q++) rootPath.push_back(previous_jBestPath[q]);

    for (unsigned int q = 0; q<shortestPaths.size(); q++){
      vector<int> path = shortestPaths[q];
      //checks if rootPath = subvector of a path from 0 to i
      if (rootPath == subVector(shortestPaths[q], 0, i)){
        //Remove the links that are part of the previous shortest paths which share the same root path
        int node1 = path[i];
        int node2 = path[i+1];
        G.removeEdge(node1, node2);
      }
    }

    for (unsigned int v = 0; v<rootPath.size(); v++){
      if (rootPath[v] != spurNode){
        G.removeNode(rootPath[v]);
      }
    }

    // Calculate the spur path from the spur node to the sink
    Dijkstra subDij =  Dijkstra(G, spurNode, t);
    G.printGraph();
    int d = subDij.dijkstraNaifSeq();
    vector<int> spurPath = subDij.shortestPath;
    for (unsigned int v = 0; v<spurPath.size(); v++) cout << spurPath[v] << " ";

    // Entire path is made up of the root path and spur path
    rootPath.insert(rootPath.end(), spurPath.begin()+1, spurPath.end());
    for (unsigned int v = 0; v<rootPath.size(); v++) cout << rootPath[v] << " ";

    // Add back the edges and nodes that were removed from the graph.
    for (unsigned int v = 0; v<G.edgesToAdd.size(); v++) G.addEdge(G.edgesToAdd[v]);
    G.edgesToAdd.clear();

    int cost = G.computeCost(rootPath);
    if (cost < minCost){
      minCost = cost;
      minimalPath = rootPath;
    }

  }

  // This handles the case of there being no spur paths, or no spur paths left.
  // This could happen if the spur paths have already been exhausted (added to shortestPaths),
  // or there are no spur paths at all - such as when both the source and sink vertices
  // lie along a "dead end"
  if (minCost == infinity)
    return;

  int *minimalPath_table = vectorToTable(minimalPath);
  int length = minimalPath.size();

  if (idproc > 0){
    MPI_Send(&length, 1, MPI_INT, root, 0, MPI_COMM_WORLD);
    MPI_Send(minimalPath_table, minimalPath.size(), MPI_INT, root, 0, MPI_COMM_WORLD);
  }

  else{
    int temp_size;
    int* temp_minimalPath_table = minimalPath_table;
    for (unsigned int k = 1; k<nbproc; k++){
      MPI_Recv(&temp_size, 1, MPI_INT, k, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv(temp_minimalPath_table, temp_size, MPI_INT, k, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      int temp_cost = G.computeCost_table(temp_minimalPath_table, temp_size);
      if (temp_cost < minCost){
        minCost = temp_cost;
        minimalPath = tableToVector(temp_minimalPath_table, temp_size);
      }
    }
  }

  //Add the lowest cost path becomes the k-shortest path
  shortestPaths.push_back(minimalPath);
}

vector< vector<int> > k_shortest_paths_p::kShortestPathsPar(){
  Dijkstra dij = Dijkstra(G, s, t);
  int d = dij.dijkstraNaifSeq();
  shortestPaths.push_back(dij.shortestPath);
  for (unsigned int p = 1; p<k; p++)
    kShortestPathsPar_stagej(shortestPaths[p-1]);
  return shortestPaths;
}
