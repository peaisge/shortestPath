#ifndef MINHEAP_H
#define MINHEAP_H

#include <vector>
#include "Graph.hpp"
using namespace std;

class MinHeap
{
private:


  int* _d;


public:
  vector<int> _vector; //contains the numbers of the arrays
  vector<int> _inverse; //inverse[i]= pos of the vertex i
  MinHeap(int length, int dist[]);
  MinHeap(int start, int size_of_heap, int length, int* dist);

  void BubbleUp(int index);
  //void Insert(int newValue);
  void BubbleDown(int index);
  void DeleteMin();
  void Heapify();
  int GetMin();



};

#endif
