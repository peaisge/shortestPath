#include "MinHeap.hpp"

// Re-adapted from https://www.codeproject.com/Tips/816934/Min-Binary-Heap-Implementation-in-Cplusplus

MinHeap::MinHeap(int length, int* dist) : _vector(length,0), _inverse(length,0)
{

  _d = dist;
  for(int i = 0; i < length; ++i)
  {
      _vector[i] = i;
      _inverse[i] = i;
  }

  //Heapify();
}

MinHeap::MinHeap(int start, int size_of_heap, int length, int* dist) : _vector(size_of_heap,0), _inverse(length,0)
{

  _d = dist;
  for(int i = 0; i < size_of_heap; ++i)
  {
      _vector[i] = start+i;
      _inverse[start+i] = i;
  }
}

void MinHeap::Heapify()
{
    int length = _vector.size();
    for(int i=length-1; i>=0; --i)
    {
        BubbleDown(_vector[i]); //because bubbledown normally takes the number of the vertex, not its position
    }
}

void MinHeap::BubbleDown(int v) // v is the number of the vertex
{
    int index = _inverse[v];
    int length = _vector.size();
    int leftChildIndex = 2*index + 1;
    int rightChildIndex = 2*index + 2;

    if(leftChildIndex >= length)
        return; //index is a leaf

    int minIndex = index;



    if(_d[_vector[index]] > _d[_vector[leftChildIndex]])
    {
        if (v==0)
          cout << "ouch" << _vector[index] << " " <<  _d[_vector[index]] << endl;
        minIndex = leftChildIndex;
    }

    if((rightChildIndex < length) && (_d[_vector[minIndex]] > _d[_vector[rightChildIndex]]))
    {
        minIndex = rightChildIndex;
    }

    if(minIndex != index)
    {
        //need to swap
        int temp = _vector[index];
        _vector[index] = _vector[minIndex];
        _vector[minIndex] = temp;
        _inverse[_vector[index]]=index;
        _inverse[_vector[minIndex]]=minIndex;

        BubbleDown(_vector[minIndex]);
    }
}


void MinHeap::BubbleUp(int v)
{
    int index = _inverse[v];

    if(index == 0)
        return;

    int parentIndex = (index-1)/2;

    if(_d[_vector[parentIndex]] > _d[_vector[index]])
    {
        int temp = _vector[parentIndex];
        _vector[parentIndex] = _vector[index];
        _vector[index] = temp;
        _inverse[_vector[index]]=index;
        _inverse[_vector[parentIndex]]=parentIndex;

        BubbleUp(_vector[parentIndex]);
    }
}


int MinHeap::GetMin()
// return the number of the closest vertex
{
    if (_vector.size()>0)
      return _vector[0];
    else return infinity;
}

void MinHeap::DeleteMin()
{
    int length = _vector.size();
    if(length == 0)
    {
        return;
    }

    _inverse[_vector[0]]=-1; //the vertex number _vector[0] is not in the tree anymore
    _inverse[_vector[length-1]] = 0;
    _vector[0] = _vector[length-1];
    _vector.pop_back();

    BubbleDown(_vector[0]);
}
