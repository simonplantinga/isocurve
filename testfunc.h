//############################################################################
#ifndef TESTFUNC_H
#define TESTFUNC_H
#include <boost/numeric/interval.hpp>
#include <fstream>
#include<math.h>
//#include <gmp.h>
//#include <mpfr.h>

//using namespace boost::numeric;
//typedef interval<double> Interval;
#include "typeInterval.h"

//############################################################################
// Abstract class for an implicit curve
//############################################################################

//-------------------------RBF-function---------------------------------------------------
class RbfFunc : public Curve
{
  int numPoints;
  double *ptx;
  double *pty;
  double *coeff;
  
public:
  RbfFunc() {}
  RbfFunc(char *filename) { ReadRbfFile(filename);}
  ~RbfFunc()
          {
          delete ptx;
          delete pty;
          delete coeff;
          cout<< "\nDestructor has been used\n";
          }

   void ReadRbfFile( char *filename ) {
      // open file
      ifstream fin;
      fin.open(filename);
      // read number of points from file
      if(!fin){
          cout<<"Unable to open file";
          exit(1);
          }
      else{
          cout<<"Reading File";
          
          // read number of points from file
          fin>>numPoints;
          // allocate memory for ptx, pty, coeff
          ptx = new double[numPoints];
          pty = new double[numPoints];
          coeff = new double[numPoints+3];
          // read data from file
          for(int i=0;i<numPoints;i++){
              fin>> ptx[i];
              fin>> pty[i];
              fin>>coeff[i];
              }
          fin.read((char*)& coeff[numPoints],sizeof(double));
          fin.read((char*)& coeff[numPoints+1],sizeof(double));
          fin.read((char*)& coeff[numPoints+2],sizeof(double));
          // close
          fin.close();
          }
       }
   
 

// compute function value or derivative at (x,y)
   double F(double x, double y) 
   {
      double r = coeff[numPoints]*x + coeff[numPoints+1]*y + coeff[numPoints+2];
      for (int i=0; i<numPoints; i++) {
        double dx = x - ptx[i];
        double dy = y - pty[i];
        double rad2 = dx*dx+dy*dy;
	double rad = sqrt(rad2);
	if (rad2==0.0)
            r+=0.0;
        else r+= coeff[i]*rad2*log(rad);
       }
      return (r);
    }
  
  double Fx(double x, double y)
  {   
      double r = coeff[numPoints];
      for (int i=0; i<numPoints; i++) {
        double dx = x - ptx[i];
        double dy = y - pty[i];
        double rad2 = dx*dx+dy*dy;
	double rad = sqrt(rad2);
        if (rad2==0.0)
            r+=0.0;
        else r+= coeff[i]*rad2*log(rad);
        }
      return (r);
  }

  double Fy(double x, double y)
  {   
      double r = coeff[numPoints+1];
      for (int i=0; i<numPoints; i++) {
        double dx = x - ptx[i];
        double dy = y - pty[i];
        double rad2 = dx*dx+dy*dy;
	double rad = sqrt(rad2);
	if (rad2==0.0)
            r+=0.0;
        else r+= coeff[i]*rad2*log(rad);
      }
      return (r);
  }

  // compute function value or derivative over 2D interval
  Interval F(Interval x, Interval y)
  {
      Interval r = coeff[numPoints]*x + coeff[numPoints+1]*y + coeff[numPoints+2];
      for (int i=0; i<numPoints; i++) {
        Interval dx = x - ptx[i];
        Interval dy = y - pty[i];
        Interval rad2 = square(dx)+square(dy);
	Interval rad = sqrt(rad2);
        r= r + coeff[i]*rad2*log(rad); 
      }
      return (r);
  }

  Interval Fx(Interval x, Interval y)
  {   
      Interval r = coeff[numPoints];
      for (int i=0; i<numPoints; i++) {
        Interval dx = x - ptx[i];
        Interval dy = y - pty[i];
        Interval rad2 = square(dx)+square(dy);
	Interval rad = sqrt(rad2);
        r=r+ coeff[i]*rad2*log(rad);
      }
      return (r);
   }

  Interval Fy(Interval x, Interval y)
  {   
      Interval r = coeff[numPoints+1];
      for (int i=0; i<numPoints; i++) {
        Interval dx = x - ptx[i];
        Interval dy = y - pty[i];
        Interval rad2 = square(dx)+square(dy);
	Interval rad = sqrt(rad2);
        r= r+coeff[i]*rad2*log(rad); 
      }
      return (r);
   }

  // return bounding box (square) of the curve
  double MinX() { /*cout<<CalcMin(numPoints,ptx)-0.7;*/ return CalcMin(numPoints,ptx)-0.7; }
  double MaxX() { return CalcMax(numPoints,ptx)+ 0.7; }
  double MinY() { return CalcMin(numPoints,pty)-0.7; }
  double MaxY() { return CalcMax(numPoints,pty)+ 0.7; }
  
  double CalcMin(int n,double *arr)
              {
              double min=10000000.0;
              
              for(int i=0;i<n;i++)
                  {
                  if(min>arr[i])
                      min=arr[i];
                  }
              return min;
              }
      double CalcMax(int n,double *arr)
              {
              double max=-10000000.0;
              for(int i=0;i<n;i++)
                  {
                  if(max<arr[i])
                      max=arr[i];
                  }
              return max;
              }
};

//----------------------------------------------------------------------------

#endif  // TESTFUNC_H
//############################################################################

