Main/*******************************************************************\
*                                                                   *
* File:     TestDotDevice.cpp                                       *
* Author:   Knud Henriksen                                          *
* Created:  Thu Sep 16 22:20:22 1999                                *
* Contents: Test Program for class DotDevice.                       *
*                                                                   *
\*******************************************************************/

#include <iostream>
#include <iomanip>   // for flush
#include <cmath>

#ifdef _MSC_VER
  void sleep(int t) { for (int j=0; j<t*10000000; ++j) ; }
#else
#include <unistd.h>  // For sleep on *NIX. NOT easy on Win32 (?)
#endif

using std::cout; using std::endl; using std::flush;

#include "DotDevice.h"

/*******************************************************************\
*                                                                   *
*                  G l o b a l   V a r i a b l e s                  *
*                                                                   *
\*******************************************************************/

DotDevice* device;

/*******************************************************************\
*                                                                   *
*                    I d l e C a l l B a c k ( )                    *
*                                                                   *
\*******************************************************************/

void IdleCallBack()
{
  // do some computations here when we are idle
}

/*******************************************************************\
*                                                                   *
*                 D i s p l a y C a l l B a c k ( )                 *
*                                                                   *
\*******************************************************************/

void DisplayCallBack()
{
  cout << "-->DisplayCallBack" << endl << flush;
  
  device->Clear();
  device->Update();
  
  // Display some nice graphics here
  
  // Let one unit correspond to 50 pixels in the window
  device->UnitLength(50);
  
  cout << "   First Picture:" << endl;
  cout << "   ==============" << endl;
  if (GridVisible) {
    cout << "   A Black Grid on a White back ground" << endl;
    device->Grid(1, 1);
    device->Update();
    sleep(3);
  }
  else {
    cout << "   A White back ground" << endl;
  }
  
  cout << endl;
  cout << "   Second Picture:" << endl;
  cout << "   ===============" << endl;
  cout << "   Blue Pixel at (1, 2, -0.4) - visible through the grid" << endl;
  device->SetPixel(1, 2, -0.4, 0.0, 0.0, 1.0);
  device->Update();
  sleep(3);
  
  cout << "   Blue Pixel at (3, 5, -0.9) - visible through the grid" << endl;
  device->SetPixel(3, 5, -0.9, 0.0, 0.0, 1.0);
  device->Update();
  sleep(3);
  
  cout << endl;
  cout << "   Third Picture:" << endl;
  cout << "   ==============" << endl;
  cout << "   Draw White pixels in a plane with Z-coordinate -0.8" << endl;
  device->UnitLength(1);
  for (int x = 0; x < device->Width(); ++x) {
    for (int y = 0; y < device->Height(); ++y) {
      device->SetPixel(x, y, -0.8, 1.0, 1.0, 1.0);
    }
    device->Update();
  }
  device->Update();
  
  cout << endl;
  cout << "   Fourth and Final Picture:" << endl;
  cout << "   =========================" << endl;
  cout << "   Now only one blue pixel is visible through the grid" << endl;
  
  device->UnitLength(1);
  device->Text(50, 50, "Hej med dig!", 0.5, 0.5, 0.5);
  device->Update();
  
  cout << "<--DisplayCallBack" << endl << flush;
}

/*******************************************************************\
*                                                                   *
*                  m a i n ( i n t , c h a r * * )                  *
*                                                                   *
\*******************************************************************/

int main(int argc, char** argv)
{
  device = new DotDevice(500, 500, "DotDevice Example");
  
  // Initialize some of the commonly used call-back functions 
  // - there are more than these!
  
  //glutIdleFunc(IdleCallBack);
  glutDisplayFunc(DisplayCallBack);
  
  // do your own initialization here
  // ...
  
  GridVisible = 1;
  
  // For this DisplayCallBack function you should see:
  // 1) a red and green grid on a gray background.
  // 2) then two blue dots, one in front of the grid and one behind the grid.
  // 3) the blue dot behind the grid should be deleted slowly.
  cout << "For this DisplayCallBack function you should see:" << endl;
  cout << "=================================================" << endl;
  cout << "1) a red and green grid on a gray background."     << endl;
  cout << "2) then two blue dots, one in front of the grid and one ";
  cout << "behind the grid." << endl;
  cout << "3) the blue dot behind the grid should be deleted slowly."
    << endl << endl;
  cout << "In the window type:"                               << endl;
  cout << "==================="                               << endl;
  cout << "g - to toggle the grid on and off."                << endl;
  cout << "d - to redisplay the scene."                       << endl;
  cout << "q - to quit the program."                          << endl;
  cout << endl;
  
  // Let GLUT take over and rely on the call-back functions
  glutMainLoop();
  
  return 0;
}
