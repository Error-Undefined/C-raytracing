#ifndef HIT_RECORD_H
#define HIT_RECORD_H

#include "vector3.h"

struct hit_record
{
  point3 p;
  vec3 normal;
  double t;
};

typedef struct hit_record hit_record;

#endif
