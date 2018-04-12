//############################################################################
#ifndef CURVE_H
#define CURVE_H

#include <boost/numeric/interval.hpp>
using namespace boost::numeric;
using namespace interval_lib;
//typedef interval<double> Interval;
#include "typeInterval.h"

//############################################################################
// Abstract class for an implicit curve
//############################################################################
class Curve
{
public:
  Curve() {}            // create curve 
  virtual ~Curve() {}   // destroy curve

  // compute function value or derivative at (x,y)
  virtual double F(double x, double y)= 0;
  virtual double Fx(double x, double y)= 0;
  virtual double Fy(double x, double y)= 0;

  // compute function value or derivative over 2D interval
  virtual Interval F(Interval x, Interval y) = 0;
  virtual Interval Fx(Interval x, Interval y)= 0;
  virtual Interval Fy(Interval x, Interval y)= 0;

  // return bounding box for the curve
  virtual double MinX() = 0;
  virtual double MaxX() = 0;
  virtual double MinY() = 0;
  virtual double MaxY() = 0;
};

#endif  // CURVE_H
//############################################################################

