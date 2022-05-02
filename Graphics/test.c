#include "hommatrix.h"
#include "vertex-list.h"
#include "vector.h"
#include "polygon.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include <math.h>

#define SCALE_BACK 0
#define SCALE_FRONT 1

double lookup_sin[256], lookup_cos[256];

typedef struct
{
        float x, y;
        float r, g, b;
} RGBPoint;

typedef struct
{
        Vector3D unit_vector;
        float point_intensivity; //red green blue
        float c0, c1, c2;
} LightSource;

typedef struct
{
        int red, green, blue;
} RGBColor;

float depth_cueing (int z_depth, 
                    int front_clipping_plane, 
                    int back_clipping_plane);
void warn_if_fail (gboolean expression, char *msg);
void draw_point (GdkDrawable *drawable, GdkGC *gc, RGBPoint point);
void draw_triangle (GdkDrawable *drawable, GdkGC *gc,
                    float x0, float y0,
                    float x1, float y1,
                    float x2, float y2);
void draw_gouraud_triangle (GdkDrawable *drawable, GdkGC *gc,
                            RGBPoint a,
                            RGBPoint b,
                            RGBPoint c);
void draw_curve (GdkDrawable *drawable, GdkGC *gc,
                 float cx[4],
                 float cy[4],
                 float cz[4],
                 int n_steps);
void create_hermite_curve_data (int p1x, int p1y, int p1z, //point
                                int p4x, int p4y, int p4z,
                                int r1x, int r1y, int r1z, //tangent
                                int r4x, int r4y, int r4z,
                                float cx[4],
                                float cy[4],
                                float cz[4]);



// should be macro we can remove for efficiency
void warn_if_fail (gboolean expression, char *msg)
{
          if (!expression)
          {
                  printf ("WARNING: %s at line %d", msg, __LINE__);
          }
}

void draw_point (GdkDrawable *drawable, GdkGC *gc, RGBPoint point)
{
        GdkColor colour;
        static GdkColormap *map;

        colour.red = (int) point.r;
        colour.green = (int) point.g;
        colour.blue = (int) point.b;

        if (map == NULL)
                map = gdk_colormap_new (gdk_visual_get_system (), TRUE);
        
        gdk_colormap_alloc_color (map, &colour, TRUE, TRUE);
        // printf ("RGB: %d, %d, %d\n", (int) point.r, (int) point.g, (int) point.b);

        gdk_gc_set_foreground (gc, &colour);

        gdk_draw_point (drawable, gc, point.x, point.y); 
}


// Go through 256 units (that is the units of a full circle)
// Convert x to radians (2 * PI in a full circle)
void 
create_sin_table ()
{
        int x;

        for (x = 0; x < 256; x++)
        {
                lookup_sin[x] = sin (x * M_PI / 128);
        }
        
}

void 
create_cos_table ()
{
        int x;
        
        for (x = 0; x < 256; x++)
        {
                lookup_cos[x] = cos (x * M_PI / 128);
        }
   
}

