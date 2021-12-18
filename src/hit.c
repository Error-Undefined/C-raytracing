#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "hit.h"
#include "hit_record.h"
#include "hittable_type.h"
#include "ray.h"

bool hit_world(hittable_list* list, ray* r, double t_min, double t_max, hit_record* rec)
{
  hit_record temp_record;
  bool hit_anything = false;
  double closest = t_max;

  hittable_list_node* node = get_next_node(list);
  while(node != NULL)
  {
    if (node->object_type == hittable_sphere)
    {
      sphere* obj = (sphere*) node->object;
      if(hit_sphere(obj, r, t_min, closest, &temp_record))
      {
        hit_anything = true;
        closest = temp_record.t;
        rec->normal = temp_record.normal;
        rec->p = temp_record.p;
        rec->t = temp_record.t;
        rec->material = obj->material;
        rec->albedo = obj->albedo;
      }
    }
    node = get_next_node(list);
  }
  reset_current_list_node(list);
  return hit_anything;
}

bool hit_sphere(sphere* s, ray* r, double t_min, double t_max, hit_record* rec)
{
  vec3 oc = vec3_sub_new(&r->origin, &s->center);
  double a = vec3_dot(&r->direction, &r->direction);
  double b = 2.0 * vec3_dot(&oc, &r->direction);
  double c = vec3_dot(&oc, &oc) - s->radius*s->radius;
  double discriminant = b*b - 4*a*c;
  if (discriminant < 0)
  {
    return false;
  }
  // Try and see if any of the two roots are in the acceptable range
  double sqrtd = sqrt(discriminant);
  double root = (-b - sqrtd)/(2.0*a);
  if (root < t_min || root > t_max)
  {
    root = (-b + sqrtd)/(2.0*a);
    if (root < t_min || root > t_max)
    {
      return false;
    }
  }

  rec->t = root;
  rec->p = ray_at(r, rec->t);
  // normal = (touch_point - center)/radius
  rec->normal = s->center;
  vec3_mul(&rec->normal, -1.0);
  vec3_add(&rec->normal, &rec->p);
  vec3_mul(&rec->normal, 1.0/s->radius);

  return true;
}
