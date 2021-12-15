#ifndef VECTOR3_H
#define VECTOR3_H

// The main vector3 struct
struct vector3
{
  double x, y, z;
};

typedef struct vector3 point3;
typedef struct vector3 color;

struct vector3 *copy_vector3(struct vector3* v);

// Vector3 functions

void vec3_add(struct vector3 *u, struct vector3 *other);
void vec3_mul(struct vector3 *u, double t);
void vec3_cross(struct vector3 *u, struct vector3 *other);

double vec3_len(struct vector3 *u);
double vec3_dot(struct vector3 *u, struct vector3 *v);

void vec3_norm(struct vector3 *u);

#endif //VECTOR3_H

