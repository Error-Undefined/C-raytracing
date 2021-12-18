

#include "scatter.h"

static bool scatter_lambertian(ray* r_in, hit_record* rec, color* attenuation, ray* r_scattered)
{
  vec3 scatter_direction = vec3_random_unit_vector();
  vec3_add(&scatter_direction, &rec->normal);
  if (vec3_near_zero(&scatter_direction) != 0)
  {
    //vec3_copy_into(&scatter_direction, &rec->normal);
  }
  vec3_copy_into(&r_scattered->origin, &rec->p);
  vec3_copy_into(&r_scattered->direction, &scatter_direction);

  vec3_copy_into(attenuation, &rec->albedo);
  return true;
}

static bool scatter_metal(ray* r_in, hit_record* rec, color* attenuation, ray* r_scattered)
{
  //Reflect the ray
  //v_out = v -2*(v.n)
  vec3 scatter_dir = {0, 0, 0};
  vec3_copy_into(&scatter_dir, &rec->normal);
  vec3_mul(&scatter_dir, -2 * vec3_dot(&r_in->direction, &rec->normal));
  vec3_add(&scatter_dir, &r_in->direction);

  vec3_copy_into(&r_scattered->origin, &rec->p);
  vec3_copy_into(&r_scattered->direction, &scatter_dir);

  vec3_copy_into(attenuation, &rec->albedo);

  return vec3_dot(&scatter_dir, &rec->normal) > 0;
}

static bool scatter_dielectric(ray* r_in, hit_record* rec, color* attenuation, ray* r_scattered)
{
  return false;
}

bool scatter(ray* r_in, hit_record* rec, color* attenuation, ray* r_scattered)
{
  bool scattered = false;
  switch (rec->material)
  {
    case lambertian_material:
    {
      scattered = scatter_lambertian(r_in, rec, attenuation, r_scattered);
      break;
    }
    case metal_material:
    {
      scattered = scatter_metal(r_in, rec, attenuation, r_scattered);
      break;
    }
    case dielectric_material:
    {
      scattered = scatter_dielectric(r_in, rec, attenuation, r_scattered);
      break;
    }
    default:
      return false;
  }
  return scattered;
}
