#include <iostream>
#include <iomanip>   // for flush
#include <cmath>

#include "DotDevice.h"
#include "ScanConversion.h"

using std::cout; using std::endl; using std::flush;

DotDevice* device;

void Canvas_IdleEvent()
{
        // do some computations here when we are idle
}

void TestGouraud1 ()
{
        RGBPoint a, b, c;

        a.X = 110; a.Y = 100; a.Red = 1, a.Green = 0, a.Blue = 0;
        b.X = 100; b.Y = 140; b.Red = 1, b.Green = 0, b.Blue = 0;
        c.X = 160; c.Y = 150; c.Red = 1, c.Green = 0, c.Blue = 0;

        DrawGouraudTriangle (device, a, b, c);

        a.X = 100; a.Y = 140; a.Red = 0, a.Green = 0, a.Blue = 0;
        b.X =  90; b.Y = 180; b.Red = 0, b.Green = 0, b.Blue = 0;
        c.X = 150; c.Y = 190; c.Red = 0, c.Green = 0, c.Blue = 0;
        
        DrawGouraudTriangle (device, a, b, c);

        a.X = 100; a.Y = 140; a.Red = 0, a.Green = 1, a.Blue = 0;
        b.X = 160; b.Y = 150; b.Red = 0, b.Green = 1, b.Blue = 0;
        c.X = 150; c.Y = 190; c.Red = 0, c.Green = 1, c.Blue = 0;
        
        DrawGouraudTriangle (device, a, b, c);

        a.X = 160; a.Y = 150; a.Red = 0, a.Green = 0, a.Blue = 1;
        b.X = 150; b.Y = 190; b.Red = 0, b.Green = 0, b.Blue = 1;
        c.X = 210; c.Y = 200; c.Red = 0, c.Green = 0, c.Blue = 1;
        
        DrawGouraudTriangle (device, a, b, c);

        a.X =  50; a.Y =  90; a.Red = 0, a.Green = 0, a.Blue = 1;
        b.X = 110; b.Y = 100; b.Red = 0, b.Green = 0, b.Blue = 1;
        c.X = 100; c.Y = 140; c.Red = 0, c.Green = 0, c.Blue = 1;
        
        DrawGouraudTriangle (device, a, b, c);

        a.X = 110; a.Y = 100; a.Red = 0, a.Green = 0, a.Blue = 0;
        b.X = 170; b.Y = 110; b.Red = 0, b.Green = 0, b.Blue = 0;
        c.X = 160; c.Y = 150; c.Red = 0, c.Green = 0, c.Blue = 0;
        
        DrawGouraudTriangle (device, a, b, c);

}

void TestGouraud2 ()
{
        RGBPoint a, b, c;

        a.X = 100; a.Y = 100; a.Red = 1, a.Green = 0, a.Blue = 0;
        b.X = 350; b.Y = 130; b.Red = 0, b.Green = 1, b.Blue = 0;
        c.X = 220; c.Y = 360; c.Red = 0, c.Green = 0, c.Blue = 1;

        DrawGouraudTriangle (device, a, b, c);

        a.X = 370; a.Y = 400; a.Red = 1, a.Green = 0, a.Blue = 0;

        DrawGouraudTriangle (device, b, c, a);
}

void TestGouraud3 ()
{
        RGBPoint a, b, c;

        a.X = 100; a.Y =  50; a.Red = 1, a.Green = 0, a.Blue = 0;
        b.X =  50; b.Y = 250; b.Red = 0, b.Green = 1, b.Blue = 0;
        c.X = 250; c.Y = 240; c.Red = 0, c.Green = 0, c.Blue = 1;

        DrawGouraudTriangle (device, a, c, b);

        a.X = 100; a.Y =  50; a.Red = 1, a.Green = 0, a.Blue = 0;
        b.X = 300; b.Y =  40; b.Red = 0, b.Green = 1, b.Blue = 0;
        c.X = 250; c.Y = 240; c.Red = 0, c.Green = 0, c.Blue = 1;

        DrawGouraudTriangle (device, b, a, c);
}

void Canvas_RedrawEvent()
{
        device->Clear();
        device->Update();

        device->UnitLength(1);

        TestGouraud3 ();

        cout << device->GetDoubleDraws () << "\n";
}

int 
main (int argc, char **argv)
{
        device = new DotDevice (500, 500, "3DRender");

        glutDisplayFunc (Canvas_RedrawEvent);        
        
        glutMainLoop();
        
        return 0;
}
