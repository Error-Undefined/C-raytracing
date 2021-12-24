#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>

#include "renderer.h"

#include "../camera.h"

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

static void make_triangle_norm(triangle* t)
{
  vec3 edge0, edge1;
  vec3_copy_into(&edge0, &t->vertex1);
  vec3_copy_into(&edge1, &t->vertex2);
  vec3_sub(&edge0, &t->vertex0);
  vec3_sub(&edge1, &t->vertex0);
  
  t->normal = vec3_cross_new(&edge0, &edge1);
  vec3_norm(&t->normal);
}

static color ray_color(hittable_list* world, ray* r, int depth)
{ 
  if (depth <= 0)
  {
    color c_d = {0,0,0};
    return c_d;
  }

  hit_record rec;
  rec.t = 0.0;

  bool hit = hit_world(world, r, 0.0001, RAYTRACE_INFINITY, &rec);
  
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

void render(int h, int w, struct camera* camera)
{
  // Seed the RNG
  srand(1);

  //Camera: we define the camera at (0, 0, 0)
  //With the camera axis along the positive Z axis
  //The image plane has a height of 2 and a width according to the aspect ratio
  
  //Image plane
  double img_plane_height = 2.0;
  double img_plane_width = 2.0*w/h;
  double cam_focal_length = 1;

  point3 camera_center = {0, 0, 0};
  vec3 center_distance = {0, 0, cam_focal_length};
  vec3 horizontal = {img_plane_width, 0, 0};
  vec3 vertical = {0, img_plane_height, 0};
  vec3 upper_left = {0, 0, 0};
  vec3 acc = {0, 0, 0};

  if (camera != NULL)
  {
    cam_focal_length = camera->focal_length;
    center_distance = (vec3) {0, 0, cam_focal_length};
    vec3_copy_into(&camera_center, &camera->camera_center);
    vec3_norm(&camera->view_dir);
    vec3_norm(&camera->camera_up);

    horizontal = vec3_cross_new(&camera->view_dir, &camera->camera_up);
    vec3_mul(&horizontal, img_plane_width);

    vec3_copy_into(&vertical, &camera->camera_up);
    vec3_mul(&vertical, -1*img_plane_height);
  }
  
  vec3_copy_into(&acc, &horizontal);
  vec3_add(&acc, &vertical);
  vec3_mul(&acc, 0.5);
  vec3_sub(&acc, &center_distance);
  vec3_sub(&upper_left, &acc);

  int samples_per_pixel = 100;
  int ray_depth = 50;

  /*
  // World
  sphere s1;
  point3 center1 = {0,0,4};
  vec3_copy_into(&s1.center, &center1);
  s1.radius = 0.5;
  s1.material = lambertian_material;
  s1.albedo = (color) {0.1, 0.9, 0.2};
  s1.fuzz_or_refraction = 1.5;

  sphere s2;
  point3 center2 = {0,100.5,1};
  vec3_copy_into(&s2.center, &center2);
  s2.radius = 100;
  s2.material = lambertian_material;
  s2.albedo = (color) {0.5, 0.5, 0.5};

  sphere s3;
  point3 center3 = {1,0,1};
  vec3_copy_into(&s3.center, &center3);
  s3.radius = 0.5;
  s3.material = lambertian_material;
  s3.albedo = (color) {1 , 0.5, 0.5};
  s3.fuzz_or_refraction = 0.2;

  sphere s4;
  point3 center4 = {0,0,1};
  vec3_copy_into(&s4.center, &center4);
  s4.radius = 0.4;
  s4.material = dielectric_material;
  s4.albedo = (color) {1,1,1};
  s4.fuzz_or_refraction = 1.8;

  sphere s5;
  point3 center5 = {-1,0,1};
  vec3_copy_into(&s5.center, &center5);
  s5.radius = 0.5;
  s5.material = metal_material;
  s5.albedo = (color) {1,1,0};
  s5.fuzz_or_refraction = 0.9;

  sphere s6;
  point3 center6 = {-1,-1,-3};
  vec3_copy_into(&s6.center, &center6);
  s6.radius = 1;
  s6.material = lambertian_material;
  s6.albedo = (color) {0.1, 0.1, 0.95};
  s6.fuzz_or_refraction = 0.1;

  triangle t1;
  t1.vertex0 = (point3) {-1.5, -1.5, 1.5};
  t1.vertex1 = (point3) {1.5, 0, 1.3};
  t1.vertex2 = (point3) {-1.5, 0, 1.3};
  make_triangle_norm(&t1);
  t1.albedo = (color) {0.5, 0.5, 0.8};
  t1.fuzz_or_refraction = 0.1;
  t1.material = lambertian_material;

  triangle t2;
  t2.vertex0 = (point3) {-1.5, -1.5, 1.5};
  t2.vertex1 = (point3) {1.5, -1.5, 1.5};
  t2.vertex2 = (point3) {1.5, 0, 1.3};
  make_triangle_norm(&t2);
  t2.albedo = (color) {0.5, 0.5, 0.8};
  t2.fuzz_or_refraction = 0.1;
  t2.material = lambertian_material;

  hittable_list* world = init_hittable_list(&s2, hittable_sphere);
  add_hittable_object(world, &s1, hittable_sphere);
  add_hittable_object(world, &s3, hittable_sphere);
  add_hittable_object(world, &s4, hittable_sphere);
  add_hittable_object(world, &s5, hittable_sphere);
  // add_hittable_object(world, &t1, hittable_triangle);
  // add_hittable_object(world, &t2, hittable_triangle);
  add_hittable_object(world, &s6, hittable_sphere);

  */

  sphere ground_sphere;
  ground_sphere.center = (vec3) {0, 1000, 0};
  ground_sphere.radius = 1000;
  ground_sphere.material = lambertian_material;
  ground_sphere.albedo = (vec3) {0.5, 0.5, 0.5};
  ground_sphere.fuzz_or_refraction = 1;

  hittable_list* world_random = init_hittable_list(&ground_sphere, hittable_sphere);

  sphere sphere_list[22*22];
  int spere_list_it = 0;
  for(int a = -11; a < 11; a++)
  {
    for(int b = -11; b < 11; b++)
    {
      double choose_mat = random_double();
      point3 center = {a + 0.9*random_double(), -0.2, b + 0.9*random_double()};
      point3 other_c = {4,0.2,0};
      vec3 condition;
      vec3_copy_into(&condition, &center);
      vec3_sub(&condition, &other_c);

      if(vec3_dot(&condition,&condition) > 0.81)
      {
        sphere* sphere_pointer = &sphere_list[spere_list_it];
        vec3_copy_into(&sphere_pointer->center, &center);
        sphere_pointer->albedo = (vec3) {random_double(),random_double(),random_double()};
        sphere_pointer->radius = random_range_double(0.18,0.2); 
        if (choose_mat < 0.8)
        {
          // Lambertian
          sphere_pointer->material = lambertian_material;
          sphere_pointer->fuzz_or_refraction = 1;
        }
        else if (choose_mat < 0.95)
        {
          // Metal
          sphere_pointer->material = metal_material;
          sphere_pointer->fuzz_or_refraction = random_range_double(0.1, 0.7);
        }
        else
        {
          // Dielectric
          sphere_pointer->material = dielectric_material;
          sphere_pointer->fuzz_or_refraction = random_range_double(1.1, 1.7);
        }
        add_hittable_object(world_random, sphere_pointer, hittable_sphere);
        spere_list_it++;
      }
    }
  }

  sphere example1;
  example1.material = lambertian_material;
  example1.albedo = (vec3) {0.4, 0.2, 0.1};
  example1.center = (vec3) {-4, -1, 0};
  example1.radius = 1;
  example1.fuzz_or_refraction = 1;

  sphere example2;
  example2.material = metal_material;
  example2.albedo = (vec3) {0.7, 0.6, 0.5};
  example2.center = (vec3) {0, -1, 0};
  example2.radius = 1;
  example2.fuzz_or_refraction = 0.1;

  sphere example3;
  example3.material = dielectric_material;
  example3.albedo = (vec3) {0.4, 0.2, 0.1};
  example3.center = (vec3) {4, -1, 0};
  example3.radius = 1;
  example3.fuzz_or_refraction = 1.5;

  add_hittable_object(world_random, &example1, hittable_sphere);
  add_hittable_object(world_random, &example2, hittable_sphere);
  add_hittable_object(world_random, &example3, hittable_sphere);

  // Allocate the image buffer
  color** image_buf = calloc(h, sizeof(color*));
  for (int i = 0; i < h; i++)
  {
    image_buf[i] = calloc(w, sizeof(color));
  }

  //Render loop
  for(int cur_h = 0; cur_h < h; cur_h++)
  {
    double progress = cur_h * 1.0 / h * 100;
    printf("Progress: %.2f%%\n", progress);
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

        color sample_color = ray_color(world_random, &r, ray_depth);
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

  for (int i = 0; i < h; i++)
  {
    free(image_buf[i]);
  }
  free(image_buf);
}
