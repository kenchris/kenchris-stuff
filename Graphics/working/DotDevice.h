// This may look like C code, but it is really -*- C++ -*-
#ifndef RM_GLDOTDEVICE_H
#define RM_GLDOTDEVICE_H
/*******************************************************************\
*                                                                   *
* File:     DotDevice.h                                             *
* Author:   Knud Henriksen                                          *
* Created:  Thu Sep 16 10:21:29 1999                                *
* Contents: Specification of an OpenGL Dot Device.                  *
*                                                                   *
\*******************************************************************/

#include <GL/glut.h>

typedef struct
{
        double X, Y;
        double Red, Green, Blue;
} RGBPoint;

/*******************************************************************\
*                                                                   *
*                  G l o b a l   V a r i a b l e s                  *
*                                                                   *
\*******************************************************************/

extern int GridVisible;

/*******************************************************************\
*                                                                   *
*  K e y b o a r d C a l l B a c k ( c h a r ,   i n t ,   i n t )  *
*                                                                   *
\*******************************************************************/

void KeyboardCallBack(unsigned char Key, int Xmouse, int Ymouse);

/*******************************************************************\
*                                                                   *
*         R e s h a p e C a l l B a c k ( i n t ,   i n t )         *
*                                                                   *
\*******************************************************************/

void ReshapeCallBack(int NewWidth, int NewHeight);

/*******************************************************************\
*                                                                   *
*                         D o t D e v i c e                         *
*                                                                   *
\*******************************************************************/

class DotDevice {
public:
  DotDevice(int Width=500, int Height=500, char* WindowTitle="DotDevice",
    int argc=1, char** argv=0);
  
  virtual ~DotDevice();
  
  void Clear();
  void Update();
  
  int  Width()  const;
  int  Height() const;
  
  int  UnitLength() const;
  int  UnitLength(int NewUnitLength);

  long GetDoubleDraws ();
  
  void SetPixel(int x, int y,
		  double red, double green, double blue);
  void SetPixel(int x, int y, double z, 
		  double red, double green, double blue);
  void SetPixel(RGBPoint point);
  
  void Text(int x, int y, char* text,
    double red = 0, double green = 0, double blue = 0);
  
  void Grid(int Xspacing = 1, int Yspacing = 1);
  
  void TestLine(int Xstart, int Ystart, int Xstop, int Ystop,
		  double red = 0, double green = 0, double blue = 0);
protected:
private:
  void Circle(int Xcenter, int Ycenter, double Zcenter, int Radius,
    double red = 0, double green = 0, double blue = 0);
  void Disk(int Xcenter, int Ycenter, double Zcenter, int Radius,
    double red = 0, double green = 0, double blue = 0);
  void CirclePoints(int Xcenter, int Ycenter, double Zcenter, int X, int Y);
  
  int  width;
  int  height;
  int  **testArray;
  long count;
  
  int  unitLength;
};

#endif
