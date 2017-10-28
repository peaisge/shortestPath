#ifndef DIJKSTRA_P_H
#define DIJKSTRA_P_H

#include <iostream>
#include <vector>
#include "Graph.hpp"

using namespace std;

class Dijkstra_p {
public:
  Graph G;
  int s; //source
  int t; //destination
  vector<int> shortestPath; //the shortest path between s and t

  int idproc; //id of the core
  int nbproc; //nb of cores
  int startp; //first vertex managed by the core
  int nbp; //nb of vertices managed by the core

  Dijkstra_p(Graph graph, int source, int dest, int id, int p, int start, int size_of_heap);
  int dijkstraNaifPar();

private:
  int* d; //table of distances
  int* prec; //table of the precessors of each vertex i in the computation of the shortest path between s and i
              //prec[i] = -1 if no path has been found between s and i
  bool* visited; //table such that visited[i] = true if the vertex i has been visited through the algorithm

  int distanceMin();
  void changeSucc();
  vector<int> reverse(vector<int> vec); //mirror of a vector
};

#endif
