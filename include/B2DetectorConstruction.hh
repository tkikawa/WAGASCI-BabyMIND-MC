#ifndef B2DetectorConstruction_H
#define B2DetectorConstruction_H 1

#include "B2HLayerSD.hh"
#include "B2VetoSD.hh"
#include "B2VLayerSD.hh"
#include "B2_Dimension.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

#include "G4VUserDetectorConstruction.hh"

#define nSciVertex 8

class B2DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  void DefineMaterial();
  void DefineSpace();
  void DefineStructures();

  G4Material *Air, *Fe, *Scinti, *Water, *Concrete, *SUS304;
  G4double WorldX, WorldY, WorldZ;
  G4double HallDirtRadiusMin, HallDirtRadiusMax, HallDirtHeight,
           HallDirtSPhi, HallDirtDPhi; //## Dirt Dimension
  G4double HallDirtX, HallDirtY, HallDirtZ; //## Dirt position
  G4double HallX, HallZ; //## center position of Hall
  G4double Floorup; 
  G4double PillarX,PillarY,PillarZ; 
  G4double PillarposX,PillarposY,PillarposZ,PillarposX_rev; 

  G4double iron_z,iron_xy, Gap, iron_start;
  G4double scibar_x, scibar_y,scibar_z, scibar_start, scibar_xy_start,Pscibar_start;
  G4double Lveto_x, Lveto_y, Lveto_z, veto_start, Pveto_start; 
  G4double Sveto_x, Sveto_y, Sveto_z;
  G4double Niron_start, Niron_z, Nscibar_start, NGap;
  G4double SciVertex_x[nSciVertex], SciVertex_y[nSciVertex];
  G4double SciVertex2_x[nSciVertex], SciVertex2_y[nSciVertex];
  G4double SciVertex3_x[nSciVertex], SciVertex3_y[nSciVertex];
  G4double SciVertex4_x[nSciVertex], SciVertex4_y[nSciVertex];

  char name[10], vetoname[4][22], ironname[30];

  //Proton Module
  G4double scibar2_y,scibar2_z;//scibar_start, scibar_xy_start;
  G4double PLveto_x, PLveto_y, PLveto_z;
  G4double PSveto_x, PSveto_y, PSveto_z;
  G4double dist_first,dist;

  //SMRD
  G4double IronSMRDX, IronSMRDY, IronSMRDZ;
  G4double FIronSMRDX, FIronSMRDY, FIronSMRDZ;
  G4double SIronSMRDX, SIronSMRDY, SIronSMRDZ;
  G4double SciSMRDX, SciSMRDY, SciSMRDZ;
  G4double SMRD_Gap, Start_ironx_smrd, Start_sciz_smrd;

  //BabyMIND
  G4double IronBMX, IronBMY, IronBMZ;
  G4double SciBMHX, SciBMHY, SciBMHZ;
  G4double SciBMVX, SciBMVY, SciBMVZ;

  //Size of module box
  G4double PMboxX, PMboxY, PMboxZ;
  G4double WagaboxX, WagaboxY, WagaboxZ;
  G4double SMRDboxX, SMRDboxY, SMRDboxZ;
  G4double BMboxX, BMboxY, BMboxZ;
  G4double IngboxX, IngboxY, IngboxZ;

  //Position of module box
  G4double PMboxposX,PMboxposY,PMboxposZ;
  G4double WagaboxposX1,WagaboxposY1,WagaboxposZ1;
  G4double WagaboxposX2,WagaboxposY2,WagaboxposZ2;
  G4double SMRDboxposX,SMRDboxposY,SMRDboxposZ;
  G4double BMboxposX,BMboxposY,BMboxposZ;
  G4double IngboxposX,IngboxposY,IngboxposZ;

  //B2 hall box
  G4double B2hallX,B2hallY,B2hallZ;
  G4double B2hallposX,B2hallposY,B2hallposZ;

  B2VetoSD* avetoSD;
  B2HLayerSD* ahlayerSD;  
  B2VLayerSD* avlayerSD; 

  B2_Dimension *fdim;
 
  int flag;
  int mode;
  int waterin;
  bool magfld;

  B2DetectorConstruction(int, int, bool);
  ~B2DetectorConstruction();
    
  G4VPhysicalVolume* Construct();
 
};

#endif
