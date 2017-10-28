#ifndef DIJKSTRA_OPTI_P_H
#define DIJKSTRA_OPTI_P_H



#include <iostream>
#include <vector>
#include "MinHeap.hpp"


using namespace std;

class DijkstraOptiP {

public:
  Graph G;
  int s; //source
  int t; //destination
  int* d; //table of distances
  int* prec; //table of the precessors of each vertex i in the computation of the shortest path between s and i
              //prec[i] = -1 if no path has been found between s and i
  bool* visited; //table such that visited[i] = true if the vertex i has been visited through the algorithm
  MinHeap heap; //binary heap to get a better complexity
  //each processor has a unique heap, which only contains the vertices it deals with

  vector<int> shortestPath; //the shortest path between s and t

  int idproc;
  int nbproc;
  int startp; //id of the first vertex it deals with
  int nbp; //number of vertices it deals with

  DijkstraOptiP(Graph graph, int source, int dest, int id, int p, int start, int size_of_heap);
  void changeSucc(); //update the lists of successors to get rid of the successors which are not
                     //considered by the proc
  int dijkstraComplexPar(); //main function

  vector<int> reverse(vector<int> vec); //mirror of a vector

};

#endif
