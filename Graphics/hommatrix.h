#ifndef _HOMMATRIX_H_
#define _HOMMATRIX_H_

#include <glib.h>
#include <math.h>

typedef float CoefficientArray[4];

typedef struct 
{
        float data[4][4];
} HomMatrix;

HomMatrix *hom_matrix_new (float m11, float m12, float m13, float m14, 
                           float m21, float m22, float m23, float m24, 
                           float m31, float m32, float m33, float m34, 
                           float m41, float m42, float m43, float m44);

void hom_matrix_free      (HomMatrix *mtx);

HomMatrix *hom_matrix_multiply (HomMatrix *mtx1, HomMatrix *mtx2);

void hom_matrix_print (HomMatrix *matx);


#endif
