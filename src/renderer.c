#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>

#include "renderer.h"

#include "common_utils.h"

#include "fileout.h"
#include "vector3.h"
#include "ray.h"
#include "hittable_type.h"
#include "hit_record.h"
#include "hit.h"
#include "hittable_list.h"
#include "scatter.h"

#define RAYTRACE_INFINITY DBL_MAX

static color ray_color(hittable_list* world, ray* r, int depth)
{ 
  if (depth <= 0)
  {
    color c_d = {0,0,0};
    return c_d;
  }

  hit_record rec;
  rec.t = 0.0;

  bool hit = hit_world(world, r, 0.001, RAYTRACE_INFINITY, &rec);
  
  if (hit)
  {
    //Scatter ray
    ray scattered_ray;
    color attenuation;
    if(scatter(r, &rec, &attenuation, &scattered_ray))
    {
      color recurse_color = ray_color(world, &scattered_ray, depth - 1);
      vec3_element_mul(&recurse_color, &attenuation);
      return recurse_color;
    }

    /* ----------- */
    
    // Create target point
    // point3 target = {0, 0, 0};
    // vec3 random_dev = vec3_random_unit_vector();
    // vec3_copy_into(&target, &rec.p);
    // vec3_add(&target, &rec.normal);
    // vec3_add(&target, &random_dev);
    
    // //Create a new ray to trace onwards
    // ray trace;
    // trace.origin = rec.p;
    // vec3_copy_into(&trace.direction, &target);
    // vec3_sub(&trace.direction, &rec.p);
    // color c_recurse = ray_color(world, &trace, depth - 1);
    // vec3_mul(&c_recurse, 0.5);
    // return c_recurse;

  }

  //vec3_norm(&r->direction);
  double t = 0.5*(r->direction.y/vec3_len(&r->direction) + 1.0);
  color c1 = {0.5, 0.7, 1.0};
  color c2 = {1.0, 1.0, 1.0};

  vec3_mul(&c2, t);
  vec3_mul(&c1, (1.0 - t));
  vec3_add(&c1, &c2);
  return c1;
}

void render(int h, int w)
{
  // Seed the RNG
  srand(1);

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

  int samples_per_pixel = 20;
  int ray_depth = 20;

  // World
  sphere s1;
  point3 center1 = {0,0,1};
  vec3_copy_into(&s1.center, &center1);
  s1.radius = 0.5;
  s1.material = lambertian_material;
  s1.albedo = (color) {0.3, 0.3, 0.3};
  //vec3_copy_into(&s1.albedo, &albedo);

  sphere s2;
  point3 center2 = {0,100.5,1};
  vec3_copy_into(&s2.center, &center2);
  s2.radius = 100;
  s2.material = lambertian_material;
  s2.albedo = (color) {0.7, 0.7, 0.7};
  //vec3_copy_into(&s2.albedo, &albedo);

  sphere s3;
  point3 center3 = {2,-1,2};
  vec3_copy_into(&s3.center, &center3);
  s3.radius = 1;
  s3.material = metal_material;
  s3.albedo = (color) {0.2 , 0.2, 0.2};
  //vec3_copy_into(&s3.albedo, &albedo);

  hittable_list* world = init_hittable_list(&s1, hittable_sphere);
  add_hittable_object(world, &s2, hittable_sphere);
  add_hittable_object(world, &s3, hittable_sphere);

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
      color c = {0,0,0};
      for (int s = 0; s < samples_per_pixel; s++)
      { 
        double u = (cur_w*1.0 + random_double())/(w - 1.0);
        double v = (cur_h*1.0 + random_double())/(h - 1.0);

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

        color sample_color = ray_color(world, &r, ray_depth);
        vec3_add(&c, &sample_color);
      }
      vec3_mul(&c, 1.0/samples_per_pixel);
      //Sqrt for gamma correction
      image_buf[cur_h][cur_w].x = sqrt(c.x);
      image_buf[cur_h][cur_w].y = sqrt(c.y);
      image_buf[cur_h][cur_w].z = sqrt(c.z);
    }
  }
  int file_result = write_file("testfile.ppm", image_buf, h, w);
  printf("Rendering finished with code %d\n", file_result);
}