void
draw_triangle (GdkDrawable *drawable, GdkGC *gc,
               float x0, float y0,
               float x1, float y1,
               float x2, float y2)
{
        float dx1, dx2, dx3;
        float right_x, left_x, y, x;

        // sort!
        
        dx1 = (y1 - y0 > 0) ? (x1 - x0) / (y1 - y0) : 0;
        dx2 = (y2 - y0 > 0) ? (x2 - x0) / (y2 - y0) : 0;
        dx3 = (y2 - y1 > 0) ? (x2 - x1) / (y2 - y1) : 0;

        //printf ("%f %f %f", dx1, dx2, dx3);

        right_x = left_x = x0;

	if (dx1 > dx2)
	{
                // printf ("Step 1 - Drawing from %f to %f in y\n", y0, y1);
		for (y = y0; y <= y1; y++)
                {
                        // printf ("Drawing from %f to %f in x\n", right_x, left_x);
                        for (x = right_x; x < left_x; x++)
                        {
                                gdk_draw_point (drawable, gc, x, y);     
                        }
                        
                        left_x += dx1;
                        right_x += dx2;
                }
                
                left_x = x1;

                // printf ("Step 2 - Drawing from %f to %f in y\n", y0, y1);
		for (; y <= y2; y++)
                {
                        // printf ("Drawing from %f to %f in x\n", right_x, left_x);
                        for (x = right_x; x < left_x; x++)
                        {
                                gdk_draw_point (drawable, gc, x, y);     
                        }

                        left_x += dx3;
                        right_x += dx2;
                }
	}
	else
	{
		for (y = y0; y <= y1; y++)
                {
                        for (x = right_x; x < left_x; x++)
                        {
                                gdk_draw_point (drawable, gc, x, y);     
                        }
                        
                        left_x += dx2;
                        right_x += dx1;
                }
                
                right_x = x1;

		for (; y <= y2; y++)
                {
                        for (x = right_x; x < left_x; x++)
                        {
                                gdk_draw_point (drawable, gc, x, y);     
                        }

                        left_x += dx2;
                        right_x += dx3;
                }
	}
}



void
draw_gouraud_triangle (GdkDrawable *drawable, GdkGC *gc,
                       RGBPoint a,
                       RGBPoint b,
                       RGBPoint c)
{
        float dx1, dr1, dg1, db1;
        float dx2, dr2, dg2, db2;
        float dx3, dr3, dg3, db3;
        
        float dy, dx, y, x, dr, dg, db;
        RGBPoint left, right, point;

        // sort!
        if (b.y - a.y > 0)
	{
                dy = (b.y - a.y);

		dx1 = (b.x - a.x) / dy;
		dr1 = (b.r - a.r) / dy;
		dg1 = (b.g - a.g) / dy;
		db1 = (b.b - a.b) / dy;
	} 
        else 
        {
                dx1 = dr1 = dg1 = db1 = 0;
        }
        
        if (c.y - a.y > 0)
	{
                dy = (c.y - a.y);

		dx2 = (c.x - a.x) / dy;
		dr2 = (c.r - a.r) / dy;
		dg2 = (c.g - a.g) / dy;
		db2 = (c.b - a.b) / dy;
	} 
        else 
        {
                dx2 = dr2 = dg2 = db2 = 0;
        }

        if (c.y - b.y > 0)
	{
                dy = (c.y - b.y);

		dx3 = (c.x - b.x) / dy;
		dr3 = (c.r - b.r) / dy;
		dg3 = (c.g - b.g) / dy;
		db3 = (c.b - b.b) / dy;
	} 
        else 
        {
                dx3 = dr3 = dg3 = db3 = 0;
        }

        //printf ("%f %f %f", dx1, dx2, dx3);

        right.x = left.x = a.x;

	if (dx1 > dx2)
	{
                //printf ("Step 1 - Drawing from %f to %f in y\n", a.y, b.y);
		for (y = a.y; y <= b.y; y++)
                {
                        if (left.x - right.x > 0)
			{
                                dx = (left.x - right.x);
				dr = (left.r - right.r) / dx;
				dg = (left.g - right.g) / dx;
				db = (left.b - right.b) / dx;
			}
                        else 
                        {
                                dr = dg = db = 0;
                        }

			point.x = left.x;
                        point.y = y;
                        point.r = left.r;
                        point.g = left.g;
                        point.b = left.b;

                        //printf ("Step 2 - Drawing from %f to %f in x\n", point.x, right.x);
			for(; point.x < right.x ; point.x++)
			{
                                draw_point (drawable, gc, point);
				point.r += dr; 
                                point.g += dg; 
                                point.b += db;
			}
                        left.x += dx2; left.r += dr2; left.g += dg2; left.b += db2;
                        right.x += dx1; right.r += dr1; right.g += dg1; right.b += db1;
                }
                
                right.x = b.x;

                //printf ("Step 2 - Drawing from %f to %f in y\n", b.y, c.y);
		for (; y <= c.y; y++)
                {
                        if (left.x - right.x > 0)
			{
                                dx = (left.x - right.x);
				dr = (left.r - right.r) / dx;
				dg = (left.g - right.g) / dx;
				db = (left.b - right.b) / dx;
			}
                        else 
                        {
                                dr = dg = db = 0;
                        }

			point.x = left.x;
                        point.y = y;
                        point.r = left.r;
                        point.g = left.g;
                        point.b = left.b;

			for(; point.x < right.x ; point.x++)
			{
				draw_point (drawable, gc, point); // add color
				point.r += dr; 
                                point.g += dg; 
                                point.b += db;
			}
                        left.x += dx2; left.r += dr2; left.g += dg2; left.b += db2;
                        right.x += dx3; right.r += dr3; right.g += dg3; right.b += db3;
                }
	}
	else
	{
                // printf ("Step 1 - Drawing from %f to %f in y\n", a.y, b.y);
		for (y = a.y; y <= b.y; y++)
                {
                        if (left.x - right.x > 0)
			{
                                dx = (left.x - right.x);
				dr = (left.r - right.r) / dx;
				dg = (left.g - right.g) / dx;
				db = (left.b - right.b) / dx;
			}
                        else 
                        {
                                dr = dg = db = 0;
                        }

			point.x = left.x;
                        point.y = y;
                        point.r = left.r;
                        point.g = left.g;
                        point.b = left.b;

			for(; point.x < right.x ; point.x++)
			{
				draw_point (drawable, gc, point); // add color
				point.r += dr; 
                                point.g += dg; 
                                point.b += db;
			}
                        left.x += dx1; left.r += dr1; left.g += dg1; left.b += db1;
                        right.x += dx2; right.r += dr2; right.g += dg2; right.b += db2;
                }
                
                left.x = b.x;

                // printf ("Step 1 - Drawing from %f to %f in y\n", b.y, c.y);
		for (; y <= c.y; y++)
                {
                        if (left.x - right.x > 0)
			{
                                dx = (left.x - right.x);
				dr = (left.r - right.r) / dx;
				dg = (left.g - right.g) / dx;
				db = (left.b - right.b) / dx;
			}
                        else 
                        {
                                dr = dg = db = 0;
                        }

			point.x = left.x;
                        point.y = y;
                        point.r = left.r;
                        point.g = left.g;
                        point.b = left.b;

			for(; point.x < right.x ; point.x++)
			{
				draw_point (drawable, gc, point); // add color
				point.r += dr; 
                                point.g += dg; 
                                point.b += db;
			}
                        left.x += dx3; left.r += dr3; left.g += dg3; left.b += db3;
                        right.x += dx2; right.r += dr2; right.g += dg2; right.b += db2;
                }
	}
}

