// This may look like C code, but it is really -*- C++ -*-
/*******************************************************************\
*                                                                   *
* File:     DotDevice.cpp                                           *
* Author:   Knud Henriksen                                          *
* Created:  Thu Okt 30 10:21:29 2002                                *
* Contents: Implementation of an OpenGL Dot Device.                 *
*                                                                   *
\*******************************************************************/

#include <stdlib.h> // exit
#include <iostream>
#include <iomanip>  // for flush
#include <cstring>  // for strlen

#include "DotDevice.h"

using std::cerr; using std::endl; using std::flush;
// Hmm, shouldn't this be in namespace std?
#ifndef _MSC_VER
  using std::strlen;
#endif

/*******************************************************************\
*                                                                   *
*                  G l o b a l   V a r i a b l e s                  *
*                                                                   *
\*******************************************************************/

static int OrgWidth    = 500;          // Original Window width
static int OrgHeight   = 500;          // Original Window height

int GridVisible        = 0;

/*******************************************************************\
*                                                                   *
*  K e y b o a r d C a l l B a c k ( c h a r ,   i n t ,   i n t )  *
*                                                                   *
\*******************************************************************/

void KeyboardCallBack(unsigned char Key, int Xmouse, int Ymouse)
{
  // cout << "You typed " << Key << endl << flush;
  switch (Key) {
  case 'q':
  case 'Q':
    // quit the program
    
    exit(0);
    break;
  case 'd':
  case 'D':
    // call the DisplayCallback as soon as possible
    
    glutPostRedisplay();
    break;  
  case 'g':
  case 'G':
    // toggle the grid on and off
    
    GridVisible = GridVisible ? 0 : 1;
    glutPostRedisplay();
    break;
  }
}

/*******************************************************************\
*                                                                   *
*         R e s h a p e C a l l B a c k ( i n t ,   i n t )         *
*                                                                   *
\*******************************************************************/

void ReshapeCallBack(int NewWidth, int NewHeight)
{
  static int visits = 0;
  
  // It is a very bad idea to reshape the window, because then the
  // mapping is no longer one on one, i.e. coordinate (5, 35) correspond
  // to pixel (5, 35) in the window. 
  // If you do a reshape you are asking for trouble!
  
  ++visits;
  if (visits == 1) {
    OrgWidth  = NewWidth;
    OrgHeight = NewHeight;
  }
  else {
    glutReshapeWindow(OrgWidth, OrgHeight);
    //glutPostRedisplay();
  }
}

/*******************************************************************\
*                                                                   *
*    D o t D e v i c e ( i n t , i n t , i n t , i n t . . . )      *
*                                                                   *
\*******************************************************************/

DotDevice::DotDevice(int Width, int Height, char* WindowTitle,
		     int argc, char** argv)
         : width(Width), height(Height), unitLength(1)
{
  // Initialize OpenGL to draw individual pixels in an X-window
  //
  // Create a window with Width x Height pixels which are addressed
  // by x = 0,..., Width-1 and y = 0,..., Height-1
  //
  // If the window is reshaped the above one on one mapping is destroyed
  // and everything will fail - you are warned!
  
  int tmpargc = argc;
  if (!argv) {
    char* tmpargv[] = {(char*) "Windows is a shipload of crap..."};
    glutInit(&tmpargc, tmpargv);
  }
  else {
    glutInit(&tmpargc, argv);
  }
  glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH); 
  if (argc <= 1) {
    glutInitWindowSize(Width, Height);
  }
  glutCreateWindow(WindowTitle); 
  glClearColor(1.0, 1.0, 1.0, 0.0);      
  
  glEnable(GL_DEPTH_TEST);
  glClearDepth(1.0);          // Depths are distances - NOT Z-coordinates!
  glDepthFunc(GL_LEQUAL);     // How to compare values in the Z-buffer
  // Change it as you like, e.g. GL_LESS
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  // Near = 0 and Far = 1 are distances from the viewpoint along the
  // negative Z-axis. So Near = 0 and Far = 1 means that the Z-coordinates
  // must be in the range [-1, 0]
  
  glOrtho(0.0, GLdouble(Width), 0.0, GLdouble(Height), 0.0, 1.0);
  glTranslated(0.25, 0.25, 0.0);    // hack to make both HP and ALPHA work
  glViewport(0, 0, GLsizei(Width), GLsizei(Height));
  
  glutKeyboardFunc(KeyboardCallBack);
  glutReshapeFunc(ReshapeCallBack);
  
  OrgWidth  = Width;
  OrgHeight = Height;

  testArray = new int *[Height];

  for (int i = 0; i < Height; i++)
  {
          testArray[i] = new int[Width];

          for (int j = 0; j < Width; j++)
          {
                  testArray[i][j] = 0;
          }
  }
  count = 0;
  
  this->Clear();
  this->Update();
}

