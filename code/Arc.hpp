#ifndef ARC_H
#define ARC_H

#include <iostream>

class Arc{

public:
  int source;
  int dest;
  int cost;
  int constraint; //contraint = 0 if there is no contraint
  Arc(int i, int j, int c);
  Arc(int i, int j, int c, int c2);
};

#endif
