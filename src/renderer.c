#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "renderer.h"
#include "fileout.h"
#include "vector3.h"
#include "ray.h"

bool hit_sphere(point3* center, double radius, ray* r)
{
  vec3 oc = vec3_sub_new(&r->origin, center);
  double a = vec3_dot(&r->direction, &r->direction);
  double b = 2.0 * vec3_dot(&oc, &r->direction);
  double c = vec3_dot(&oc, &oc) - radius*radius;
  double discriminant = b*b - 4*a*c;
  return (discriminant > 0);
}

static color ray_color(ray* r)
{ 
  point3 sphere_center = {-3,0,2};
  double radius = 0.7;
  if (hit_sphere(&sphere_center, radius, r))
  {
    color c = {0.0, 0.0, 1.0};
    return c;
  }

  //vec3_norm(&r->direction);
  double t = 0.5*(r->direction.y/vec3_len(&r->direction) + 1.0);

  color c1 = {0.5, 1.0, 0.7};
  color c2 = {1.0, 1.0, 1.0};

  vec3_mul(&c2, t);
  vec3_mul(&c1, (1.0 - t));
  vec3_add(&c1, &c2);
  return c1;
}

void render(int h, int w)
{
  //Camera: we define the camera at (0, 0, 0)
  //With the camera axis along the positive Z axis
  //The image plane has a height of 2 and a width according to the aspect ratio

  //Image plane
  double img_plane_height = 2.0;
  double img_plane_width = 2.0*w/h;
  double focal_length = 1.0;

  //Camera geometry
  point3 camera_center = {0, 0, 0};
  vec3 center_distance = {0, 0, focal_length};
  vec3 horizontal = {img_plane_width, 0, 0};
  vec3 vertical = {0, img_plane_height, 0};
  vec3 upper_left = {0, 0, 0};
  vec3 acc = {0, 0, 0};

  vec3_copy_into(&acc, &horizontal);
  vec3_add(&acc, &vertical);
  vec3_mul(&acc, 0.5);
  vec3_sub(&acc, &center_distance);
  vec3_sub(&upper_left, &acc);

  // Allocate the image buffer
  color** image_buf = calloc(h, sizeof(color*));
  for (int i = 0; i < w; i++)
  {
    image_buf[i] = calloc(w, sizeof(color));
  }

  //Render loop
  for(int cur_h = 0; cur_h < h; cur_h++)
  {
    for (int cur_w = 0; cur_w < w; cur_w++)
    {
      double u = cur_w*1.0/(w - 1.0);
      double v = cur_h*1.0/(h - 1.0);

      ray r;
      //Origin = camera center
      vec3_copy_into(&r.origin, &camera_center);
      //r.dir = lower left
      vec3_copy_into(&r.direction, &upper_left);
      //r.dir += u*horizontal
      vec3_copy_into(&acc, &horizontal);
      vec3_mul(&acc, u);
      vec3_add(&r.direction, &acc);
      //r.dir += v*vertical
      vec3_copy_into(&acc, &vertical);
      vec3_mul(&acc, v);
      vec3_add(&r.direction, &acc);

      color c = ray_color(&r);

      image_buf[cur_h][cur_w].x = c.x;
      image_buf[cur_h][cur_w].y = c.y;
      image_buf[cur_h][cur_w].z = c.z;
    }
  }
  int file_result = write_file("testfile.ppm", image_buf, h, w);
  printf("Rendering finished with code %d\n", file_result);
}
