#ifndef HITTABLE_TYPE_H
#define HITTABLE_TYPE_H

#include "vector3.h"

enum hittable_type_enum
{
  hittable_sphere = 0x1
};

enum material_type_enum
{
  lambertian_material = 0x1,
  metal_material = 0x2,
  dielectric_material = 0x3
};

typedef enum hittable_type_enum hittable_type;
typedef enum material_type_enum material_type;

struct sphere
{
  color albedo;
  point3 center;
  double radius;
  double fuzz_or_refraction;
  enum material_type_enum material;
};
typedef struct sphere sphere;

#endif
