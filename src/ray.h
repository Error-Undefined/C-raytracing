#ifndef RAY_H
#define RAY_H

#include "vector3.h"

struct ray
{
  point3 origin;
  vec3 direction;
};

point3 ray_at(struct ray* r, double t);

typedef struct ray ray;

#endif
