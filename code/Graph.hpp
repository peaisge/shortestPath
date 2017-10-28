#ifndef GRAPH_H
#define GRAPH_H
#define infinity 100000000

#include <iostream>
#include <vector>
#include "Arc.hpp"

using namespace std;

class Graph {

public:
  int n; //number of vertices
  int m; //number of arcs
  int max_weight;
  vector< vector<Arc> > successors; //vectors of pairs of size n :
                                             //the successors of each vertex and the cost of the arc
  Graph();
  Graph(int n, int m);
  static Graph* makeGraph1();
  static Graph* makeGraph2();
  static Graph* makeGraph3();

  vector<Arc> edgesToAdd; //the edges erased from the graph during the algorithm of the k shortest paths

  static Graph* ReadGraphFromFile(char* name);
  void printGraph();

  Arc getEdge(int i, int j); //returns the edge between i and j, supposing that this exists
  int computeCost(vector<int> path); //computes the cost of a path represented by a vector
  int computeCost_table(int* path, int size); //computes the cost of a path represented by a table

  void removeEdge(int i, int j); //removes the edge between vertices i and j, if this exists
  void addEdge(Arc edge); //adds the edge between vertices i and j, supposing that this does not exist
  void removeNode(int i); //removes the vertex i, and all the related edges

  void addErasedNode(int i); //re-add a node that has been erased = attempt

};

#endif
