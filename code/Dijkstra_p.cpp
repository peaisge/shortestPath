#include <iostream>
#include <vector>
#include "Dijkstra_p.hpp"
#include "mpi.h"

using namespace std;

int main(int argc, char *argv[]) {
  int id, p;
  Graph* g = Graph::makeGraph1();
  const int root = 0;

  if (argc == 1)
    g = Graph::makeGraph2();
  else if (argc == 2)
    g = Graph::ReadGraphFromFile(argv[1]);

  // Initialize MPI.
  MPI_Init(&argc, &argv);
  // Get the number of processes.
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  // Get the individual process ID.
  MPI_Comm_rank(MPI_COMM_WORLD, &id);

  int size_of_heap;
  if (id < p-1)
    size_of_heap=g->n/p;
  else
    size_of_heap=g->n-(g->n/p*(p-1));
  int start = g->n/p*id;

  Dijkstra_p dij= Dijkstra_p(*g, 0, 3, id, p, start, size_of_heap);
  dij.dijkstraNaifPar();

  if (id == 0){
    cout << "The path is : ";
    for (int i = 0; i < dij.shortestPath.size(); i++)
      cout << dij.shortestPath[i] << " ";
    cout << "" << endl;
  }
  return 0;
}

Dijkstra_p::Dijkstra_p(Graph graph, int source, int dest, int id, int p, int start, int size_of_heap): \
  //Initialization
  G(graph), s(source), t(dest), d(new int[G.n]), visited(new bool[G.n]), prec(new int[G.n]), \
  idproc(id), nbproc(p), startp(start), nbp(size_of_heap){

  //tentative distances are infinite
  for (unsigned int k = 0; k<G.n; k++) {
    d[k] = infinity;
  }
  d[s] = 0;
  // tentative parents are not found yet
  for (unsigned int k = 0; k<G.n; k++) {
    prec[k] = -1;
  }
  prec[s] = s;
  //optimization : store tentative distances and tentative parents of the source's successors
  for (unsigned int k = 0; k<G.successors[s].size(); k++){
    int successor = G.successors[s][k].dest;
    d[successor] = G.successors[s][k].cost;
    prec[successor] = s;
  }
  //only the source has been visited
  for (unsigned int k = 0; k<G.n; k++) {
    visited[k] = false;
  }
  visited[s] = true;

  //consider only the relevant children for each core
  changeSucc();
}

void Dijkstra_p::changeSucc(){
  //here we get rid of useless successors according to the id of the processor

  for (int v = 0; v<G.n; v++){
    vector<Arc>::iterator it = G.successors[v].begin();
    while (it != G.successors[v].end()) {
        if (it->dest < startp || it->dest >= startp + nbp)
          it = G.successors[v].erase(it);
        else
          it++;
    }
  }
}

int Dijkstra_p::distanceMin(){
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

int Dijkstra_p::dijkstraNaifPar(){

  const int root = 0;

  while(true){
    int v = distanceMin();
    int shortestDist, closestVertex;
    closestVertex = v;
    if (v == -1)
      shortestDist = infinity;
    else
      shortestDist = d[v];

    if (idproc > 0){
      MPI_Send(&shortestDist, 1, MPI_INT, root, 0, MPI_COMM_WORLD);
      MPI_Send(&closestVertex, 1, MPI_INT, root, 0, MPI_COMM_WORLD);
    }

    else{
      int temp_value = shortestDist;
      int temp_index = closestVertex;
      for (unsigned int k = 1; k<nbproc; k++){
        MPI_Recv(&temp_value, 1, MPI_INT, k, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&temp_index, 1, MPI_INT, k, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (temp_value < shortestDist){
          shortestDist = temp_value;
          closestVertex = temp_index;
        }
      }
    }

    //broadcasting before the next if section is important
    MPI_Bcast(&closestVertex, 1, MPI_INT, 0, MPI_COMM_WORLD);

    v = closestVertex;
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
  shortestPath = reverse(shortestPath);
  int res = d[t];

  //delete the arrays
  delete[] d;
  delete[] visited;
  delete[] prec;

  return res;
}

vector<int> Dijkstra_p::reverse(vector<int> vec){
  vector<int> res;
  for (unsigned int k = 0; k<vec.size(); k++) res.push_back(vec[vec.size()-1-k]);
  return res;
}
