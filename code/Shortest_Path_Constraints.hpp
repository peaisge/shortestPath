#ifndef SHORTEST_PATH_H
#define SHORTEST_PATH_H

#include <iostream>
#include <vector>
#include <list>

using namespace std;

class ShortestPathConstraints {

public:
  Graph G;
  int s; //source
  int t; //destination
  int*** d; //table of distances d[v][j][w] -> distance to reach the vertex v via j arcs with a weight = w
               //is = infinity if no feasible path
  int** prec; //table of the precessors of each vertex i in the computation of the shortest path between s and i
                //second dimension : with a weight =w
              //prec[i][w] = -1 if no path has been found between s and i
  int visits; //number of vertices that have been visited
  int max_weight;


  vector<int> shortestPath; //the shortest path between s and t



  ShortestPathConstraints(Graph graph, int source, int destination);
  int compute();
};

#endif
