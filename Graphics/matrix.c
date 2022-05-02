#include <glib.h>

typedef struct 
{
        long *data;
        int   n_rows;
        int   n_columns;
} Matrix;

Matrix *
matrix_new (long *array, int rows, int columns)
{
        Matrix *mtx;
        
        mtx = g_new (Matrix, 1);
        mtx->n_rows = rows;
        mtx->n_columns = columns;
        mtx->data = (long *) calloc (rows * columns, sizeof (long));
        
        if (array != NULL)
        {
                memcpy (mtx->data, array, sizeof (array) * rows * columns);
        }
        
        return mtx;
}

void
matrix_free (Matrix *mtx)
{
        g_return_if_fail (mtx != NULL);

        g_free (mtx->data);
        g_free (mtx);
}

void
matrix_print (Matrix *matx)
{
        int i, j;
        
        for (i = 0; i < matx->n_rows; i++)
        {
                printf ("{");

                for (j = 0; j < matx->n_columns; j++)
                {
                        printf ("%5d", matx->data[i * matx->n_rows + j]);
                }

                printf ("  }\n");
        }        
}


Matrix *
matrix_multiply (Matrix *mtx1, Matrix *mtx2)
{
        Matrix *result = matrix_new (NULL, mtx1->n_rows,mtx2->n_columns);
        int i, j, k;

        g_return_val_if_fail (mtx1->n_columns == mtx2->n_rows, NULL);

        for (j = 0; j < result->n_rows; j++)
        {
                for (i = 0; i < result->n_columns; i++)
                {
                        result->data[j * result->n_rows + i] = 0;
                        for(k = 0; k < mtx1->n_columns; k++)
                        {
                                result->data[j * result->n_rows + i] +=
                                        mtx1->data[j * mtx1->n_rows + k] * 
                                        mtx2->data[k * mtx2->n_rows + i];
                        }
                }
        }

        return result;
}

int 
main ()
{
        Matrix *matx1, *matx2, *result;
        
        long data1[3 * 3] = {4, 7, 2, 5, 2, 5, 8, 9, 2};
        long data2[3 * 3] = {1, 2, 4, 7, 3, 7, 2, 9, 3};

        matx1 = matrix_new (data1, 3, 3);
        matx2 = matrix_new (data2, 3, 3);
        
        printf ("\nMatrix No. 1:\n");
        matrix_print (matx1);
        printf ("\nMatrix No. 1:\n");
        matrix_print (matx2);

        result = matrix_multiply (matx1, matx2);

        printf ("\nMatrix multiplication result:\n");
        matrix_print (result);
        matrix_free (matx1);
        matrix_free (matx2);
        matrix_free (result);
}
