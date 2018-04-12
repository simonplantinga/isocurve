//############################################################################
// approximate.h
//############################################################################

#ifndef APPROXIMATE_H
#define APPROXIMATE_H

//#include "Interval.h"
#include <boost/numeric/interval.hpp>
#include "quadtree.h"
#include "curve.h"

#include "typeInterval.h"

//using namespace boost::numeric;
//using namespace interval_lib;
//typedef interval<double> Interval;

class Approximate
{
public:
  Approximate( Curve *c, bool show_squares );
  ~Approximate() {}
private:
  void IntervalSplit( QuadTree &T, QuadNode *node, Curve *c );
  bool IntervalTest( Curve *c, const Interval &ix, const Interval &iy );
  bool GetVertex( QuadNode *n, int dir, double &x, double &y );
  void ProcessCell( Graphics &g, QuadTree *T, QuadNode *node );
  void ComputeSigns( QuadTree &T, QuadNode *node, Curve *c );
};

#endif  // APPROXIMATE_H
