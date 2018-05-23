#include "B2MagneticField.hh"
#include "G4MagneticField.hh"
#include "globals.hh"
#include <math.h>
#include <fstream.h>

B2MagneticField::B2MagneticField()
{
  Xmin = -2.*m;
  Xmax = 2.*m;
  Ymin = -1.5*m;
  Ymax = 1.5*m;
  Zmin = 0*m;
  Zmax = 4*m;
  double tmp;
  ifstream data("data/magfield.table");
  for(int i=0;i<Xbin;i++){
    for(int j=0;j<Ybin;j++){
      data>>tmp>>tmp>>tmp>>Bx[i][j]>>By[i][j]>>Bz[i][j];
    }
  }
}

B2MagneticField::~B2MagneticField()
{
}

void B2MagneticField::GetFieldValue(const G4double Point[3], G4double* Bfield) const
{
  Bfield[0]=0.*tesla;
  Bfield[1]=0.*tesla;
  Bfield[2]=0.*tesla;

  G4double x = Point[0]-45.*cm;
  G4double y = Point[1];	
  G4double z = Point[2]-(275.-200)*cm;

  if(x>Xmin&&x<Xmax&&y>Ymin&&y<Ymax&&z>Zmin&&z<Zmax){

    int xint = (int)(x-Xmin)/(Xmax-Xmin)*Xbin;
    int yint = (int)(y-Ymin)/(Ymax-Ymin)*Ybin;
    Bfield[0] = Bx[xint][yint]*tesla;
    Bfield[1] = By[xint][yint]*tesla;
    Bfield[2] = Bz[xint][yint]*tesla;
  }

}

