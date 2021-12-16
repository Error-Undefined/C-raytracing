#ifndef HIT_H
#define HIT_H

#include <stdbool.h>

#include "ray.h"

bool hit_sphere(ray* r, double t_min, double t_max, hit_record* rec);

#endif
