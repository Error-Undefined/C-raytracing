#include "ray.h"

point3 ray_at(struct ray* r, double t)
{
  point3 point = {r->direction.x, r->direction.y, r->direction.z};
  vec3_mul(&point, t);
  vec3_add(&point, &r->origin);
  return point;
}
