#include "polygon.h"

EdgeTable *
create_edge_table (VertexList *points)
{
        EdgeTableEntry *ptr;
        EdgeTableEntry *add_ptr;
        EdgeTableEntry *tmp;
        EdgeTable      *table;
        YCoordList     *ptr2;
        YCoordList     *ycoord;
        GPtrArray      *entries;
        GPtrArray      *result;
        GPtrArray      *sub;
        Vertex         *vertex1;
        Vertex         *vertex2;

        int count;
        int sign;
        int i, j;
        int offset;

        vertex1 = vertexlist_get (points, 0);
        
        table = g_new0 (EdgeTable, 1);
        table->start = vertex1->y;
        table->end   = 0;
        
        entries = g_ptr_array_new ();
        
        for (i = 1; i < points->len; i++)
        {       
                ptr2 = ycoord = g_new (YCoordList, 1);
                ptr  = ycoord->entry = g_new (EdgeTableEntry, 1);
                
                g_ptr_array_add (entries, ycoord);
                
                vertex2 = vertexlist_get (points, i);
                
                if (vertex1->y > vertex2->y)
                {
                        ptr->y_max       = vertex1->y;
                        ptr->x           = vertex2->x;
                        ptr->denumenator = ptr->y_max - vertex2->y;

                        ptr2->y_coord    = vertex2->y;
                        table->start     = MIN (vertex2->y, table->start);
                        table->end       = MAX (vertex1->y, table->end);
                } else {
                        ptr->y_max       = vertex2->y;
                        ptr->x           = vertex1->x;
                        ptr->denumenator = ptr->y_max - vertex1->y;

                        ptr2->y_coord    = vertex1->y;
                        table->start     = MIN (vertex1->y, table->start);
                        table->end       = MAX (vertex2->y, table->end);
                }
                ptr->increment = ptr->denumenator;

                ptr->positive_sign = (ptr->x != MIN (vertex1->x, vertex2->x)) ? -1 : 1;
                ptr->numerator = (MAX (vertex1->x, vertex2->x) - MIN (vertex1->x, vertex2->x));

                vertex1 = vertex2;
        }

        ptr2 = g_ptr_array_index (entries, 0);

        /* Sort into buckets - based on insertion sort */
        result = g_ptr_array_new ();

        g_ptr_array_set_size (result, (table->end - table->start + 1)); // it NULLs them

        for (i = 0; i + 1 < points->len; i++)
        {
                ptr2 = g_ptr_array_index (entries, i);
                offset = ptr2->y_coord - table->start;
                sub = g_ptr_array_index (result, offset);
                
                if (sub == NULL)
                {
                        sub = g_ptr_array_new ();
                        g_ptr_array_index (result, offset) = sub;
                        g_ptr_array_add (sub, ptr2->entry);
                }
                else 
                {
                        j = 0;

                        do 
                        {
                                add_ptr = g_ptr_array_index (sub, j);
                                j++;
                        } while (j < sub->len && ptr2->entry->y_max > add_ptr->y_max);

                        if (ptr2->entry->y_max > add_ptr->y_max)
                                g_ptr_array_add (sub, ptr2->entry);
                        else
                        {
                                g_ptr_array_add (sub, add_ptr);
                                g_ptr_array_index (sub, j - 1) =  ptr2->entry;
                        }
                }       
        }

        table->entries = result;

        return table;
}

int
cmp_edge_table_on_x (EdgeTableEntry *a, EdgeTableEntry *b)
{
        return a->x - b->x;
}

void
print_active (GPtrArray *active_edge_table)
{
        EdgeTableEntry *entry, *entry2, *tmp;
        int j;
        
        for (j = 0; j < active_edge_table->len; j++)
        {
                entry2 =  g_ptr_array_index (active_edge_table, j);
                        
                if (entry2->numerator == 0)
                {
                        printf ("[%3d,%3d,%3d] -> ", 
                                entry2->y_max, 
                                entry2->x, 0);
                } else {        
                        printf ("[%3d,%3d,%3d/%d] -> ", 
                                entry2->y_max, 
                                entry2->x, 
                                entry2->numerator, 
                                entry2->denumenator);
                }
        }
        printf ("\n");
}


