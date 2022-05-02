#include "ScanConversion.h"
#include <math.h>

void
DrawGouraudTriangle (DotDevice *device,
                     RGBPoint a,
                     RGBPoint b,
                     RGBPoint c)
{
        double dx1, dr1, dg1, db1;
        double dx2, dr2, dg2, db2;
        double dx3, dr3, dg3, db3;
        
        double dy, dx, y, x, dr, dg, db;
        RGBPoint left, right, point;
        
        // sort!
        if (b.Y - a.Y > 0)
	{
                dy = (b.Y - a.Y);

		dx1 = (b.X - a.X) / dy;
		dr1 = (b.Red - a.Red) / dy;
		dg1 = (b.Green - a.Green) / dy;
		db1 = (b.Blue - a.Blue) / dy;
	} 
        else 
        {
                dx1 = dr1 = dg1 = db1 = 0;
        }
        
        if (c.Y - a.Y > 0)
	{
                dy = (c.Y - a.Y);

		dx2 = (c.X - a.X) / dy;
		dr2 = (c.Red - a.Red) / dy;
		dg2 = (c.Green - a.Green) / dy;
		db2 = (c.Blue - a.Blue) / dy;
	} 
        else 
        {
                dx2 = dr2 = dg2 = db2 = 0;
        }

        if (c.Y - b.Y > 0)
	{
                dy = (c.Y - b.Y);

		dx3 = (c.X - b.X) / dy;
		dr3 = (c.Red - b.Red) / dy;
		dg3 = (c.Green - b.Green) / dy;
		db3 = (c.Blue - b.Blue) / dy;
	} 
        else 
        {
                dx3 = dr3 = dg3 = db3 = 0;
        }

        //printf ("%f %f %f", dx1, dx2, dx3);

        right.X = left.X = a.X;
        right.Red = left.Red = a.Red;
        right.Green = left.Green = a.Green;
        right.Blue = left.Blue = a.Blue;

	if (dx1 >= dx2)
	{
                //printf ("Step 1 - Drawing from %f to %f in y\n", a.y, b.y);
		for (y = a.Y; y < b.Y; y++)
                {
                        if (right.X - left.X > 0)
			{
                                dx = (right.X - left.X);
				dr = (right.Red - left.Red) / dx;
				dg = (right.Green - left.Green) / dx;
				db = (right.Blue - left.Blue) / dx;
			}
                        else 
                        {
                                dr = dg = db = 0;
                        }

			point.X = left.X;
                        point.Y = y;
                        point.Red = left.Red;
                        point.Green = left.Green;
                        point.Blue = left.Blue;

                        //printf ("Step 2 - Drawing from %f to %f in x\n", point.x, right.x);
			for(; point.X < right.X; point.X++)
			{
                                point.X = floorf (point.X);
                                device->SetPixel (point);
				point.Red += dr; 
                                point.Green += dg; 
                                point.Blue += db;
			}

                        left.X += dx2; 
                        left.Red += dr2; 
                        left.Green += dg2; 
                        left.Blue += db2;

                        right.X += dx1; 
                        right.Red += dr1; 
                        right.Green += dg1; 
                        right.Blue += db1;
                }
                
                right.X = b.X;
                right.Red = b.Red;
                right.Green = b.Green;
                right.Blue = b.Blue;

                //printf ("Step 2 - Drawing from %f to %f in y\n", b.y, c.y);
		for (; y < c.Y; y++)
                {
                        // midpoint to the left, lower part 
                        if (right.X - left.X > 0)
			{
                                dx = (right.X - left.X);
				dr = (right.Red - left.Red) / dx;
				dg = (right.Green - left.Green) / dx;
				db = (right.Blue - left.Blue) / dx;
			}
                        else 
                        {
                                dr = dg = db = 0;
                        }

			point.X = left.X;
                        point.Y = y;
                        point.Red = left.Red;
                        point.Green = left.Green;
                        point.Blue = left.Blue;

			for(; point.X < right.X; point.X++)
			{
                                point.X = floorf (point.X);
				device->SetPixel (point);
				point.Red += dr; 
                                point.Green += dg; 
                                point.Blue += db;
			}

                        left.X += dx2;
                        left.Red += dr2; 
                        left.Green += dg2; 
                        left.Blue += db2;
                        
                        right.X += dx3;
                        right.Red += dr3; 
                        right.Green += dg3; 
                        right.Blue += db3;
                }
	}
	else
	{
                // printf ("Step 1 - Drawing from %f to %f in y\n", a.y, b.y);
		for (y = a.Y; y < b.Y; y++)
                {
                        // midpoint to the right, upper part 
                        if (right.X - left.X > 0)
			{
                                dx = (right.X - left.X);
				dr = (right.Red - left.Red) / dx;
				dg = (right.Green - left.Green) / dx;
				db = (right.Blue - left.Blue) / dx;
			}
                        else 
                        {
                                dr = dg = db = 0;
                        }

			point.X = left.X;
                        point.Y = y;
                        point.Red = left.Red;
                        point.Green = left.Green;
                        point.Blue = left.Blue;


			for(; point.X < right.X; point.X++)
			{
                                point.X = ceilf (point.X);
                                device->SetPixel (point);
				point.Red += dr; 
                                point.Green += dg; 
                                point.Blue += db;
			}

                        left.X += dx1;
                        left.Red += dr1; 
                        left.Green += dg1; 
                        left.Blue += db1;
                        
                        right.X += dx2; 
                        right.Red += dr2; 
                        right.Green += dg2; 
                        right.Blue += db2;
                }
                
                left.X = b.X;
                left.Red = b.Red;
                left.Green = b.Green;
                left.Blue = b.Blue;

                // printf ("Step 1 - Drawing from %f to %f in y\n", b.y, c.y);
		for (; y < c.Y; y++)
                {
                         // midpoint to the right, lower part 
                        if (right.X - left.X > 0)
			{
                                dx = (right.X - left.X);
				dr = (right.Red - left.Red) / dx;
				dg = (right.Green - left.Green) / dx;
				db = (right.Blue - left.Blue) / dx;
			}
                        else 
                        {
                                dr = dg = db = 0;
                        }

			point.X = left.X;
                        point.Y = y;
                        point.Red = left.Red;
                        point.Green = left.Green;
                        point.Blue = left.Blue;

			for(; point.X < right.X; point.X++)
			{
                                point.X = ceilf (point.X);
				device->SetPixel (point);
				point.Red += dr; 
                                point.Green += dg; 
                                point.Blue += db;
			}

                        left.X += dx3; 
                        left.Red += dr3; 
                        left.Green += dg3; 
                        left.Blue += db3;

                        right.X += dx2; 
                        right.Red += dr2; 
                        right.Green += dg2; 
                        right.Blue += db2;
                }
	}
}
