#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <iostream>
#include <vector>
#include "Graph.hpp"

using namespace std;

class Dijkstra {
public:
  Graph G;
  int s; //source
  int t; //destination
  vector<int> shortestPath; //the shortest path between s and t

  Dijkstra(Graph graph, int source, int destination);
  int dijkstraNaifSeq();

private:
  int* d; //table of distances
  int* prec; //table of the precessors of each vertex i in the computation of the shortest path between s and i
              //prec[i] = -1 if no path has been found between s and i
  bool* visited; //table such that visited[i] = true if the vertex i has been visited through the algorithm

  int distanceMin();
  vector<int> reverse(vector<int> vec); //mirror of a vector
};

#endif
