//############################################################################
// quadtree.cpp
//
// copyright (c) 2003 by S.H. Plantinga
//############################################################################

#include "quadtree.h"

//----------------------------------------------------------------------------
// The child type is given by the bit mask 00yx, so we have
//
// +--+--+
// | 2| 3|
// +--+--+
// | 0| 1|
// +--+--+
//
// For the GetNeighbour function the directions are:
//
//    3
//    |
// 2 -+- 1
//    |
//    0
//----------------------------------------------------------------------------

//############################################################################
// QuadNode implementation
//############################################################################
QuadNode::QuadNode( QuadNode *myparent )
{
  parent = myparent;
  is_leaf = true;
  if ( myparent == NULL )
    depth = 0;
  else
    depth = myparent->depth + 1;
}

//----------------------------------------------------------------------------
QuadNode::~QuadNode()
{
  if ( !is_leaf )
    for ( int i=0; i<4; i++ )
      delete child[i];
}

//----------------------------------------------------------------------------
void
QuadNode::Split()
{
  Interval x1, x2, y1, y2;

//  x1 = Interval( x_range.lower(), x_range.median() );//inf() is changed to lower(), mid() is changed to median()
  x1 = Interval( x_range.lower(), median(x_range) );//inf() is changed to lower(), mid() is changed to median()
  x2 = Interval( median(x_range), x_range.upper() );// sup() is changed to upper()
  y1 = Interval( y_range.lower(), median(y_range) );
  y2 = Interval( median(y_range), y_range.upper() );

  for ( int i=0; i<4; i++ )
    {
      child[i] = new QuadNode( this );
      child[i]->SetChildType( i );
    }

  child[0]->SetRange( x1, y1 );
  child[1]->SetRange( x2, y1 );
  child[2]->SetRange( x1, y2 );
  child[3]->SetRange( x2, y2 );

  is_leaf = false;
}

//############################################################################
// QuadTree implementation
//############################################################################
QuadTree::QuadTree( const Interval &x, const Interval &y )
{
  root = new QuadNode( NULL );
  root->SetRange( x, y );
}

//----------------------------------------------------------------------------
QuadTree::~QuadTree()
{
  delete root;
}

//----------------------------------------------------------------------------
void
QuadTree::InsertLeaves( NodeList *l, QuadNode *n )
{
  if ( n->is_leaf )
    l->push( n );
  else
    {
      for ( int i=0; i<4; i++ )
        InsertLeaves( l, n->child[i] );
    }
}

//----------------------------------------------------------------------------
void
QuadTree::Balance()
{
  static int c1[4] = { 3, 2, 1, 0 };
  static int c2[4] = { 2, 0, 3, 1 };
  NodeList nodes;
  QuadNode *n;
  QuadNode *neighbour;

  // insert all the leaves of T into a linear list
  InsertLeaves( &nodes, root );

  // while list not empty
  while ( !nodes.empty() )
    {
      // remove a leaf from list
      n = nodes.top();
      nodes.pop();

      // test if leaf has to be split
      for ( int d=0; d<4; d++ )
        {
          neighbour = GetNeighbour( n, d );
          if ( neighbour != NULL 
                && !neighbour->is_leaf 
                && ( !neighbour->child[c1[d]]->is_leaf ||
                     !neighbour->child[c2[d]]->is_leaf ) )
            {
              // need to split this node
              n->Split();

              // add children to list
              for ( int i=0; i<4; i++ )
                nodes.push( n->child[i] );

              // check for neighbours that need splitting
              for ( int d=0; d<4; d++ )
                {
                  neighbour = GetNeighbour( n, d );
                  if ( neighbour != NULL && neighbour->depth < n->depth )
                    nodes.push( neighbour );
                }

              // don't need to test other directions
              break;
            }
        }
    }
}

//----------------------------------------------------------------------------
void
QuadTree::SplitNode( QuadNode *node )
{
  node->Split();
}

//----------------------------------------------------------------------------
QuadNode *
QuadTree::GetRoot()
{
  return root;
}

//----------------------------------------------------------------------------
QuadNode *
QuadTree::GetChild( QuadNode *node, int childtype )
{
  return node->child[childtype];
}

//----------------------------------------------------------------------------
QuadNode *
QuadTree::GetParent( QuadNode *node )
{
  return node->parent;
}
      
//----------------------------------------------------------------------------
// The child type is given by the bit mask 00yx, so we have
//
// +--+--+
// | 2| 3|
// +--+--+
// | 0| 1|
// +--+--+
//
// For the GetNeighbour function the directions are:
//
//    3
//    |
// 2 -+- 1
//    |
//    0

//----------------------------------------------------------------------------
// Return the deepest neighbour node whose depth is at most the depth of 
// *node, or NULL if there is no such node
QuadNode *
QuadTree::GetNeighbour( QuadNode *node, int direction )
{
  static int c1[4] = { 3, 2, 1, 0 };
  static int c2[4] = { 1, 3, 0, 2 };
  static int c3[4] = { 2, 0, 3, 1 };
  static int c4[4] = { 0, 1, 2, 3 };

  QuadNode *m;

  if ( node->parent == NULL )
    return NULL;
  
  if ( node->child_type == c1[direction] )
    return node->parent->child[c2[direction]];

  if ( node->child_type == c3[direction] )
    return node->parent->child[c4[direction]];

  m = GetNeighbour( node->parent, direction );
  if ( m == NULL || m->is_leaf )
    return m;

  if ( node->child_type == c2[direction] )
    return m->child[c1[direction]];
  else
    return m->child[c3[direction]];
}

//----------------------------------------------------------------------------
QuadNode *
QuadTree::GetCorner( QuadNode *node, int direction )
{
  while ( !node->is_leaf )
    node = node->child[direction];
  return node;
}

//----------------------------------------------------------------------------
// private member
void 
QuadTree::WriteNode( Graphics &g, QuadNode *node )
{
  if ( node->is_leaf )
    {
      g.Line( node->x_range.lower(), node->y_range.upper(),
              node->x_range.upper(), node->y_range.upper() );
      g.Line( node->x_range.upper(), node->y_range.upper(),
              node->x_range.upper(), node->y_range.lower() );
    }
  else
    {
      for ( int i=0; i<4; i++ )
        WriteNode( g, node->child[i] );
    }
}

//----------------------------------------------------------------------------
void
QuadTree::WriteToGraphics( Graphics &g )
{
  g.Line( root->x_range.lower(), root->y_range.lower(),
          root->x_range.lower(), root->y_range.upper() );
  g.Line( root->x_range.lower(), root->y_range.lower(),
          root->x_range.upper(), root->y_range.lower() );
  WriteNode( g, root );
}

//----------------------------------------------------------------------------

