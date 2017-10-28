#include <iostream>
#include <vector>
#include <limits>
#include "Graph.hpp"
#include "Dijkstra_opti_p.hpp"
#include "MinHeap.hpp"
#include "mpi.h"
#include <unistd.h>


using namespace std;

int main(int argc, char *argv[]){
  int id, p;

  Graph* g;

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
  if (id<p-1)
    size_of_heap=g->n/p;
  else
    size_of_heap=g->n-(g->n/p*(p-1));
  int start = g->n/p*id;

  DijkstraOptiP dij = DijkstraOptiP(*g, 0, g->n-1, id, p, start, size_of_heap);

  int final_length = dij.dijkstraComplexPar();;


  if (id == 0){
    cout << "length of the path :" << final_length << endl;
    cout << "The path is : ";
    for (int i = 0; i< dij.shortestPath.size(); i++)
      cout << dij.shortestPath[i] + 1 << " ";
    cout << "" << endl;
  }

}

DijkstraOptiP::DijkstraOptiP(Graph graph, int source, int dest, int id, int p, int start, int size_of_heap): \
  G(graph), s(source), t(dest), idproc(id), nbproc(p), d(new int[G.n]), visited(new bool[G.n]), \
  prec(new int[G.n]),  heap(MinHeap(start, size_of_heap, G.n, d)), nbp(size_of_heap), startp(start){
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

  changeSucc();
}

void DijkstraOptiP::changeSucc(){
  //here we get rid of useless successors according to the id of the processor
  for (int v=0; v<G.n; v++){
    vector<Arc>::iterator it = G.successors[v].begin();
    while (it != G.successors[v].end()) {
        if (it->dest<startp || it->dest>=startp+nbp)
          it = G.successors[v].erase(it);
        else{
          it++;
        }
    }
  }
}


int DijkstraOptiP::dijkstraComplexPar(){

  while(true){

    int v = heap.GetMin();

    //to deal with with parallelism
    int best_value=infinity;
    if(v!=infinity) //case where the heap is empty !
      best_value=d[v];
    int best_index=v;

    if(idproc!=0){ //each proc sends its best vertex to the root
      MPI_Send(&best_value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
      MPI_Send(&v, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    else {
      int temp_value = best_value;
      int temp_index = v;
      for(int i=1; i<nbproc;i++){
        MPI_Recv(&temp_value, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&temp_index, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (temp_value<best_value){
          best_value=temp_value;
          best_index=temp_index;
        }
      }
    }

    //broadcasting the index before the next "if section" is important
    MPI_Bcast(&best_index, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // update the values for the root and then broadcast
    int proc_with_good_value = min(best_index/(G.n/nbproc),nbproc-1);
    if (best_index/(G.n/nbproc)!=0){
      if (idproc==proc_with_good_value)
        MPI_Send(&prec[best_index], 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
      else if (idproc==0)
        MPI_Recv(&prec[best_index], 1, MPI_INT, proc_with_good_value, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    if (idproc==0)
      d[best_index]=best_value;

    MPI_Bcast(&d[best_index], 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&prec[best_index], 1, MPI_INT, 0, MPI_COMM_WORLD); //address of the proc who knows who is the prec[best_index]

    if (idproc==best_index/(G.n/nbproc)){
      heap.DeleteMin();
    }

    v = best_index;
    visited[v]=true;

    //the algorithm stops as soon as t has been visited, or when all the vertices have been visited
    if (v == t || v == -1)
      break;

    for (unsigned int k = 0; k<G.successors[v].size(); k++){
      int successor = G.successors[v][k].dest;
      int cost = G.successors[v][k].cost;
      if (!visited[successor] && d[successor] > d[v] + cost){
          d[successor] = d[v] + cost;
          prec[successor] = v;
          heap.BubbleUp(successor);
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

  return d[t];
}

vector<int> DijkstraOptiP::reverse(vector<int> vec){
  vector<int> res;
  for (unsigned int k = 0; k<vec.size(); k++) res.push_back(vec[vec.size()-1-k]);
  return res;
}
