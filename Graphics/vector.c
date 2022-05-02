#include "vector.h"

Vector3D
vector_projection (Vector3D vector, Vector3D on)
{
        Vector3D result;
        float divisor = on.x * on.x + on.y * on.y + on.z * on.z;

        result.x = on.x * ((vector.x * on.x) / divisor);
        result.y = on.y * ((vector.y * on.y) / divisor);
        result.z = on.z * ((vector.z * on.z) / divisor);

        return result;
}

Vector3D
vector_mirror (Vector3D ray, Vector3D surface_normal)
{
        Vector3D proj;
        Vector3D result;

        proj =  vector_projection (ray, surface_normal);

        result.x = ray.x - 2.0 * proj.x;
        result.y = ray.y - 2.0 * proj.y;
        result.x = ray.z - 2.0 * proj.z;

        return result;
}

// probably inline
double 
vector_dot (Vector3D v1, Vector3D v2)
{
        return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

// The cross product gives you a vector perpendiculat to the given vectors
// probably inline
Vector3D
vector_cross (Vector3D v1, Vector3D v2)
{
        Vector3D result;

        result.x = (v1.y * v2.z) - (v2.y * v1.z);
        result.y = (v1.z * v2.x) - (v2.z * v1.x);
        result.z = (v1.x * v2.y) - (v2.x * v1.y);

        return result;
}

// This makes a vector length 1
// probably inline
void 
vector_normalize (Vector3D *n)
{
        double length;
        
        length = sqrt (n->x * n->x + n->y * n->y + n->z * n->z);
 
        // avoid division by zero
        if (length == 0) 
                return;
        
        n->x /= length;
        n->y /= length;
        n->z /= length;
}
