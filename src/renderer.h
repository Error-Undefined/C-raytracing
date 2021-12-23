#ifndef RENDERER_H
#define RENDERER_H

#include "vector3.h"

struct camera
{
  int h, w;
  double plane_width, plane_height;
  double focal_length;
  vec3 camera_center;
};

//Entry point to the renderer
void render(int h, int w);

#endif
