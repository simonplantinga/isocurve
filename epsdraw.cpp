//############################################################################
// EPSDRAW.CPP
// Creates Graphics class to print eps files
//

#include "epsdraw.h"

//----------------------------------------------------------------------------
// constructor: print eps header
Graphics::Graphics( double xmin, double xmax, double ymin, double ymax )
{ 
  x_min=xmin; x_max=xmax; y_min=ymin; y_max=ymax; 
  cout << "\%!PS-Adobe-2.0 EPSF-2.0\n";
  cout << "\%\%CreationDate: 00/00/00\n";
  cout << "\%\%BoundingBox: 0 0 " << SCALE << " " << SCALE << endl;
  cout << "\%\%EndComments\n";
}

//----------------------------------------------------------------------------
// destructor: print eps closing info
Graphics::~Graphics()
{
  cout << "showpage\n";
}

//----------------------------------------------------------------------------
// draw line from (x1,y1) to (x2,y2)
void Graphics::Line( double x1, double y1, double x2, double y2 )
{
  double x_1 = ( x1 - x_min ) / ( x_max - x_min );
  double x_2 = ( x2 - x_min ) / ( x_max - x_min );
  double y_1 = ( y1 - y_min ) / ( y_max - y_min );
  double y_2 = ( y2 - y_min ) / ( y_max - y_min );
//  cout << (int)(x_1*SCALE) << " " << (int)(y_1*SCALE) << " moveto\n";
//  cout << (int)(x_2*SCALE) << " " << (int)(y_2*SCALE) << " lineto\n";
  cout << (x_1*SCALE) << " " << (y_1*SCALE) << " moveto\n";
  cout << (x_2*SCALE) << " " << (y_2*SCALE) << " lineto\n";
  cout << "stroke\n";
}

//----------------------------------------------------------------------------
// draw dot at (x,y)
void Graphics::Dot( double x, double y )
{
  double xp = ( x - x_min ) / ( x_max - x_min );
  double yp = ( y - y_min ) / ( y_max - y_min );
  int xi = (int)( xp * SCALE );
  int yi = (int)( yp * SCALE );
  cout << xi-1 << " " << yi-1 << " moveto\n";
  cout << xi+1 << " " << yi-1 << " lineto\n";
  cout << xi+1 << " " << yi+1 << " lineto\n";
  cout << xi-1 << " " << yi+1 << " lineto\n";
  cout << "closepath\n";
  cout << "stroke\n";
}


