//############################################################################

#ifndef QUADTREE_H
#define QUADTREE_H

//#include "Interval.h"
#include <boost/numeric/interval.hpp>
#include "epsdraw.h"

#include <stack>
using namespace std;
using namespace boost::numeric;
using namespace interval_lib;
//typedef interval<double> Interval;
#include "typeInterval.h"

class QuadNode;
class QuadTree;


typedef stack<QuadNode *> NodeList;


//----------------------------------------------------------------------------
// Node of the quadtree
class QuadNode
{
public:
  QuadNode( QuadNode *myparent );
  ~QuadNode();
  void SetRange( const Interval &x, const Interval &y )
    { x_range = x; y_range = y; }
  void GetRange( Interval &x, Interval &y )
    { x = x_range; y = y_range; }
  void SetChildType( int tp ) { child_type = tp; }
  int  GetChildType() { return child_type; }
  bool IsLeaf() { return is_leaf; }
  int  GetDepth() { return depth; }
  void SetData( int d ) { data = d; }
  int  GetData() { return data; }
  void Split();
private:
  QuadNode *parent;
  int       child_type;
  QuadNode *child[4];   // index using bits: 00yx, e.g. northeast=3
  bool      is_leaf;
  int       depth;
  Interval  x_range;
  Interval  y_range;
  int       data;

  friend class QuadTree;
};

//----------------------------------------------------------------------------
class QuadTree
{
public:
  QuadTree( const Interval &x, const Interval &y );
  ~QuadTree();
  void Balance();
  void SplitNode( QuadNode *node );
  QuadNode *GetRoot();
  QuadNode *GetChild( QuadNode *node, int childtype );
  QuadNode *GetParent( QuadNode *node );
  QuadNode *GetNeighbour( QuadNode *node, int direction );
  QuadNode *GetCorner( QuadNode *node, int direction );
  void WriteToGraphics( Graphics &g );
private:
  void WriteNode( Graphics &g, QuadNode *node );
  void InsertLeaves( NodeList *l, QuadNode *n );

  QuadNode *root;
};

#endif  // QUADTREE_H

