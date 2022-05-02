#include "hommatrix.h"

void
hom_matrix_print (HomMatrix *matx);

HomMatrix *
hom_matrix_new (float m11, float m12, float m13, float m14, 
                float m21, float m22, float m23, float m24, 
                float m31, float m32, float m33, float m34, 
                float m41, float m42, float m43, float m44)
{
        HomMatrix *mtx;
        mtx = g_new (HomMatrix, 1);

        mtx->data[0][0] = m11;
        mtx->data[0][1] = m12;
        mtx->data[0][2] = m13;
        mtx->data[0][3] = m14;

        mtx->data[1][0] = m21;
        mtx->data[1][1] = m22;
        mtx->data[1][2] = m23;
        mtx->data[1][3] = m24;

        mtx->data[2][0] = m31;
        mtx->data[2][1] = m32;
        mtx->data[2][2] = m33;
        mtx->data[2][3] = m34;

        mtx->data[3][0] = m41;
        mtx->data[3][1] = m42;
        mtx->data[3][2] = m43;
        mtx->data[3][3] = m44;      

        return mtx;
}

void
hom_matrix_free (HomMatrix *mtx)
{
        g_return_if_fail (mtx != NULL);

        g_free (mtx);
}

void
hom_matrix_print (HomMatrix *matx)
{
        int i, j;
        
        for (i = 0; i < 4; i++)
        {
                printf ("|");

                for (j = 0; j < 4; j++)
                {
                        printf ("%8.f", matx->data[i][j]);
                }

                printf ("  |\n");
        }        
}


HomMatrix *
hom_matrix_multiply (HomMatrix *mtx1, HomMatrix *mtx2)
{
        HomMatrix *result = g_new (HomMatrix, 1);
        int i, j, k;

        for (j = 0; j < 4 /* row */; j++)
        {
                for (i = 0; i < 4 /* columns */; i++)
                {
                        result->data[j][i] = 0;
                        for(k = 0; k < 4; k++)
                        {
                                result->data[j][i] +=
                                        mtx1->data[j][k] * 
                                        mtx2->data[k][i];
                        }
                }
        }

        return result;
}

HomMatrix *
get_matrix_RZ (int theta)
{
        HomMatrix *R;

        theta /= 360;
        theta *= 2 * G_PI;

        R  = hom_matrix_new (cos (theta), 
                             - sin (theta), 0, 0, 
                             sin (theta),
                             cos (theta), 0, 0,
                             0, 0, 1, 0,
                             0, 0, 0, 1);

        return R;
}


int
main_1 ()
{
        HomMatrix *A, *B, *Id, *result, *R;
        
        A  = hom_matrix_new (4, 7, 2, 5,
                             2, 5, 8, 9,
                             4, 7, 2, 5,
                             2, 5, 8, 9);
        
        B  = hom_matrix_new (4, 8, 3, 2,
                             5, 2, 6, 8,
                             4, 7, 2, 5,
                             2, 5, 8, 9);
        
        Id = hom_matrix_new (1, 0, 0, 0,
                             0, 1, 0, 0,
                             0, 0, 1, 0,
                             0, 0, 0, 1);

        hom_matrix_print (Id);

        printf ("\nMatrix A:\n");
        hom_matrix_print (A);
        printf ("\nMatrix B:\n");
        hom_matrix_print (B);

        result = hom_matrix_multiply (A, B);
        printf ("\nMatrix A*B:\n");
        hom_matrix_print (result);
        hom_matrix_free (result);

        result = hom_matrix_multiply (B, A);
        printf ("\nMatrix B*A:\n");
        hom_matrix_print (result);
        hom_matrix_free (result);

        result = hom_matrix_multiply (B, A);
        printf ("\nMatrix B*A:\n");
        hom_matrix_print (result);
        hom_matrix_free (result);

        result = hom_matrix_multiply (B, Id);
        printf ("\nMatrix B*Id:\n");
        hom_matrix_print (result);
        hom_matrix_free (result);

        result = hom_matrix_multiply (Id, B);
        printf ("\nMatrix Id*B:\n");
        hom_matrix_print (result);
        hom_matrix_free (result);

        R = get_matrix_RZ (360);
        printf ("\nMatrix RZ(360):\n");
        hom_matrix_print (R);

        result = hom_matrix_multiply (R, B);
        printf ("\nMatrix RZ(360)*B\n");
        hom_matrix_print (result);
        hom_matrix_free (result);

        result = hom_matrix_multiply (get_matrix_RZ (45), B);
        printf ("\nMatrix RZ(45)*B\n");
        hom_matrix_print (result);
        hom_matrix_free (result);

        hom_matrix_free (A);
        hom_matrix_free (B);
        hom_matrix_free (R);
        hom_matrix_free (Id);
}