void
internal_draw_circle_points (GdkDrawable *drawable, GdkGC *gc,
                             int x0, int y0, int x, int y)
{
     gdk_draw_point (drawable, gc,  x + x0,  y + y0);
     gdk_draw_point (drawable, gc,  y + x0,  x + y0);
     gdk_draw_point (drawable, gc,  y + x0, -x + y0);
     gdk_draw_point (drawable, gc,  x + x0, -y + y0); 
     gdk_draw_point (drawable, gc, -x + x0, -y + y0);
     gdk_draw_point (drawable, gc, -y + x0, -x + y0);
     gdk_draw_point (drawable, gc, -y + x0,  x + y0);
     gdk_draw_point (drawable, gc, -x + x0,  y + y0);
}

void
internal_draw_ellipse_points (GdkDrawable *drawable, GdkGC *gc,
                             int x0, int y0, int x, int y)
{
     gdk_draw_point (drawable, gc,  x + x0,  y + y0);
     gdk_draw_point (drawable, gc,  x + x0, -y + y0); 
     gdk_draw_point (drawable, gc, -x + x0, -y + y0);
     gdk_draw_point (drawable, gc, -x + x0,  y + y0);
}


// Can be optimized, look in book.
// supposed to be 3d, only printing 2d
void
draw_curve (GdkDrawable *drawable, GdkGC *gc,
            float cx[4],
            float cy[4],
            float cz[4],
            int n_steps)
{
        int i;
        double d = 1.0 / n_steps;
        double t = 0;
        
        gdk_draw_point (drawable, gc, cx[3], cy[3]);
        
        for (i = 0; i < n_steps; i++)
        {
                double t2, t3, x, y, z;
                
                t += d;
                t2 = t * t;
                t3 = t2 * t;
                
                x = cx[0] * t3 + cx[1] * t2 + cx[2] * t + cx[3];
                y = cy[0] * t3 + cy[1] * t2 + cy[2] * t + cy[3];
                z = cz[0] * t3 + cz[1] * t2 + cz[2] * t + cz[3];
                
                gdk_draw_point (drawable, gc, x, y);
        }
}

