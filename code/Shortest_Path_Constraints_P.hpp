#ifndef SHORTEST_PATH_H
#define SHORTEST_PATH_H

#include <iostream>
#include <vector>
#include <list>

using namespace std;

class ShortestPathConstraintsP {

public:
  Graph G;
  int s; //source
  int t; //destination
  int*** d; //table of distances d[v][j][w] -> distance to reach the vertex v via j arcs with a weight = w
               //is = infinity if no feasible path
  int** prec; //table of the precessors of each vertex i in the computation of the shortest path between s and i
                //second dimension : with a weight =w
              //prec[i][w] = -1 if no path has been found between s and i
  int max_weight;

  Arc** arc_array;
  int nb_arcs;

  int idproc;
  int nbproc;
  int startp;
  int nbp;


  vector<int> shortestPath; //the shortest path between s and t


  ShortestPathConstraintsP(Graph graph, int source, int dest, int id, int p, int start, int size_of_heap, int nb_acs);
  int compute();
};

#endif
