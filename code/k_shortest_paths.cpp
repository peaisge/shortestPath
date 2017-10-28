#include <iostream>
#include <vector>
#include "k_shortest_paths.hpp"
#include "Dijkstra_withoutmain.hpp"
#include "MinHeap.hpp"
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[]){
  Graph* g;

  if (argc == 2)
    g = Graph::makeGraph1();
  else if (argc == 3)
    g = Graph::ReadGraphFromFile(argv[2]);

  k_shortest_paths dijK = k_shortest_paths(*g, 0, g->n-1, atoi(argv[1]));
  vector< vector<int> > kShortestPaths = dijK.kShortestPathsSeq();

  cout << "The " << atoi(argv[1]) << " best paths are : " << endl;
  for (int i = 0; i < kShortestPaths.size(); i++){
      cout << i+1 << "th path : ";
      for (int j = 0; j<kShortestPaths[i].size(); j++){
        cout << kShortestPaths[i][j] << " ";
      }
      cout << "; whose cost is : " << dijK.G.computeCost(kShortestPaths[i]) << endl;
  }
}

k_shortest_paths::k_shortest_paths(Graph graph, int source, int dest, int nb){
  G = graph;
  s = source;
  t = dest;
  k = nb;
}

vector<int> k_shortest_paths::subVector(vector<int> vec, int i, int j){
  vector <int> res;
  for (unsigned int k = i; k<=j; k++)
    res.push_back(vec[k]);
  return res;
}

//adapted from Yen's algorithm
vector< vector<int> > k_shortest_paths::kShortestPathsSeq(){
  Dijkstra dij = Dijkstra(G, s, t);
  //G.printGraph();
  int d = dij.dijkstraNaifSeq();
  shortestPaths.push_back(dij.shortestPath);
  for (unsigned int p = 1; p<k; p++){

    int min = infinity;
    vector<int> minimalPath = vector<int>();

    //The spur node ranges from the first node to the next to last node in the previous k-shortest path
    for (unsigned int i = 0; i<shortestPaths[p-1].size()-1; i++){
      //Spur node is retrieved from the previous k-shortest path, k − 1
      cout << "i = " << i << endl;
      int spurNode = shortestPaths[p-1][i];
      //cout << "spurNode = " << spurNode << endl;
      vector<int> rootPath;
      for (unsigned int q = 0; q<=i; q++) rootPath.push_back(shortestPaths[p-1][q]);

      for (unsigned int q = 0; q<shortestPaths.size(); q++){
        vector<int> path = shortestPaths[q];
        //checks if rootPath = subvector of a path from 0 to i
        if (rootPath == subVector(path, 0, i)){
          //Remove the links that are part of the previous shortest paths which share the same root path
          int node1 = path[i];
          int node2 = path[i+1];
          //cout << "m = " << G.m << endl;
          G.removeEdge(node1, node2);
          //cout << "m = " << G.m << endl;
        }
      }

      for (unsigned int v = 0; v<rootPath.size(); v++){
        if (rootPath[v] != spurNode){
          //cout << "erase node " << rootPath[v] << endl;
          G.removeNode(rootPath[v]);
          //cout << "erased node" << endl;
        }
      }

      // Calculate the spur path from the spur node to the sink
      Dijkstra subDij = Dijkstra(G, spurNode, t);

      //G.printGraph();
      int d = subDij.dijkstraNaifSeq();
      cout << "ok" << endl;
      //cout << "d = " << d << endl;
      vector<int> spurPath = subDij.shortestPath;

      // Entire path is made up of the root path and spur path
      rootPath.insert(rootPath.end(), spurPath.begin()+1, spurPath.end());
      // Add the potential k-shortest path to shortestPath_temp

      // Add back the edges and nodes that were removed from the graph.
      for (unsigned int v = 0; v<G.edgesToAdd.size(); v++) G.addEdge(G.edgesToAdd[v]);
      G.edgesToAdd.clear();

      //cout << "Graph rebuilt --------" << endl;
      //G.printGraph();

      int cost = G.computeCost(rootPath);
      if (cost < min){
        min = cost;
        minimalPath = rootPath;
      }
    }

    // This handles the case of there being no spur paths, or no spur paths left.
    // This could happen if the spur paths have already been exhausted (added to shortestPaths),
    // or there are no spur paths at all - such as when both the source and sink vertices
    // lie along a "dead end"
    if (min == infinity)
      break;

    //Add the lowest cost path becomes the k-shortest path
    shortestPaths.push_back(minimalPath);


  }


  return shortestPaths;
}
