#ifndef CAMERA_H
#define CAMERA_H

#include "src/vector3.h"

struct camera
{
  double focal_length;
  vec3 camera_center;
  vec3 view_dir;
};

#endif
