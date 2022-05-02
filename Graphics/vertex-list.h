#include <stdio.h>

#define VERTEXLIST_MIN_SIZE 5

typedef struct
{
        float x;
        float y;
} Vertex;


typedef struct 
{
        Vertex *array;
        int     len;
        int     internal_size;
} VertexList;

Vertex      vertex_new           (float x, float y);
VertexList *vertexlist_construct (void);
Vertex     *vertexlist_get       (VertexList *list, int number);
gboolean    vertexlist_set       (VertexList *list, int number, Vertex value);
void        vertexlist_add       (VertexList *list, Vertex value);


