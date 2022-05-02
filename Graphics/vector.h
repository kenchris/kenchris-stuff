#ifndef _VECTOR_H
#define _VECTOR_H

typedef struct
{
        double x, y, z;
} Vector3D;

Vector3D vector_projection (Vector3D vector, Vector3D on);
Vector3D vector_mirror (Vector3D ray, Vector3D surface_normal);
double vector_dot (Vector3D v1, Vector3D v2);
Vector3D vector_cross (Vector3D v1, Vector3D v2);
void vector_normalize (Vector3D *n);

#endif /* _VECTOR_H */
