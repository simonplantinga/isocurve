#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

class RbfFunc 
{

  int numPoints;
  double *ptx;
  double *pty;
  double *coeff;
  
public:
  RbfFunc() {}
  RbfFunc(char *filename) { ReadRbfFile(filename);}
  ~RbfFunc() {cout<< "\nDestructor has been used\n";}

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
              fin>> coeff[i];
              }
          fin>> coeff[numPoints];
          fin>> coeff[numPoints+1];
          fin>> coeff[numPoints+2];
          // close
          fin.close();
          }
   }
    int getNumPoints()
    {
        return numPoints;
    }
    double getPtx(int i)
    {
        return ptx[i];
    }



};

int main(int argc, char *argv[]){

    RbfFunc f(argv[1]);

    cout <<f.getNumPoints();
    for(int i=0;i<f.getNumPoints();i++)
        cout<< f.getPtx(i)<<"\n";
    return 0;
    
    }

        

    

