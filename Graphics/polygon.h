#ifndef _POLYGON_H
#define _POLYGON_H

#include "vertex-list.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include <math.h>

typedef struct
{
        int y_max;
        int x;
        int numerator;
        int denumenator;
        gboolean positive_sign;
        int increment;
} EdgeTableEntry;

typedef struct
{
        int start;
        int end;
        GPtrArray *entries;
} EdgeTable;

typedef struct
{
        int y_coord;
        EdgeTableEntry *entry;
} YCoordList;

EdgeTable *create_edge_table (VertexList *points);
void print_active (GPtrArray *active_edge_table);
void draw_filled_polygon (GdkDrawable *drawable, GdkGC *gc, EdgeTable *table);

#endif
