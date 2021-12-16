#ifndef HITTABLE_TYPE_H
#define HITTABLE_TYPE_H

#include "vector3.h"

struct sphere
{
  point3 center;
  double radius;
  material_type material;
};
typedef struct sphere sphere;

enum hittable_type_enum
{
  hittable_sphere = 0x1
};

enum material_type_enum
{
  lambertian_material = 0x1,
  metal_material = 0x2
};

typedef enum hittable_type_enum hittable_type;
typedef enum material_type_enum material_type;

#endif