/*******************************************************************\
*                                                                   *
*                      ~ D o t D e v i c e ( )                      *
*                                                                   *
\*******************************************************************/

DotDevice::~DotDevice()
{}

/*******************************************************************\
*                                                                   *
*                           C l e a r ( )                           *
*                                                                   *
\*******************************************************************/

void DotDevice::Clear()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

long DotDevice::GetDoubleDraws ()
{
        return count;
}

/*******************************************************************\
*                                                                   *
*                          U p d a t e ( )                          *
*                                                                   *
\*******************************************************************/

void DotDevice::Update()
{
  count = 0;

  glFinish();
  
  int AnyErrors = 0;
  GLenum ErrorCode;
  while ((ErrorCode = glGetError()) != GL_NO_ERROR) {
    cerr << (const char*) gluErrorString(ErrorCode) << endl << flush;
    AnyErrors = 1;
  }
  if (AnyErrors) {
    //throw runtime_error("DotDevice::Update(): OpenGL rendering failure");
    cerr << "DotDevice::Update(): OpenGL rendering failure" << endl;
    exit(-1);
  }
}

/*******************************************************************\
*                                                                   *
*                           W i d t h ( )                           *
*                                                                   *
\*******************************************************************/

int DotDevice::Width() const
{
  return this->width;
}

/*******************************************************************\
*                                                                   *
*                          H e i g h t ( )                          *
*                                                                   *
\*******************************************************************/

int DotDevice::Height() const
{
  return this->height;
}

/*******************************************************************\
*                                                                   *
*                      U n i t L e n g t h ( )                      *
*                                                                   *
\*******************************************************************/

int DotDevice::UnitLength() const
{
  return this->unitLength;
}

/*******************************************************************\
*                                                                   *
*                   U n i t L e n g t h ( i n t )                   *
*                                                                   *
\*******************************************************************/

int DotDevice::UnitLength(int NewUnitLength)
{
  int OldUnitLength = this->unitLength;
  this->unitLength  = NewUnitLength;
  return OldUnitLength;
}

/*******************************************************************\
*                                                                   *
*      S e t P i x e l ( i n t , i n t ,   3  x  d o u b l e )      *
*                                                                   *
\*******************************************************************/

void DotDevice::SetPixel(int x, int y, double red, double green, double blue)
{
  glColor3d(red, green, blue);
  if (this->unitLength > 1)
    Disk(x * this->unitLength, y * this->unitLength, 0.0,
    (this->unitLength - 1) / 2, red, green, blue);
  else {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
  }
}

/*******************************************************************\
*                                                                   *
*      S e t P i x e l ( R G P P o i n t )                          *
*                                                                   *
\*******************************************************************/

void DotDevice::SetPixel(RGBPoint point)
{
        glColor3d (point.Red, point.Green, point.Blue);

        if (this->unitLength > 1)
        {
                Disk ((int) point.X * this->unitLength, 
                      (int) point.Y * this->unitLength, 
                      0.0,
                      (this->unitLength - 1) / 2, 
                      point.Red, point.Green, point.Blue);
        }
        else 
        {
                glBegin(GL_POINTS);
                glVertex2i((int) point.X, (int) point.Y);
                glEnd();
        }

        testArray[(int) point.X][(int) point.Y] += 1;
        if (testArray[(int) point.X][(int) point.Y] > 1)
        {
                count++;
        }
        
}

/*******************************************************************\
*                                                                   *
*S e t P i x e l ( i n t , i n t , d o u b l e , 3  x  d o u b l e )*
*                                                                   *
\*******************************************************************/

void DotDevice::SetPixel(int x, int y, double z, 
                         double red, double green, double blue)
{
  glColor3d(red, green, blue);
  if (this->unitLength > 1)
    Disk(x * this->unitLength, y * this->unitLength, z,
    (this->unitLength - 1) / 2, red, green, blue);
  else {
    glBegin(GL_POINTS);
    glVertex3d(double(x), double(y), z);
    glEnd();
  }
}

/*******************************************************************\
*                                                                   *
* T e x t ( i n t ,   i n t ,   c h a r * ,   3   x   d o u b l e ) *
*                                                                   *
\*******************************************************************/

void DotDevice::Text(int x, int y, char* text,
                     double red, double green, double blue)
{
  glColor3d(red, green, blue);
  glLineWidth(2.0);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslated(GLdouble(x), GLdouble(y), GLdouble(0));
  glScaled(0.125, 0.125, 0.125);
  int ch;
  int length = strlen(text);
  for (int i = 0; i < length; ++i) {
    ch = text[i];
    glutStrokeCharacter(GLUT_STROKE_ROMAN, ch);
  }
  glPopMatrix();
  glLineWidth(1.0);
}

