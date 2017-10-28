#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include "Graph.hpp"
#include "Shortest_Path_Constraints.hpp"
#include <cstdlib>


using namespace std;


int main(int argc, char *argv[]){

  Graph* g;
  if (argc == 1){
    g = Graph::makeGraph2();
    g->max_weight = 5;
  }
  else if (argc == 2)
    g = Graph::ReadGraphFromFile(argv[1]);

  ShortestPathConstraints sh= ShortestPathConstraints(*g, 0, g->n-1);
  int final_length = sh.compute();

  cout << "length of the path :" << final_length << endl;
  cout << "The path is : ";
  cout << "1 ";
  for (int i = 0; i< sh.shortestPath.size(); i++)
    cout << sh.shortestPath[sh.shortestPath.size()-1-i]+1 << " ";
  cout << "" << endl;
}

ShortestPathConstraints::ShortestPathConstraints(Graph graph, int source, int dest): G(graph), s(source), t(dest), max_weight(graph.max_weight){


  d = new int**[G.n];
  for(int v = 0; v < G.n; ++v) {
    d[v] = new int*[G.n];
    for(int k = 0; k < G.n; ++k) {
      d[v][k] = new int[max_weight];
    }
  }

  //tentative distances are infinite
  for (unsigned int v = 0; v<G.n; v++) {
    for (unsigned int k = 0; k<G.n; k++) {  //max : n steps
      for (unsigned int w = 0; w<max_weight; w++) { //max weight
        d[v][k][w] = infinity;
      }
    }
  }
  d[s][0][0] = 0;

  prec = new int*[G.n];
  //tentative parents are not found yet
  for (unsigned int v = 0; v<G.n; v++) {
    prec[v] = new int[max_weight];
    for (unsigned int w = 0; w<max_weight; w++) {
      prec[v][w] = -1;
    }
  }

  for (unsigned int w = 0; w<max_weight; w++) {
    prec[s][w] = s;
  }

}



int ShortestPathConstraints::compute(){


  for (int k = 1; k<G.n; k++){ //nb of steps
    //Initialize the new step :
    for (int v = 0;v<G.n;v++){ //for each vertex
      for (int w = 0; w<max_weight; w++){ //for each weight
        d[v][k][w] = d[v][k-1][w];
      }
    }

    //Dynamic Programming
    for (int v=0;v<G.n;v++){ //for each vertex
      for (int e=0; e<G.successors[v].size();e++){  //manipulating edges
        int succ = G.successors[v][e].dest;
        int edge_d = G.successors[v][e].cost;
        int edge_w = G.successors[v][e].constraint;
        for (int w = 0; w<max_weight-edge_w; w++){ //for each weight when new path doesn't exceed the limit
          if (d[v][k-1][w] != infinity){ //if a path to v with weight w exists
            if (d[succ][k][w+edge_w] > d[v][k-1][w] + edge_d){ //update the path
              d[succ][k][w+edge_w] = d[v][k-1][w]+edge_d;
              prec[succ][w+edge_w] = v;
            }
          }
        }
      }
    }
  }

  //find the best path to reach t
  int w = 0, best_w = 0;
  for (w = 0; w<max_weight; w++){
    if (d[t][G.n-1][w] < d[t][G.n-1][best_w])
      best_w=w;
  }

  w=best_w;

  if (d[t][G.n-1][w] == infinity){
    cout << "No path found to respect the constraints" << endl;
    exit(EXIT_FAILURE);
  }


  //creation of the path
  int count = t;
  int final_length = 0;
  while (count != s){
    shortestPath.push_back(count);
    int count2 = count;
    count = prec[count][w];
    for (int i = 0; i<G.successors[count].size(); i++){
      if (G.successors[count][i].dest==count2){ //find the arc between the two vertices
        w -= G.successors[count][i].constraint;
        final_length += G.successors[count][i].cost;
        break;
      }
    }
  }

  return final_length;
}
