#ifndef HIT_RECORD_H
#define HIT_RECORD_H

#include "vector3.h"
#include "hittable_type.h"

struct hit_record
{
  point3 p;
  vec3 normal;
  double t;
  material_type material;
  color albedo;
};

typedef struct hit_record hit_record;

#endif