/*******************************************************************\
*                                                                   *
*                     G r i d ( i n t , i n t )                     *
*                                                                   *
\*******************************************************************/

void DotDevice::Grid(int Xspacing, int Yspacing)
{
  glColor3d(0.0, 0.0, 0.0);
  int yspacing = this->unitLength * Yspacing;
  glBegin(GL_LINES);
  for (int y = 0; y < this->height; y += yspacing) {
    glVertex3d(0, double(y), 0.0);
    glVertex3d(double(this->width-1), double(y), 0.0);
  }
  glEnd();
  glColor3d(0.0, 0.0, 0.0);
  int xspacing = this->unitLength * Xspacing;
  glBegin(GL_LINES);
  for (int x = 0; x < this->width; x += xspacing) {
    glVertex3d(double(x), 0.0, 0.0);
    glVertex3d(double(x), double(this->height-1), 0.0);
  }
  glEnd();
}

/*******************************************************************\
*                                                                   *
*     T e s t L i n e ( 4   x   i n t ,   3   x   d o u b l e )     *
*                                                                   *
\*******************************************************************/

void DotDevice::TestLine(int Xstart, int Ystart, int Xstop, int Ystop,
                         double red, double green, double blue)
{
  glColor3d(red, green, blue);
  glBegin(GL_LINES);
  glVertex2i(Xstart * this->unitLength, Ystart * this->unitLength);
  glVertex2i(Xstop  * this->unitLength, Ystop  * this->unitLength);
  glEnd();
}

/*******************************************************************\
*                                                                   *
* C i r c l e ( 2 x i n t , d o u b l e , i n t , 3 x d o u b l e ) *
*                                                                   *
\*******************************************************************/

void DotDevice::Circle(int Xcenter, int Ycenter, double Zcenter, int Radius,
                       double red, double green, double blue)
{
  int X = 0;
  int Y = Radius;
  int d = 1 - Radius;
  int deltaE  = 3;
  int deltaSE = -2 * Radius + 5;
  
  glColor3d(red, green, blue);
  glBegin(GL_POINTS);
  CirclePoints(Xcenter, Ycenter, Zcenter, X, Y);
  
  while (Y > X) {
    if (d < 0) {
      d += deltaE;
      deltaSE += 2;
    }
    else {
      d += deltaSE;
      deltaSE += 4;
      --Y;
    }
    ++X;
    deltaE += 2;
    CirclePoints(Xcenter, Ycenter, Zcenter, X, Y);
  }
  glEnd();
}

/*******************************************************************\
*                                                                   *
*   D i s k ( 2 x i n t , d o u b l e , i n t , 3 x d o u b l e )   *
*                                                                   *
\*******************************************************************/

void DotDevice::Disk(int Xcenter, int Ycenter, double Zcenter, int Radius,
                     double red, double green, double blue)
{
  int X = 0;
  int Y = Radius;
  int d = 1 - Radius;
  int deltaE  = 3;
  int deltaSE = -2 * Radius + 5;
  
  glColor3d(red, green, blue);
  glBegin(GL_LINES);
  CirclePoints(Xcenter, Ycenter, Zcenter, X, Y);
  
  while (Y > X) {
    if (d < 0) {
      d += deltaE;
      deltaSE += 2;
    }
    else {
      d += deltaSE;
      deltaSE += 4;
      --Y;
    }
    ++X;
    deltaE += 2;
    CirclePoints(Xcenter, Ycenter, Zcenter, X, Y);
  }
  glEnd();
}

/*******************************************************************\
*                                                                   *
*     C i r c l e P o i n t s ( i n t , i n t , i n t , i n t )     *
*                                                                   *
\*******************************************************************/

void DotDevice::CirclePoints(int Xcenter, int Ycenter, double Zcenter,
                             int X, int Y)
{
  glVertex3d(double(Xcenter - X), double(Ycenter - Y), Zcenter);
  glVertex3d(double(Xcenter + X), double(Ycenter - Y), Zcenter);
  glVertex3d(double(Xcenter - X), double(Ycenter + Y), Zcenter);
  glVertex3d(double(Xcenter + X), double(Ycenter + Y), Zcenter);
  glVertex3d(double(Xcenter - Y), double(Ycenter - X), Zcenter);
  glVertex3d(double(Xcenter + Y), double(Ycenter - X), Zcenter);
  glVertex3d(double(Xcenter - Y), double(Ycenter + X), Zcenter);
  glVertex3d(double(Xcenter + Y), double(Ycenter + X), Zcenter);
}
