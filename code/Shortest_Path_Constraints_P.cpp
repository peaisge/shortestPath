#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include "Graph.hpp"
#include "Shortest_Path_Constraints_P.hpp"
#include "mpi.h"


using namespace std;


int main(int argc, char *argv[]){

  int id, p;

  Graph* g;

  if (argc == 1){
    g = Graph::makeGraph2();
    g->max_weight=5;
  }
  else if (argc == 2)
    g = Graph::ReadGraphFromFile(argv[1]);

  // Initialize MPI.
  MPI_Init(&argc, &argv);

  // Get the number of processes.
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  // Get the individual process ID.
  MPI_Comm_rank(MPI_COMM_WORLD, &id);

  int nb_arcs=0;

  for (unsigned int v = 0; v<g->n; v++)
    nb_arcs+=g->successors[v].size();

  int size_of_heap;
  if (id<p-1)
    size_of_heap=nb_arcs/p;
  else
    size_of_heap=nb_arcs-(nb_arcs/p*(p-1));
  int start = nb_arcs/p*id;


  ShortestPathConstraintsP sh= ShortestPathConstraintsP(*g, 0, g->n-1, id, p, start, size_of_heap, nb_arcs);
  int final_length = sh.compute();

  if (id==0){
    cout << "length of the path :" << final_length << endl;
    cout << "The path is : ";
    cout << "1 ";
    for (int i=0; i< sh.shortestPath.size(); i++)
      cout << sh.shortestPath[sh.shortestPath.size()-1-i]+1 << " ";
    cout << "" << endl;
  }
}

ShortestPathConstraintsP::ShortestPathConstraintsP(Graph graph, int source, int dest, int id, int p, int start, int size_of_heap, int nb_acs): \
G(graph), s(source), t(dest), idproc(id), nbproc(p), nbp(size_of_heap), startp(start), max_weight(graph.max_weight), nb_arcs(nb_acs) {


  d = new int**[G.n];
  for(int v = 0; v < G.n; ++v) {
    d[v] = new int*[G.n];
    for(int k = 0; k < G.n; ++k) {
      d[v][k] = new int[max_weight];
      for (unsigned int w = 0; w<max_weight; w++) { //max weight
        d[v][k][w] = infinity; //tentative distances are infinite
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
  prec[s][0]=s;


  arc_array = new Arc*[nb_arcs];
  int count=0;
  for (unsigned int v = 0; v<G.n; v++){
    for (unsigned int i = 0; i<G.successors[v].size();i++)
    {
      arc_array[count]=&G.successors[v][i];
      count++;
    }
  }


}


int ShortestPathConstraintsP::compute(){


  for (int k=1;k<G.n;k++){ //nb of steps
    //Initialize the new step :
    for (int v=0;v<G.n;v++){ //for each vertex
      for (int w=0; w< max_weight; w++){ //for each weight
        d[v][k][w]=d[v][k-1][w];
      }
    }


    for (int a=startp; a<startp+nbp;a++){
      int succ = arc_array[a]->dest;
      int source = arc_array[a]->source;
      int edge_d = arc_array[a]->cost;
      int edge_w = arc_array[a]->constraint;

      //Dynamic Programming
      for (int w=0; w<max_weight-edge_w; w++){ //for each weight when new path doesn't exceed the limit
        if (d[source][k-1][w]!=infinity){ //if a path to v with weight w exists
          if (d[succ][k][w+edge_w]>d[source][k-1][w]+edge_d){
            d[succ][k][w+edge_w]=d[source][k-1][w]+edge_d;
            prec[succ][w+edge_w]=source;
          }
        }
      }
    }
    //actualization of all the values, doesn't change the complexity if G.n>p*p
    //every proc sends its value of d[v][k][w] and the root decides which is the best
    //and allows actualization of prec[v][w] via broadcasting
    for (int v=0;v<G.n;v++){
      for (int w=0; w< max_weight; w++){
        int dvkw;
        //everybody knows the new distances of all points and all weights
        MPI_Allreduce(&d[v][k][w], &dvkw, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);

        //procs send -1 if they are not responsible of the best value ,
        //prec[v][w] if they are
        if (dvkw==d[v][k][w] && idproc!=0){
          MPI_Send(&prec[v][w], 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
        else if (idproc!=0){
          int temp=-1;
          MPI_Send(&temp, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }

        //the root determines who is prec[v][w] and broadcasts it to all
        else if (idproc==0){
          int temp_value;
          for (int i=1; i<nbproc;i++){
            MPI_Recv(&temp_value, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (temp_value!=-1){
              prec[v][w]=temp_value;
            }
          }
        }
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Bcast(&prec[v][w], 1, MPI_INT, 0, MPI_COMM_WORLD);
        d[v][k][w]=dvkw;
      }
    }
  }


  //find the best path to reach t

  //first, find the final weight
  int w = 0, best_w = 0;
  for (w = 0; w<max_weight; w++){
    if (d[t][G.n-1][w] < d[t][G.n-1][best_w])
      best_w=w;
  }

  w=best_w;

  if (d[t][G.n-1][w] == infinity)
    cout << "No path found to respect the constraints" << endl;


  //creation of the path
  int count = t;
  int final_length = 0;
  if (idproc == 0){
    while (count != s){
      shortestPath.push_back(count);
      int count2 = count;
      count = prec[count][w];
      for (int i = 0; i<G.successors[count].size(); i++){
        if (G.successors[count][i].dest == count2){
          w -= G.successors[count][i].constraint;
          final_length += G.successors[count][i].cost;
          break;
        }
      }
    }
  }

  return final_length;
}