void
create_hermite_curve_data (int p1x, int p1y, int p1z, //point
                           int p4x, int p4y, int p4z,
                           int r1x, int r1y, int r1z, //tangent
                           int r4x, int r4y, int r4z,
                           float cx[4],
                           float cy[4],
                           float cz[4])
{
        HomMatrix *hermite_matrix;
        HomMatrix *geometry_matrix;
        HomMatrix *result;

        hermite_matrix 
                = hom_matrix_new 
                (2, -2,  1,  1,
                -3,  3, -2, -1, 
                 0,  0,  1,  0,  
                 1,  0,  0,  0);
        
        geometry_matrix 
                = hom_matrix_new
                (p1x, p1y, p1z, 0,
                 p4x, p4y, p4z, 0,
                 r1x, r1y, r1z, 0,
                 r4x, r4y, r4z, 0);
                
        result = hom_matrix_multiply (hermite_matrix, geometry_matrix);
        
        hom_matrix_free (hermite_matrix);
        hom_matrix_free (geometry_matrix);

        cx[0] = result->data[0][0];
        cx[1] = result->data[1][0];
        cx[2] = result->data[2][0];
        cx[3] = result->data[3][0];

        cy[0] = result->data[0][1];
        cy[1] = result->data[1][1];
        cy[2] = result->data[2][1];
        cy[3] = result->data[3][1];

        cz[0] = result->data[0][2];
        cz[1] = result->data[1][2];
        cz[2] = result->data[2][2];
        cz[3] = result->data[3][2];

        hom_matrix_free (result);
}

void
create_bezier_curve_data (int p1x, int p1y, int p1z, //end point
                          int p2x, int p2y, int p2z,
                          int p3x, int p3y, int p3z, //control points
                          int p4x, int p4y, int p4z,
                          float cx[4],
                          float cy[4],
                          float cz[4])
{
        HomMatrix *bezier_matrix;
        HomMatrix *geometry_matrix;
        HomMatrix *result;

        bezier_matrix 
                = hom_matrix_new 
                (-1,  3, -3,  1,
                  3, -6,  3,  0, 
                 -3,  3,  0,  0,  
                  1,  0,  0,  0);
        
        geometry_matrix 
                = hom_matrix_new
                (p1x, p1y, p1z, 0,
                 p2x, p2y, p2z, 0,
                 p3x, p3y, p3z, 0,
                 p4x, p4y, p4z, 0);
                
        result = hom_matrix_multiply (bezier_matrix, geometry_matrix);
        
        hom_matrix_free (bezier_matrix);
        hom_matrix_free (geometry_matrix);

        cx[0] = result->data[0][0];
        cx[1] = result->data[1][0];
        cx[2] = result->data[2][0];
        cx[3] = result->data[3][0];

        cy[0] = result->data[0][1];
        cy[1] = result->data[1][1];
        cy[2] = result->data[2][1];
        cy[3] = result->data[3][1];

        cz[0] = result->data[0][2];
        cz[1] = result->data[1][2];
        cz[2] = result->data[2][2];
        cz[3] = result->data[3][2];

        hom_matrix_free (result);
}