void
draw_filled_polygon (GdkDrawable *drawable, GdkGC *gc, EdgeTable *table)
{
/*   Algorithm
 *
 *   1. Set y to smallest y coord that has an entry in the edge table. (First non-empty bucket);
 *   2. Init active edge table to be empty;
 *   3. Repeat till AET and ET are empty
 *
 *        3.1 Move from ET bucket y to the AET those edges whose y_min = y (entering edges);
 *        3.2 Remove from the AET those entries for which y = y_max, then sort AET on x;
 *        3.3 Fill the decired pixel values on scan line y by using pairs of x coords from the AET; 
 *            use odd parity rule. parity is initial even and each intersec inverts it draw when odd
 *        3.4 incr y by 1;
 *        3.5 for each nonvertical line in the AET , update x for new y;    
 *      
 */

        GPtrArray *active_edge_table;
        GPtrArray *sub_array;
        EdgeTableEntry *entry, *entry2, *tmp;
        int parity;
        int i, j, x, y, len;
        double nom, denom;

        active_edge_table = g_ptr_array_new ();

        // printf ("\n\n");

        for (i = 0; i <= table->end - table->start; i++)
        {
                sub_array = g_ptr_array_index (table->entries, i);
                
                if (sub_array != NULL)
                {
                        for (j = 0; j < sub_array->len; j++)
                        {
                                // 3.1
                                entry = g_ptr_array_index (sub_array, j);
                                
                                g_ptr_array_add (active_edge_table, entry);
                        }
                }

                for (j = 0; j < active_edge_table->len; j++)
                {
                        entry =  g_ptr_array_index (active_edge_table, j);
                        
                        // 3.2.1
                        if (entry->y_max == table->start + i)
                        {
                                g_ptr_array_remove_index (active_edge_table, j);
                                j--;
                        }
                        
                }
                // 3.2.2
                // g_ptr_array_sort (active_edge_table, (GCompareFunc) cmp_edge_table_on_x);
                len = 0;
                
                while (len < active_edge_table->len)
                {
                        for (j = active_edge_table->len - 1; j > len; j--)
                        {
                                entry =  g_ptr_array_index (active_edge_table, j);
                                entry2 =  g_ptr_array_index (active_edge_table, j - 1);
                                
                                //printf ("(%d < %d) ", entry->x, entry2->x);
                                
                                if (entry->x < entry2->x) 
                                {
                                        g_ptr_array_index (active_edge_table, j) = entry2;
                                        g_ptr_array_index (active_edge_table, j - 1) = entry;
                                }
                                //print_active (active_edge_table);
                        }
                        len++;
                } 
                
                // 3.3
                parity = 0;
                y = table->start + i;
                entry =  g_ptr_array_index (active_edge_table, 0);
             
                for (j = 1; j < active_edge_table->len; j++)
                {
                        entry2 =  g_ptr_array_index (active_edge_table, j);
                        
                        if (parity % 2 == 0)
                        {
                                x = entry->x;
                                while (x < entry2->x)
                                {
                                        gdk_draw_point (drawable, gc,  x,  y);
                                        x++;
                                }
                        }       
                        parity++;

                        
                        entry = entry2;
                }
                
                // test print
                // printf ("[%2d] -> ", y);
                // print_active (active_edge_table);

                // 3.5
                for (j = 0; j < active_edge_table->len; j++)
                {
                        entry2 =  g_ptr_array_index (active_edge_table, j);
                        
                        if (entry2->numerator != 0)
                        {
                                if (entry2->numerator > entry2->denumenator)
                                {
                                        entry2->increment += (entry2->numerator % entry2->denumenator);

                                        if (entry2->positive_sign == TRUE)
                                                        entry2->x += (entry2->numerator / entry2->denumenator);
                                                else
                                                        entry2->x -= (entry2->numerator / entry2->denumenator);

                                        if (entry2->increment > entry2->denumenator)
                                        {
                                                if (entry2->positive_sign == TRUE)
                                                        entry2->x++;
                                                else
                                                        entry2->x--;
                                                entry2->increment -= entry2->denumenator;
                                        }
                                } else {
                                        entry2->increment += entry2->numerator;
                                        if (entry2->increment > entry2->denumenator)
                                        {
                                                if (entry2->positive_sign == TRUE)
                                                        entry2->x++;
                                                else
                                                        entry2->x--;
                                                entry2->increment -= entry2->denumenator;
                                        }
                                }
                        }
                }
        }
}

