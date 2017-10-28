#include <iostream>
#include <vector>
#include <limits>
#include "Graph.hpp"
#include "Dijkstra_opti.hpp"
#include "MinHeap.hpp"

using namespace std;

int main(int argc, char *argv[]){

  int id, p;

  Graph* g;

  if (argc==1)
    g = Graph::makeGraph2();
  else if (argc==2)
    g = Graph::ReadGraphFromFile(argv[1]);

  DijkstraOpti dij= DijkstraOpti(*g, 0, g->n-1);

  int final_length = dij.dijkstraComplexSeq();

  cout << "length of the path :" << final_length << endl;

  cout << "The path is : ";
  for (int i=0; i< dij.shortestPath.size(); i++)
    cout << dij.shortestPath[i]+1 << " ";
  cout << "" << endl;

}

DijkstraOpti::DijkstraOpti(Graph graph, int source, int dest): G(graph), s(source), t(dest), d(new int[G.n]), visited(new bool[G.n]), prec(new int[G.n]),  heap(MinHeap(graph.n, d)){
//Initialization


  //tentative distances are infinite
  for (unsigned int k = 0; k<G.n; k++) {
    d[k] = infinity;
  }
  d[s] = 0;

  // tentative parents are not found yet
  for (unsigned int k = 0; k<G.n; k++) {
    prec[k] = -1;
    visited[k] = false;
  }
  prec[s] = s;

}


int DijkstraOpti::distanceMin(){
  int v = heap.GetMin();
  heap.DeleteMin(); //delete v
  return v;
}

int DijkstraOpti::dijkstraComplexSeq(){
  //classic Dijkstra algorithm, with the use of a binary tree

  while(true){
    int v = distanceMin();
    visited[v] = true;

    //the algorithm stops as soon as t has been visited, or when all the vertices have been visited
    if (v == t || v == -1)
      break;

    for (unsigned int k = 0; k<G.successors[v].size(); k++){

      int successor = G.successors[v][k].dest;
      int cost = G.successors[v][k].cost;
      if (!visited[successor] && d[successor] > d[v] + cost){
        //update the tentative distances and parents of the vertices that have not been visited yet
        d[successor] = d[v] + cost;
        prec[successor] = v;
      }

      heap.BubbleUp(successor);
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

  return d[t];
}

vector<int> DijkstraOpti::reverse(vector<int> vec){
  vector<int> res;
  for (unsigned int k = 0; k<vec.size(); k++) res.push_back(vec[vec.size()-1-k]);
  return res;
}
