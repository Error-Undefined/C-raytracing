#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

#include <stdlib.h>

static inline double random_double()
{
  double d = rand() / (RAND_MAX - 1.0);
  return d;
}

static inline double random_range_double(double min, double max)
{
  return min + (max - min)*random_double();
}

#endif