void
draw_midpoint_ellipse (GdkDrawable *drawable, GdkGC *gc,
                        int x0, int y0, int a, int b)
{
        int x = 0;
        int y = b;
        int b2 = b * b;
        int a2 = a * a;

        double d1 = b2 - (a2 * b) + (0.25 * a2);
        double d2 = b2 * (x + 0.5) * (x + 0.5) +
                a2 * (y - 1) *(y - 1) - a2 * b2;

        internal_draw_ellipse_points (drawable, gc, x0, y0, x, y);

        /* region 1
        while (a2 * (y - 0.5) > b2 * (x + 1)) /* checking for gradient */
        {
                if (d1 < 0) /* east */
                {
                        d1 += b2 * (2 * x + 3);
                } else {    /* south east */
                        d1 += b2 * (2 * x + 3) + a2 * (-2 * y + 2);
                        y--;
                }
                x++;
                
                internal_draw_ellipse_points (drawable, gc, x0, y0, x, y);
        }
        
        /* region 2 */
        while (y > 0)
        {
                if (d2 < 0) /* south east */
                {
                        d2 += b2 * (2 * x + 2) + a2 * (-2 * y +3);
                        x++;
                } else {    /* south */
                        d2 += a2 * (-2 * y + 3);
                }
                y--;
                
                internal_draw_ellipse_points (drawable, gc, x0, y0, x, y);
        }
}


void
draw_midpoint_circle (GdkDrawable *drawable, GdkGC *gc, 
                      int x0, int y0, int radius)
{
        int x = 0;
        int y = radius;
        int d = 1 - radius;
        internal_draw_circle_points (drawable, gc, x0, y0, x, y);
        
        while (y > x)
        {
                if (d < 0)
                {
                        d += 2 * x + 3;
                } else {
                        d += 2 * (x - y) + 5;
                        y--;
                }
                x++;
                
                internal_draw_circle_points (drawable, gc, x0, y0, x, y);
        }
}



void 
draw_midpoint_line (GdkDrawable *drawable, GdkGC *gc, 
                    int x0, int y0, int x1, int y1)
{
        int x = x0;
        int y = y0;
        int dx = ABS (x1 - x0);
        int dy = ABS (y1 - y0);

        gdk_draw_point (drawable, gc, x, y);

        /* We times things with two (<< 1) so we can stay in
         * int. We need to substract a half, but since we are
         * only interested in the sign, we just multiply with 2
         */
        int incr_x = (x0 < x1) ? 1 : - 1;
        int incr_y = (y0 < y1) ? 1 : - 1;

        if (dx >= dy)
        {
                int incr_below  = dy << 1;
                int incr_above = (dy - dx) << 1;
                
                int d = 2 * dy - dx;
                
                while (x1 != x)
                {
                        if (d < 0)
                        {
                                d += incr_below;
                                x += incr_x;
                                
                        } else {
                                d += incr_above;
                                x += incr_x;
                                y += incr_y;
                        }
                        gdk_draw_point (drawable, gc, x, y);   
                }
        }
        else {
                int incr_below  = dx << 1;
                int incr_above = (dx - dy) << 1;

                int d = 2 * dx - dy;

                while (y != y1)
                {
                        if (d < 0)
                        {
                                d += incr_below;
                                y += incr_y;
                                
                        } else {
                                d += incr_above;
                                y += incr_y;
                                x += incr_x;
                        }

                        gdk_draw_point (drawable, gc, x, y);
                }
        } 
}


void
draw_polyline_from_points (GdkDrawable *drawable, GdkGC *gc, VertexList *points)
{
        int i;
        Vertex *vertex1, *vertex2;
        
        for (i = 0; i + 1 < points->len; i++)
        {
                vertex1 = vertexlist_get (points, i);
                vertex2 = vertexlist_get (points, i + 1);

                draw_midpoint_line (drawable, gc, 
                                    vertex1->x, vertex1->y,
                                    vertex2->x, vertex2->y);
        }
}

