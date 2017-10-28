#ifndef KPATHS_P_H
#define KPATHS_P_H

#include <iostream>
#include <vector>
#include "Graph.hpp"
#include "Dijkstra_withoutmain.hpp"

using namespace std;

class k_shortest_paths_p{
public:
  Graph G;
  int s; //source
  int t; //destination
  int k; //number of shortest paths
  vector< vector<int> > shortestPaths; //contains the shortest paths from s to t

  int idproc; //id of the core
  int nbproc; //nb of cores

  k_shortest_paths_p(Graph graph, int source, int dest, int nb, int id, int p);
  vector< vector<int> > kShortestPathsPar();

private:
  int* vectorToTable(vector<int> vec); //convert a vector into a table
  vector<int> tableToVector(int* table, int size); //convert a table into a vector
  vector<int> subVector(vector<int> vec, int i, int j);


  void kShortestPathsPar_stagej(vector<int> previous_jBestPath); //the jth stage in the algorithm, 0<j<k, which
    //returns the (j+1)_th shortest path as a deviation of the jth shortest one
};


#endif
