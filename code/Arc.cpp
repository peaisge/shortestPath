#include <iostream>
#include "Arc.hpp"

Arc::Arc(int i, int j, int c):source(i),dest(j),cost(c),constraint(0){}

Arc::Arc(int i, int j, int c, int c2):source(i),dest(j),cost(c),constraint(c2){}
