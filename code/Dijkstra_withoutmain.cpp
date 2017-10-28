#include <iostream>
#include <vector>
#include "Dijkstra_withoutmain.hpp"

using namespace std;

// int main(int argc, char *argv[]){
//
//   int id, p;
//
//   Graph* g;
//
//   if (argc==1)
//     g = Graph::makeGraph2();
//   else if (argc==2)
//     g = Graph::ReadGraphFromFile(argv[1]);
//
//   Dijkstra dij= Dijkstra(*g, 0, g->n-1);
//
//   int final_length = dij.dijkstraNaifSeq();
//
//   cout << "length of the path :" << final_length << endl;
//
//   cout << "The path is : ";
//   for (int i=0; i< dij.shortestPath.size(); i++)
//     cout << dij.shortestPath[i]+1 << " ";
//   cout << "" << endl;
//
// }

Dijkstra::Dijkstra(Graph graph, int source, int destination){
  G = graph;
  s = source;
  t = destination;
  //tentative distances are infinite
  //int infinity = numeric_limits<int>::max();
  d = new int[G.n];
  for (unsigned int k = 0; k<G.n; k++) {
    d[k] = infinity;
  }
  d[s] = 0;
  //tentative parents are not found yet
  prec = new int[G.n];
  for (unsigned int k = 0; k<G.n; k++) {
    prec[k] = -1;
  }
  prec[s] = s;
  //no node has been visited
  visited = new bool[G.n];
  for (unsigned int k = 0; k<G.n; k++) {
    visited[k] = false;
  }
}

int Dijkstra::distanceMin(){
  //int infinity = numeric_limits<int>::max();
  int min = infinity;
  int sommet = -1;
  for (unsigned int k = 0; k<G.n; k++){
    if (!visited[k] && d[k] < min){
      min = d[k];
      sommet = k;
    }
  }
  return sommet;
}

int Dijkstra::dijkstraNaifSeq(){
  while(true){
    int v = distanceMin();
    visited[v] = true;

    //the algorithm stops as soon as t has been visited, or when all the vertices have been visited
    if (v == t || v == -1)
      break;

    for (unsigned int k = 0; k<G.successors[v].size(); k++){//for every child of v
      int successor = G.successors[v][k].dest;
      int cost = G.successors[v][k].cost;
      if (!visited[successor] && d[successor] > d[v] + cost){
        //update the tentative distances and parents of the vertices that have not been visited yet
        d[successor] = d[v] + cost;
        prec[successor] = v;
      }
    }
  }

  //creation of the path
  int count = t;
  while (count != s){
    shortestPath.push_back(count);
    count = prec[count];
  }
  shortestPath.push_back(s);
  shortestPath = reverse(shortestPath);
  int res = d[t];

  //delete the arrays
  delete[] d;
  delete[] visited;
  delete[] prec;

  return res;
}

vector<int> Dijkstra::reverse(vector<int> vec){
  vector<int> res;
  for (unsigned int k = 0; k<vec.size(); k++) res.push_back(vec[vec.size()-1-k]);
  return res;
}
