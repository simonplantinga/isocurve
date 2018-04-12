#include <getopt.h>

#include "curve.h"
#include "quadtree.h"
#include "approximate.h"

#include "testfunc.h"

static int verbose_flag;    // Flag set by --verbose 


int main( int argc, char *argv[] )
{
  int c;

  while ( 1 )
    {
      static struct option long_options[] =
        {
          { "verbose",    no_argument,  &verbose_flag,  1 },
          { "brief",      no_argument,  &verbose_flag,  0 },
          { "min-depth",  required_argument,  0,  'd' },
          { "show-tree",  no_argument,        0,  's' },
          { 0, 0, 0, 0 }
        };

      int option_index = 0;

      c = getopt_long( argc, argv, "", long_options, &option_index );

      if ( c == -1 )
        break;
      
      switch ( c )
        {
        case '?':
          break;
        default:
          abort();
        }
    }

  //--------------------------------------------------------------------------
  RbfFunc f(argv[1]);
  // Curve f;
  

  Approximate a( &f, true );
  
  //--------------------------------------------------------------------------

   return 0;
}



