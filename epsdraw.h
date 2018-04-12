//############################################################################
// EPSDRAW.H
// Creates Graphics class to print eps files
//
#ifndef EPSDRAW_H
#define EPSDRAW_H

#include <iostream>
using namespace std;

#define SCALE (6*72)      // size of image in eps points

//----------------------------------------------------------------------------
class Graphics
{
public:
  // create eps file
  Graphics( double xmin, double xmax, double ymin, double ymax );

  // close eps file
  ~Graphics();

  // set grey level (0.0 = black, 1.0 = white)
  void SetGrey( double grey )
    { cout << grey << " setgray\n"; }

  // draw line from (x1,y1) to (x2,y2)
  void Line( double x1, double y1, double x1, double y2 );

  // draw a small square dot at (x,y)
  void Dot( double x, double y );
private:
  double x_min, x_max, y_min, y_max;
};

#endif    // EPSDRAW_H

