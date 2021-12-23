#ifndef CAMERA_H
#define CAMERA_H

#include "src/vector3.h"

struct camera
{
  int h, w;
  double plane_width, plane_height;
  double focal_length;
  vec3 camera_center;
};

#endif
