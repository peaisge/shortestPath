#ifndef DIJKSTRA_OPTI_H
#define DIJKSTRA_OPTI_H

#include <iostream>
#include <vector>
#include "MinHeap.hpp"

using namespace std;

class DijkstraOpti {

public:
  Graph G;
  int s; //source
  int t; //destination
  int* d; //table of distances
  int* prec; //table of the precessors of each vertex i in the computation of the shortest path between s and i
              //prec[i] = -1 if no path has been found between s and i
  bool* visited; //table such that visited[i] = true if the vertex i has been visited through the algorithm
  MinHeap heap; //binary heap to get a better complexity
  vector<int> shortestPath; //the shortest path between s and t

  DijkstraOpti(Graph graph, int source, int destination);
  int distanceMin(); //take the best vertex in the heap and delete it
  int dijkstraComplexSeq(); //main function to compute the best path

  vector<int> reverse(vector<int> vec); //mirror of a vector

};

#endif
