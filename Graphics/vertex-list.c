#include <stdio.h>
#include <glib.h>
#include "vertex-list.h"

Vertex
vertex_new (float x, float y)
{
        Vertex v;
        
        v.x = x;
        v.y = y;   v.y = y;
        
        return v;
}

VertexList *
vertexlist_construct ()
{
        VertexList *list;
        list = g_new (VertexList, 1);

        list->array = g_new0 (Vertex, VERTEXLIST_MIN_SIZE);
        list->len  = 0;
        list->internal_size = VERTEXLIST_MIN_SIZE;

        return list;
}

Vertex *
vertexlist_get (VertexList *list, int number)
{
        if (number < list->len)
                return list->array + number;
        else
                return NULL;
}

gboolean
vertexlist_set (VertexList *list, int number, Vertex value)
{
        if (number > list->len)
                return FALSE;
        
        list->array[number] = value;
        return TRUE;
}

void
vertexlist_add (VertexList *list, Vertex value)
{
        int new_size;

        if (list->len + 1 > list->internal_size)
        {
                new_size = 2 * list->internal_size;
                list->array = (Vertex *) realloc ((void *) list->array, sizeof (Vertex) * new_size);
                list->internal_size = new_size;
        }
        list->array[list->len] = value; // index starts at 0
        list->len++;
}
