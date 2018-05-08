#include "G4MagneticField.hh"
#include "globals.hh"

#define Xbin 251
#define Ybin 201

class B2MagneticField : public G4MagneticField{
public:
  virtual void GetFieldValue(const G4double Point[3], G4double* Bfield) const;  
  B2MagneticField();
  ~B2MagneticField();
  double Bx[Xbin][Ybin],By[Xbin][Ybin],Bz[Xbin][Ybin];
  double Xmin,Xmax,Ymin,Ymax,Zmin,Zmax;
};
