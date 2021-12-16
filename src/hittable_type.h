#ifndef HITTABLE_TYPE_H
#define HITTABLE_TYPE_H

#include "vector3.h"

struct sphere
{
  point3 center;
  double radius;
};
typedef struct sphere sphere;

enum hittable_type_enum
{
  hittable_sphere = 0x1
};

typedef enum hittable_type_enum hittable_type;

#endif