void
draw_polyline_from_edges (GdkDrawable *drawable, GdkGC *gc, VertexList *points)
{
}


void
print_edge_table (EdgeTable *table)
{
        int i, j, offset;
        GPtrArray *sub;
        EdgeTableEntry *ptr;
        

        printf ("--== Edge Table ==--\n\n");
        for (i = table->end; i >= table->start; i--)
        {
                offset = i - table->start;
                sub = g_ptr_array_index (table->entries, offset);
                printf ("[%2d] -> ", i);

                for (j = 0; sub != NULL && j < sub->len; j++)
                {
                        ptr = g_ptr_array_index (sub, j);
                        if (ptr->numerator == 0)
                        {
                                printf ("[%3d,%3d,%3d] -> ", 
                                         ptr->y_max, 
                                         ptr->x, 0);
                        } else {        
                                printf ("[%3d,%3d,%3d/%d] -> ", 
                                        ptr->y_max, 
                                        ptr->x, 
                                        ptr->numerator, 
                                        ptr->denumenator);
                        }
                        
                }
                printf ("[NULL]\n");
        }
}

void do_draw (GObject *);

void do_draw (GObject *o)
{
        GdkColor colour, colour2;
        GdkGC *gc, *gc2;   
        float cx[4];
        float cy[4];
        float cz[4];
        VertexList *vertices;
        int i;
        RGBPoint pa, pb, pc;

        EdgeTable *edge_table_entries, *edge_table_entries2;
        VertexList *test, *test2, *test3;

        gc = gdk_gc_new (GTK_WIDGET (o)->window);
        gc2 = gdk_gc_new (GTK_WIDGET (o)->window);
        colour.pixel = 1234;
        gdk_gc_set_foreground (gc, &colour);
        colour2.pixel = 1;
        gdk_gc_set_foreground (gc2, &colour2);


/*         draw_midpoint_line (GTK_WIDGET (o)->window, gc, 30, 30, 100, 200); */
/*         draw_midpoint_line (GTK_WIDGET (o)->window, gc, 20, 60, 100, 200); */

/*         draw_midpoint_line (GTK_WIDGET (o)->window, gc, 30, 30, 200, 100); */
/*         draw_midpoint_line (GTK_WIDGET (o)->window, gc, 20, 60, 200, 100); */

/*         draw_midpoint_line (GTK_WIDGET (o)->window, gc, 200, 200, 300, 50); */
/*         draw_midpoint_line (GTK_WIDGET (o)->window, gc, 200, 200, 300, 150); */

/*         draw_midpoint_line (GTK_WIDGET (o)->window, gc, 0, 0, 400, 400); */

/*         draw_midpoint_line (GTK_WIDGET (o)->window, gc, 0, 400, 400, 0); */

         draw_midpoint_line (GTK_WIDGET (o)->window, gc, 0, 200, 400, 200); 
         draw_midpoint_line (GTK_WIDGET (o)->window, gc, 200, 0, 200, 400); 

/*         draw_midpoint_line (GTK_WIDGET (o)->window, gc, 300, 50, 300, 150); */

/*         draw_midpoint_circle (GTK_WIDGET (o)->window, gc, 150, 150, 100); */
/*         draw_midpoint_ellipse (GTK_WIDGET (o)->window, gc, 200, 200, 30, 50); */

/*         create_hermite_curve_data (50, 50, 0, */
/*                                    300, 200, 0, */
/*                                    -200, 0, 0, */
/*                                    0, -500, 0, */
/*                                    cx, cy, cz); */

/*         draw_curve (GTK_WIDGET (o)->window, gc, cx, cy, cz, 5000); */

/*         create_bezier_curve_data (100, 100, 0, */
/*                                   300, 300, 0, */
/*                                   100, 150, 0, */
/*                                   0, 300, 0, */
/*                                   cx, cy, cz);        */

/*         draw_curve (GTK_WIDGET (o)->window, gc, cx, cy, cz, 5000); */
        
/*         vertices = vertexlist_construct (); */
        

/*         vertexlist_add (vertices, vertex_new (10, 20)); */
/*         vertexlist_add (vertices, vertex_new (50, 10)); */
/*         vertexlist_add (vertices, vertex_new (60, 90)); */
/*         // vertexlist_add (vertices, vertex_new (10, 20)); // error! */
/*         vertexlist_add (vertices, vertex_new (80, 110)); */

/*         draw_polyline_from_points (GTK_WIDGET (o)->window, gc, vertices); */

/*         // You can change some of the entries and draw the line again :) */
/*         vertexlist_set (vertices, 1, vertex_new (60, 15)); */
        
/*         draw_polyline_from_points (GTK_WIDGET (o)->window, gc, vertices); */

        test = vertexlist_construct ();
/*         vertexlist_add (test, vertex_new (2, 3)); */
/*         vertexlist_add (test, vertex_new (7, 1)); */
/*         vertexlist_add (test, vertex_new (13, 5)); */
/*         vertexlist_add (test, vertex_new (13, 11)); */
/*         vertexlist_add (test, vertex_new (7, 7)); */
/*         vertexlist_add (test, vertex_new (2, 9)); */
/*         vertexlist_add (test, vertex_new (2, 3));  */
        /*       vertexlist_add (test, vertex_new (20, 30));
        vertexlist_add (test, vertex_new (70, 10));
        vertexlist_add (test, vertex_new (130, 50));
        vertexlist_add (test, vertex_new (130, 110));
        vertexlist_add (test, vertex_new (70, 70));
        vertexlist_add (test, vertex_new (20, 90));
        vertexlist_add (test, vertex_new (20, 30)); 

        test2 = vertexlist_construct ();
        vertexlist_add (test2, vertex_new (220, 30));
        vertexlist_add (test2, vertex_new (270, 10));
        vertexlist_add (test2, vertex_new (330, 50));
        vertexlist_add (test2, vertex_new (330, 110));
        vertexlist_add (test2, vertex_new (270, 70));
        vertexlist_add (test2, vertex_new (220, 90));
        vertexlist_add (test2, vertex_new (220, 30));
        draw_polyline_from_points (GTK_WIDGET (o)->window, gc, test2);

        test3 = vertexlist_construct ();
        vertexlist_add (test3, vertex_new (20, 230));
        vertexlist_add (test3, vertex_new (70, 210));
        vertexlist_add (test3, vertex_new (130, 250));
        vertexlist_add (test3, vertex_new (130, 310));
        vertexlist_add (test3, vertex_new (70, 270));
        vertexlist_add (test3, vertex_new (20, 290));
        vertexlist_add (test3, vertex_new (20, 230)); 
        
        edge_table_entries = create_edge_table (test);
        // print_edge_table (edge_table_entries);

        // Test that shows that we can delete a line drawing the opposite direction
        draw_midpoint_line (GTK_WIDGET (o)->window, gc, 360, 340, 390, 320);
        draw_midpoint_line (GTK_WIDGET (o)->window, gc, 360 + 3, 340 + 3, 390 + 3, 320 + 3);
        draw_midpoint_line (GTK_WIDGET (o)->window, gc2, 390, 320, 360, 340);

        draw_filled_polygon (GTK_WIDGET (o)->window, gc2, edge_table_entries);
        draw_polyline_from_points (GTK_WIDGET (o)->window, gc, test);
        edge_table_entries2 = create_edge_table (test3);
        draw_filled_polygon (GTK_WIDGET (o)->window, gc2, edge_table_entries2);

        */
        pa.x = 100; pa.y = 100; pa.r = 0, pa.g = 0, pa.b = 0;
        pb.x = 350; pb.y = 130; pb.r = 8000, pb.g = 8000, pb.b = 8000;
        pc.x = 220; pc.y = 360; pc.r = 20000, pc.g = 20000, pc.b = 20000;

        draw_gouraud_triangle (GTK_WIDGET (o)->window, gc,
                               pa, pb, pc);

        //draw_triangle (GTK_WIDGET (o)->window, gc,
        //             100, 100,
        //             50, 150,
        //             200, 200);
}

