#ifndef RAY_H
#define RAY_H

#include "vector3.h"

struct ray
{
  point3 origin;
  vector3 direction;
};

point3 at(struct ray* r, double t)
{
  point3 point{r->origin.x, r->origin.y, r->origin.z};
  vec3_mul(&point, t);
  vec3_add(&point, &r->origin);
  return point;
}


#endif
