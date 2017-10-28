#ifndef KPATHS_H
#define KPATHS_H

#include <iostream>
#include <vector>
#include "Dijkstra_withoutmain.hpp"


using namespace std;

class k_shortest_paths{
public:
  Graph G;
  int s; //source
  int t; //destination
  int k; //number of shortest paths
  vector< vector<int> > shortestPaths; //contains the shortest paths from s to t


  k_shortest_paths(Graph graph, int source, int dest, int nb);
  vector< vector<int> > kShortestPathsSeq();

private:
  vector<int> subVector(vector<int> vec, int i, int j);

};

#endif