RGBColor
intensitivity (LightSource lights[],
               int light_no,
               Vector3D planenorm_unit_vector,
               Vector3D viewpoint_unit_vector,
               RGBColor inf_color,
               float specular_reflec_exponent,
               float lightsrc_dist[],
               float ambient_const,
               float diffuse_const,
               float specular_const,
               float ambient_inten[3], 
               float diffuse_object[3],
               float specular_object[3],
               float z_depth, 
               float front_clipping_plane,
               float back_clipping_plane)
{
        RGBColor result;

	float ambient;
        float diffuse_specular_sum = 0;
	float lightsrc_attent;
        float phong[3];
	float cuing; // atmospheric attenuation
        int colorno, i;
        Vector3D reflection_unit_vector;
        
        warn_if_fail (ambient_const <= 1 && ambient_const >= 0, "ambient const not within [0..1]");
        warn_if_fail (diffuse_const <= 1 && diffuse_const >= 0, "diffuse const not within [0..1]");
        warn_if_fail (specular_const <= 1 && specular_const >= 0, "specular const not within [0..1]");                

        for (colorno = 0; colorno < 3; colorno++)
        {
                // diffuse is not a typo
                ambient = ambient_const * diffuse_object[colorno] * ambient_inten[colorno];

                for (i = 0; i < light_no; i++)
                {
                        lightsrc_attent = MIN (1, 1 / (lights[i].c0 + 
                                                       lights[i].c1 * lightsrc_dist[i] + 
                                                       lights[i].c2 * lightsrc_dist[i] * lightsrc_dist[i]));

                        reflection_unit_vector = vector_mirror (lights[i].unit_vector, planenorm_unit_vector);
                        
                        diffuse_specular_sum += lightsrc_attent * lights[i].point_intensivity * 
                                (diffuse_const * diffuse_object[colorno] * 
                                 vector_dot (planenorm_unit_vector, lights[i].unit_vector) 
                                 + 
                                 specular_const * specular_object[colorno] *
                                 pow (vector_dot (reflection_unit_vector, viewpoint_unit_vector), 
                                      specular_reflec_exponent));
                }
                
                phong[colorno] = ambient + diffuse_specular_sum;
        }
        
        cuing = depth_cueing (z_depth, front_clipping_plane, back_clipping_plane);
	
	result.red   = cuing * phong[0] + (1 - cuing) * inf_color.red;
        result.green = cuing * phong[1] + (1 - cuing) * inf_color.green;
        result.blue  = cuing * phong[2] + (1 - cuing) * inf_color.blue;
        
        return result;
}


float
depth_cueing (int z_depth, 
	      int front_clipping_plane, 
	      int back_clipping_plane)
{
	if (z_depth < back_clipping_plane)
	{
		return SCALE_BACK; // [0..1]
	}
	else if (z_depth > front_clipping_plane)
	{
		return SCALE_FRONT; // [0..1]
	}
	else
	{
		return SCALE_BACK + ((SCALE_FRONT - SCALE_BACK) * (z_depth - back_clipping_plane)) /
                        (front_clipping_plane - back_clipping_plane);
	}
}

int
main (int argc, char **argv) 
{
        GtkWidget *window;
     
        gtk_init (&argc, &argv);

        window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title (GTK_WINDOW (window), "Graphics Test");
        gtk_window_set_default_size (GTK_WINDOW (window), 400, 400);
        g_signal_connect (G_OBJECT (window), "destroy",
                          gtk_main_quit, NULL);

        

        g_signal_connect (G_OBJECT (window), "expose_event",
                          G_CALLBACK (do_draw), G_OBJECT (window));

        gtk_widget_show_all (window);

        gtk_main ();
      
        return 0;       
}
