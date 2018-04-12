//############################################################################
// approximate.cpp
//############################################################################

#include "approximate.h"
//using namespace std;
//using namespace boost::numeric::interval_lib;

Approximate::Approximate( Curve *c, bool show_squares )
{
  QuadTree T(Interval(c->MinX(), c->MaxX()), Interval(c->MinY(), c->MaxY()));

  IntervalSplit( T, T.GetRoot(), c );

  T.Balance();

  ComputeSigns( T, T.GetRoot(), c );
  
  Graphics g( c->MinX(), c->MaxX(), c->MinY(), c->MaxY() );

  if ( show_squares )
    {
      g.SetGrey( 0.5 );
      T.WriteToGraphics( g );
    }

  g.SetGrey( 0.0 );
  ProcessCell( g, &T, T.GetRoot() );
}

bool 
Approximate::IntervalTest( Curve *c, const Interval &ix, const Interval &iy )
{
  Interval ifx, ify, I;

  ifx = c->Fx( ix, iy );
  ify = c->Fy( ix, iy );
  I = ifx * ifx + ify * ify;
  if ( in_zero(I) )//contains( 0.0 ) is replaced by zero_in(I)
    return false;
  else
    return true;
}

void 
Approximate::IntervalSplit( QuadTree &T, QuadNode *node, Curve *c )
{
  Interval ix, iy;

  node->GetRange( ix, iy );

  // if function is non-zero we're done
  //if ( !c->F( ix, iy ).contains( 0.0 ) )
  if ( !in_zero(c->F( ix, iy )) )
    return;

  // perform interval test
  if ( IntervalTest( c, ix, iy ) )
    return;
/*
    {
      // compute signs
      int signs = 0;
      if ( c->F(ix.sup(), iy.sup() ) >= 0 ) signs |= 1;
      if ( c->F(ix.inf(), iy.sup() ) >= 0 ) signs |= 2;
      if ( c->F(ix.inf(), iy.inf() ) >= 0 ) signs |= 4;
      if ( c->F(ix.sup(), iy.inf() ) >= 0 ) signs |= 8;
      node->SetData( signs );
      return;
    }
*/
  T.SplitNode( node );
  for ( int i=0; i<4; i++ )
    IntervalSplit( T, T.GetChild( node, i ), c );
}

bool
Approximate::GetVertex( QuadNode *n, int dir, double &x, double &y )
{
  Interval xr, yr;
  int signs = n->GetData();

  n->GetRange( xr, yr );

  switch ( dir )
    {
    case 0:
      signs &= 12;
      if ( signs == 4 || signs == 8 )
        {
          x = median(xr);
          y = yr.lower();
          return true;
        }
      break;

    case 1:
      signs &= 9;
      if ( signs == 1 || signs == 8 )
        {
          x = xr.upper();
          y = median(yr);
          return true;
        }
      break;
      
    case 2:
      signs &= 6;
      if ( signs == 2 || signs == 4 )
        {
          x = xr.lower();
          y = median(yr);
          return true;
        }
      break;

    case 3:
      signs &= 3;
      if ( signs == 1 || signs == 2 )
        {
          x = median(xr);
          y = upper(yr);
          return true;
        }
      break;
    }
  return false;
}

void
Approximate::ProcessCell( Graphics &g, QuadTree *T, QuadNode *node )
{
  if ( !node->IsLeaf() )
    {
      for ( int i=0; i<4; i++ )
        ProcessCell( g, T, T->GetChild( node, i ) );
      return;
    }

  // compute border vertices ccw starting north
  double xb[4];
  double yb[4];
  int    side[4];
  int    num = 0;
  QuadNode *n;
  double x, y;
  static int dir[4]  = { 3, 2, 0, 1 };    // direction
  static int odir[4] = { 0, 1, 3, 2 };    // opposite direction
  static int c1[4]   = { 1, 3, 2, 0 };
  static int c2[4]   = { 0, 1, 3, 2 };

  for ( int d=0; d<4; d++ )
    {
      n = T->GetNeighbour( node, dir[d] );
      if ( n == NULL )
        continue;
      if ( n->IsLeaf() )
        {
          // examine this cell's edge
          if ( GetVertex( node, dir[d], x, y ) )
            {
              xb[num] = x;
              yb[num] = y;
              side[num] = d;
              num++;
            }            
        }
      else
        {
          // examine two neighbouring cells
          if ( GetVertex( T->GetChild(n, c1[d]), odir[d], x, y ) )
            {
              xb[num] = x;
              yb[num] = y;
              side[num] = d;
              num++;
            }            
          if ( GetVertex( T->GetChild(n, c2[d]), odir[d], x, y ) )
            {
              xb[num] = x;
              yb[num] = y;
              side[num] = d;
              num++;
            }        
        }
    }
  // connect vertices
  if ( num == 2 )
    g.Line( xb[0], yb[0], xb[1], yb[1] );
  else if ( num == 4 )
    {
      int v1, v2, v3, v4;
      if ( side[0]==side[1] )
        {
          v1 = 0;
          v2 = 1;
          v3 = 2;
          v4 = 3;
        }
      else if ( side[1]==side[2] )
        {
          v1 = 1;
          v2 = 2;
          v3 = 3;
          v4 = 0;
        }
      else 
        {
          v1 = 2;
          v2 = 3;
          v3 = 0;
          v4 = 1;
        }
      g.Line( xb[v1], yb[v1], xb[v4], yb[v4] );
      g.Line( xb[v2], yb[v2], xb[v3], yb[v3] );
    }
  else
    {
      if ( num > 0 )
        {
          Interval x, y;
          cerr << "ERROR: " << num << " vertices\n";
          node->GetRange(x,y);
          //cerr << "node " << x << " " << y << endl;
          //cerr << "side " << side[0] << "point " << xb[0] << " " << yb[0] << endl;
        }
    }
}

void
Approximate::ComputeSigns( QuadTree &T, QuadNode *node, Curve *c )
{
  if ( node->IsLeaf() )
    {
      // compute signs
      int signs = 0;
      Interval ix, iy;
      node->GetRange( ix, iy );
      if ( c->F(ix.upper(), iy.upper() ) >= 0 ) signs |= 1;
      if ( c->F(ix.lower(), iy.upper() ) >= 0 ) signs |= 2;
      if ( c->F(ix.lower(), iy.lower() ) >= 0 ) signs |= 4;
      if ( c->F(ix.upper(), iy.lower() ) >= 0 ) signs |= 8;
      node->SetData( signs );
    }
  else
    {
      for ( int i=0; i<4; i++ )
        ComputeSigns( T, T.GetChild( node, i ), c );
      
    }
}

