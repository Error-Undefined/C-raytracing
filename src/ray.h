#ifndef RAY_H
#define RAY_H

#include "vector3.h"

struct ray
{
  point3 origin;
  vec3 direction;
};

typedef struct ray ray;

point3 ray_at(struct ray* r, double t)
{
  point3 point = {r->direction.x, r->direction.y, r->direction.z};
  vec3_mul(&point, t);
  vec3_add(&point, &r->origin);
  return point;
}


#endif
