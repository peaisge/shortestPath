#include <iostream>
#include <vector>
#include <string>
#include "Arc.hpp"
#include "Graph.hpp"
#include <fstream>
#include <cstdlib>

using namespace std;

Graph::Graph():n(0),m(0)
{}

Graph::Graph(int nb, int mb):n(nb),m(mb)
{}

Graph* Graph::makeGraph1(){
  Graph* g = new Graph(4, 5);

  vector<Arc> ve0 = vector<Arc>();
  ve0.push_back(Arc(0,1,1));
  ve0.push_back(Arc(0,2,50));

  vector<Arc> ve1 = vector<Arc>();
  ve1.push_back(Arc(1,2,100));
  ve1.push_back(Arc(1,3,50));

  vector<Arc> ve2 = vector<Arc>();
  ve2.push_back(Arc(2,3,1));

  vector<Arc> ve3 = vector<Arc>();

  g->successors.push_back(ve0);
  g->successors.push_back(ve1);
  g->successors.push_back(ve2);
  g->successors.push_back(ve3);

  return g;
}

Graph* Graph::makeGraph2(){
  Graph* g = new Graph(4, 5);

  vector<Arc> ve0 = vector<Arc>();
  ve0.push_back(Arc(0,1,1,4));
  ve0.push_back(Arc(0,2,50,1));

  vector<Arc> ve1 = vector<Arc>();
  ve1.push_back(Arc(1,2,1,5));
  ve1.push_back(Arc(1,3,50,1));

  vector<Arc> ve2 = vector<Arc>();
  ve2.push_back(Arc(2,3,1,2));

  vector<Arc> ve3 = vector<Arc>();

  g->successors.push_back(ve0);
  g->successors.push_back(ve1);
  g->successors.push_back(ve2);
  g->successors.push_back(ve3);

  return g;
}

Graph* Graph::ReadGraphFromFile(char* name){

  ifstream myfile;
  myfile.open(name);
  int n;
  int m;
  Graph* g;
  if(myfile){
    myfile >> n;
    myfile >> m;
    g = new Graph(n,m);
    for (int i=0;i<n;i++){
      g->successors.push_back(vector<Arc>());
    }
    myfile >> n; //useless -> 1 resource
    myfile >> n; //again useless -> lim inf = 0
    myfile >> g->max_weight;
    for (int i=0;i<g->n;i++){
      myfile >> n;
    }
    for (int i=0;i<g->m+1;i++){
      int source;
      int dest;
      int cost;
      int constraint;

      myfile >> source >> dest >> cost >> constraint;
      if (i!=0)
        g->successors[source-1].push_back(Arc(source-1,dest-1,cost,constraint));
    }
    myfile.close();
  }
    else{
      cout << "Impossible d'ouvrir le fichier !" << endl;
      exit(EXIT_FAILURE);
    }
    return g;
}

void Graph::printGraph(){
  cout << "PRINT GRAPH ------" << endl;
  cout << "n = " << n << endl;
  cout << "m = " << m << endl;
  for (unsigned int k = 0; k<successors.size(); k++){
    cout << "Neighbors of "<< k << " : ";
    if (successors[k].size() == 0) cout << "None";
    else{
      for (unsigned int l = 0; l<successors[k].size(); l++) cout << successors[k][l].dest << " ";
    }
    cout << endl;
  }
}

Arc Graph::getEdge(int i, int j){
  //suppose that the edge exists
  for (unsigned int k = 0; k<successors[i].size(); k++){
    if (successors[i][k].dest == j) return successors[i][k];
  }
}

int Graph::computeCost(vector<int> path){
  int res = 0;
  for (unsigned int k = 0; k<path.size()-1; k++)
    res += getEdge(path[k], path[k+1]).cost;
  return res;
}

int Graph::computeCost_table(int* path, int size){
  int res = 0;
  for (unsigned int k = 0; k<size-1; k++)
    res += getEdge(path[k], path[k+1]).cost;
  return res;
}

void Graph::removeEdge(int i, int j){
  //remove from successors
  for (unsigned int k = 0; k<successors[i].size(); k++){
    if (successors[i][k].dest == j){
      edgesToAdd.push_back(successors[i][k]);
      successors[i].erase(successors[i].begin()+k);
      m--;
    }
  }
}

void Graph::addEdge(Arc edge){
  //suppose that it does not exist
  successors[edge.source].push_back(edge);
  m++;
}

void Graph::removeNode(int i){
  for (unsigned int k = 0; k<successors.size(); k++)
    removeEdge(k, i);
  m = m - successors[i].size();
  for (unsigned int k = 0; k<successors[i].size(); k++)
    edgesToAdd.push_back(successors[i][k]);
  successors[i].clear();
}

void Graph::addErasedNode(int i){
  vector<Arc> lastNodeSuccessors = successors[successors.size()-1];
  for (unsigned int k = successors.size()-1; k>i; k--)
    successors[k] = successors[k-1];
  successors[i].clear();
  successors.push_back(lastNodeSuccessors);
  n++;
}
