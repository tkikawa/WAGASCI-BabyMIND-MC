//#define VIS //G4Subtraction is not used for OpenGL visualization

#include <stdio.h>
#include <iostream>
#include <vector>

#include "B2DetectorConstruction.hh"
#include "B2HLayerSD.hh"
#include "B2VLayerSD.hh"
#include "B2VetoSD.hh"
#include "B2_Dimension.hh"

#include "B2MagneticField.hh"
#include "G4TransportationManager.hh"
#include "G4ChordFinder.hh"
#include "G4FieldManager.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4SDManager.hh"
#include "G4TwoVector.hh"
#include "G4ExtrudedSolid.hh"
#include "G4EllipticalTube.hh"
#include "G4SubtractionSolid.hh"
#include "G4VisAttributes.hh"

#include "globals.hh"

#include "Const.hh"

B2DetectorConstruction::B2DetectorConstruction(int MODE, int WATERIN, bool MAGFLD)
{
  mode = MODE;
  waterin = WATERIN;
  magfld = MAGFLD;
}

B2DetectorConstruction::~B2DetectorConstruction()
{
}


G4VPhysicalVolume* B2DetectorConstruction::Construct()
{
  //Initialise materials
  DefineMaterial();

  //Initialise volumes
  DefineSpace();

  //Initialise detector elements parameters
  DefineStructures();

  int start_mod,stop_mod;
  start_mod=0;stop_mod=6;//without INGRID module (default)

  //Visibility of box
  G4VisAttributes* BoxVisAtt = new G4VisAttributes(G4Color(1.,1.,1.));
  BoxVisAtt->SetVisibility(false);

  //Rotation definition
  G4RotationMatrix *xrot  = new G4RotationMatrix(G4ThreeVector(1,0,0),90.*degree);
  G4RotationMatrix *xrot2 = new G4RotationMatrix(G4ThreeVector(1,0,0),-90.*degree);
  G4RotationMatrix *yrot  = new G4RotationMatrix(G4ThreeVector(0,1,0),90.*degree);
  G4RotationMatrix *yrot2 = new G4RotationMatrix(G4ThreeVector(0,1,0),-90.*degree);
  //for Y grid layer
  G4RotationMatrix *rotgridY_v = new G4RotationMatrix(G4ThreeVector(0,0,1),180.*degree);
  rotgridY_v->rotateY(-90*degree);
  rotgridY_v->rotateX(-90*degree);
  G4RotationMatrix *rotgridY_h = new G4RotationMatrix(G4ThreeVector(1,0,0),-90.*degree);
  rotgridY_h->rotateY(-90*degree);
  //for Xgrid layer
  G4RotationMatrix *rotgridX_v = new G4RotationMatrix(G4ThreeVector(0,1,0),-90.*degree);
  rotgridX_v->rotateX(-90*degree);
  G4RotationMatrix *rotgridX_h = new G4RotationMatrix(G4ThreeVector(0,0,1),180.*degree);
  rotgridX_h->rotateX(-90*degree);
  rotgridX_h->rotateY(-90*degree);

  
  //World volume
  G4Box* experimentalHall_box = new G4Box("Hall",WorldX,WorldY,WorldZ);
  G4LogicalVolume* worldLV = new G4LogicalVolume(experimentalHall_box, Air, "hall_log",0,0,0);
  G4VPhysicalVolume* worldPV    = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),worldLV,"hall",0,false,0);

  //Hall Dirt volume
  G4Tubs* HallDirtSLD = new G4Tubs("halldirt", HallDirtRadiusMin, HallDirtRadiusMax, HallDirtHeight, HallDirtSPhi, HallDirtDPhi);
  G4LogicalVolume* HallDirtLV = new G4LogicalVolume(HallDirtSLD, Concrete, "HallDirt",0,0,0);
  new G4PVPlacement(xrot,G4ThreeVector(HallDirtX,HallDirtY,HallDirtZ),HallDirtLV,"HallDirt",worldLV,false,0);
  G4VisAttributes* HallDirtVisAtt = new G4VisAttributes(G4Color(1.,1.,1.));
  HallDirtVisAtt->SetForceWireframe(true);
  HallDirtLV->SetVisAttributes(HallDirtVisAtt);

  //B2 floor
  G4Tubs* HallDirtfloor = new G4Tubs("hallfloor", 0*cm, HallDirtRadiusMin-1*mm, (HallDirtHeight+Floorup)/2., HallDirtSPhi, HallDirtDPhi);
  G4LogicalVolume* HallDirtfloorLV = new G4LogicalVolume(HallDirtfloor, Concrete, "HallDirtfloor",0,0,0);
  new G4PVPlacement(xrot,G4ThreeVector(HallDirtX,HallDirtY - HallDirtHeight + (HallDirtHeight+Floorup)/2.,HallDirtZ),HallDirtfloorLV,"HallDirtfloor",worldLV,false,0);
  G4VisAttributes* HallDirtfloorVisAtt = new G4VisAttributes(G4Color(1.,1.,1.));
  HallDirtfloorVisAtt->SetForceWireframe(true);
  HallDirtfloorLV->SetVisAttributes(HallDirtfloorVisAtt);

  //B2 pillar
  G4Box* Pillar_box = new G4Box("Pillar",PillarX,PillarY,PillarZ);
  G4LogicalVolume* PillarLV = new G4LogicalVolume(Pillar_box, Concrete, "Pillar",0,0,0);
  new G4PVPlacement(0,G4ThreeVector(PillarposX,PillarposY,PillarposZ),PillarLV,"Pillar",worldLV,false,0);
  new G4PVPlacement(0,G4ThreeVector(PillarposX_rev,PillarposY,PillarposZ),PillarLV,"Pillar",worldLV,false,0);
  G4VisAttributes* PillarVisAtt = new G4VisAttributes(G4Color(1.,1.,1.));
  PillarVisAtt->SetForceWireframe(true);
  PillarLV->SetVisAttributes(PillarVisAtt);

  //B2 hall box
  G4Box* B2Hall_box= new G4Box("B2Hall",B2hallX,B2hallY,B2hallZ);
  G4LogicalVolume* B2LV = new G4LogicalVolume(B2Hall_box, Air, "B2_log",0,0,0);
  new G4PVPlacement(0,G4ThreeVector(B2hallposX,B2hallposY,B2hallposZ),B2LV,"B2",worldLV,false,0);
  B2LV->SetVisAttributes(BoxVisAtt);

  //////////////////////////////////////////////////////////
  /// We first create modules, than put materials inside ///
  //////////////////////////////////////////////////////////
  
  //********** Module **********//
  
  //Modules objects:
  G4Box* pm_box = new G4Box("Proton_Module",PMboxX, PMboxY, PMboxZ);
  G4Box* wagasci_box = new G4Box("WAGASCI_Module",WagaboxX, WagaboxY, WagaboxZ);
  G4Box* smrd_box = new G4Box("SMRD_Module",SMRDboxX, SMRDboxY, SMRDboxZ);
  G4Box* babymind_box = new G4Box("BabyMIND_Module",BMboxX, BMboxY, BMboxZ);
  G4Box* ingrid_box = new G4Box("INGRID_Module",IngboxX, IngboxY, IngboxZ);
  
  G4LogicalVolume* moduleLV[6];//
  
  for(int k=start_mod;k<stop_mod;k++){
    
    if(k==0){//Proton Module
      moduleLV[k] = new G4LogicalVolume(pm_box, Air, "module_lv_pm");
      new G4PVPlacement(0,G4ThreeVector(PMboxposX,PMboxposY,PMboxposZ),moduleLV[k],"module0",B2LV,false,k);
      moduleLV[k]->SetVisAttributes(BoxVisAtt);
    }
    else if(k==1){//1st WAGASCI
      moduleLV[k] = new G4LogicalVolume(wagasci_box, Air, "module_lv_wagasci1");
      new G4PVPlacement(0,G4ThreeVector(WagaboxposX1,WagaboxposY1,WagaboxposZ1),moduleLV[k],"module1",B2LV,false,k);
      moduleLV[k]->SetVisAttributes(BoxVisAtt);
    }
    else if(k==2){//2nd WAGASCI
      moduleLV[k] = new G4LogicalVolume(wagasci_box, Air, "module_lv_wagasci2");
      new G4PVPlacement(0,G4ThreeVector(WagaboxposX2,WagaboxposY2,WagaboxposZ2),moduleLV[k],"module2",B2LV,false,k);
      moduleLV[k]->SetVisAttributes(BoxVisAtt);
    }
    else if(k==3){//left side MRD
      moduleLV[k] = new G4LogicalVolume(smrd_box, Air, "module_lv_smrd_left");
      new G4PVPlacement(0,G4ThreeVector(-SMRDboxposX,SMRDboxposY,SMRDboxposZ),moduleLV[k],"module3",B2LV,false,k);
      moduleLV[k]->SetVisAttributes(BoxVisAtt);
    }
    else if(k==4){//right side MRD
      moduleLV[k] = new G4LogicalVolume(smrd_box, Air, "module_lv_smrd_right");
      new G4PVPlacement(0,G4ThreeVector(SMRDboxposX,SMRDboxposY,SMRDboxposZ),moduleLV[k],"module4",B2LV,false,k);
      moduleLV[k]->SetVisAttributes(BoxVisAtt);
    }
    else if(k==5){//Baby-MIND
      moduleLV[k] = new G4LogicalVolume(babymind_box, Air, "module_lv_babymind");
      new G4PVPlacement(0,G4ThreeVector(BMboxposX,BMboxposY,BMboxposZ),moduleLV[k],"module5",B2LV,false,k);
      moduleLV[k]->SetVisAttributes(BoxVisAtt);
    }
    else if(k==6){//INGRID (optional)
      moduleLV[k] = new G4LogicalVolume(ingrid_box, Air, "module_lv_ingrid");
      new G4PVPlacement(0,G4ThreeVector(IngboxposX,IngboxposY,IngboxposZ),moduleLV[k],"module6",B2LV,false,k);
      moduleLV[k]->SetVisAttributes(BoxVisAtt);
    }
  }

  //********** IRON and WATER BLOCK **********//

  //Visualization setting
  G4VisAttributes* ironVisAtt = new G4VisAttributes(G4Color(0.7,0.,0.7)); // magenta
  //ironVisAtt->SetForceWireframe(true);
  ironVisAtt->SetForceSolid(true);

  //Iron plate for INGRID module   
  G4Box* iron_block = new G4Box("Iron",iron_xy,iron_xy,iron_z);
  G4LogicalVolume* ironLV = new G4LogicalVolume(iron_block,Fe,"ironLV");  
  ironLV->SetVisAttributes(ironVisAtt);

  //Central iron plate for SMRD
  G4Box* iron_smrd = new G4Box("IronSMRD",IronSMRDX,IronSMRDY,IronSMRDZ);
  G4LogicalVolume* iron_smrdLV = new G4LogicalVolume(iron_smrd,Fe,"iron_smrdLV");
  iron_smrdLV->SetVisAttributes(ironVisAtt);

  //Front and rear iron plate for SMRD
  G4Box* firon_smrd = new G4Box("FIronSMRD",FIronSMRDX,FIronSMRDY,FIronSMRDZ);
  G4LogicalVolume* firon_smrdLV = new G4LogicalVolume(firon_smrd,Fe,"firon_smrdLV");
  firon_smrdLV->SetVisAttributes(ironVisAtt);

  //Side iron plate for SMRD
  G4Box* siron_smrd = new G4Box("SIronSMRD",SIronSMRDX,SIronSMRDY,SIronSMRDZ);
  G4LogicalVolume* siron_smrdLV = new G4LogicalVolume(siron_smrd,Fe,"siron_smrdLV");
  siron_smrdLV->SetVisAttributes(ironVisAtt);

  //Magnet module (iron plate) for Baby-MIND
  G4Box* iron_bm = new G4Box("iron_bm", IronBMX, IronBMY, IronBMZ);
  G4LogicalVolume* iron_bm_LV = new G4LogicalVolume(iron_bm, Fe, "iron_bmLV");
  iron_bm_LV->SetVisAttributes(ironVisAtt);

  //Water tank (SUS304) for WAGASCI
  G4Box* SUS_box[2];
  SUS_box[0] = new G4Box("SUS_box0",(62.8+1.2)*cm,(62.8+1.6)*cm,(25.+0.4)*cm);
  SUS_box[1] = new G4Box("SUS_box1",(62.8+1.2)*cm,(62.8+1.6)*cm,(25.+0.4)*cm);
  G4LogicalVolume *SUSLV[2];
  SUSLV[0] = new G4LogicalVolume(SUS_box[0],SUS304,"SUSLV0");
  SUSLV[1] = new G4LogicalVolume(SUS_box[1],SUS304,"SUSLV1");
  G4VisAttributes* SUSVisAtt = new G4VisAttributes(G4Colour(1.,0.,0.));
  SUSLV[0]->SetVisAttributes(SUSVisAtt);
  SUSLV[1]->SetVisAttributes(SUSVisAtt);

  //Water for WAGASCI
  G4Box* Water_box[2];
  Water_box[0] = new G4Box("Water_box0",62.8*cm,62.8*cm,25.*cm);
  Water_box[1] = new G4Box("Water_box1",62.8*cm,62.8*cm,25.*cm);
  G4VisAttributes* WaterVisAtt = new G4VisAttributes(G4Colour(0.,0.,1.));
  WaterVisAtt->SetForceSolid(true);
  G4VisAttributes* AirVisAtt = new G4VisAttributes(G4Colour(1.,0.,0.));
  AirVisAtt->SetForceWireframe(true);
  G4LogicalVolume *WaterLV[2];

  if(waterin%2==0){
    WaterLV[0] = new G4LogicalVolume(Water_box[0], Air  ,"WaterLV0");
    WaterLV[0]->SetVisAttributes(AirVisAtt);
  }
  else{
    WaterLV[0] = new G4LogicalVolume(Water_box[0], Water,"WaterLV0");
    WaterLV[0]->SetVisAttributes(WaterVisAtt);
  }
  if(waterin/2==0){
    WaterLV[1] = new G4LogicalVolume(Water_box[1], Air  ,"WaterLV1");
    WaterLV[1]->SetVisAttributes(AirVisAtt);
  }
  else{
    WaterLV[1] = new G4LogicalVolume(Water_box[1], Water,"WaterLV1");
    WaterLV[1]->SetVisAttributes(WaterVisAtt);
  }



  //********** Scintillator for tracking plane**********//

  //Visualization setting
  G4VisAttributes* sciVisAtt = new G4VisAttributes(G4Color(0.,1.,0.)); // green
  sciVisAtt->SetForceWireframe(true);
  //sciVisAtt->SetForceSolid(true);

  //INGRID scintilaltor
  std::vector<G4TwoVector> vdim;  // vertical-layer scintillator (internal)
  std::vector<G4TwoVector> hdim;  // horizontal-layer scintillator (internal)

  //SciBar type for Proton module
  std::vector<G4TwoVector> svdim;  // vertical-layer scintillator (internal)
  std::vector<G4TwoVector> shdim;  // horizontal-layer scintillator (internal)

  //INGRID type for Proton module
  std::vector<G4TwoVector> ivdim;  // vertical-layer scintillator (internal)
  std::vector<G4TwoVector> ihdim;  // horizontal-layer scintillator (internal)

  //Scintillator dimension for WAGASCI
  std::vector<G4TwoVector> wvdim;  // vertical-layer scintillator (internal)
  std::vector<G4TwoVector> whdim;  // horizontal-layer scintillator (internal)


  //for INGRID
  for(int iver=0; iver<nSciVertex; iver++){
    vdim.push_back( G4TwoVector( SciVertex_x[iver], SciVertex_y[iver] ) );
    hdim.push_back( G4TwoVector( SciVertex_y[nSciVertex-1-iver], 
    SciVertex_x[nSciVertex-1-iver] ) );
  }
 
  //for SciBar type of PM
  for(int iver=0; iver<nSciVertex; iver++){
    svdim.push_back( G4TwoVector( SciVertex2_x[iver], SciVertex2_y[iver] ) );
    shdim.push_back( G4TwoVector( SciVertex2_y[nSciVertex-1-iver], 
    SciVertex2_x[nSciVertex-1-iver] ) );
  }
 
  //for INGRID type of PM
  for(int iver=0; iver<nSciVertex; iver++){
    ivdim.push_back( G4TwoVector( SciVertex3_x[iver], SciVertex3_y[iver] ) );
    ihdim.push_back( G4TwoVector( SciVertex3_y[nSciVertex-1-iver], 
    SciVertex3_x[nSciVertex-1-iver] ) );
  }

  //for WAGASCI
  for(int iver=0; iver<nSciVertex; iver++){
    wvdim.push_back( G4TwoVector( SciVertex4_x[iver], SciVertex4_y[iver] ) );
    whdim.push_back( G4TwoVector( SciVertex4_y[nSciVertex-1-iver], SciVertex4_x[nSciVertex-1-iver] ) );
  }

  std::vector<G4ExtrudedSolid::ZSection> zsec;
  zsec.push_back( G4ExtrudedSolid::ZSection(-600*mm, G4TwoVector(0*mm,0*mm), 1) );
  zsec.push_back( G4ExtrudedSolid::ZSection(600*mm, G4TwoVector(0*mm,0*mm), 1) );

  std::vector<G4ExtrudedSolid::ZSection> wzsec;
  wzsec.push_back( G4ExtrudedSolid::ZSection(-500*mm, G4TwoVector(0*mm,0*mm), 1) );
  wzsec.push_back( G4ExtrudedSolid::ZSection( 500*mm, G4TwoVector(0*mm,0*mm), 1) );

  
  //INGRID vertical scintillator
  G4ExtrudedSolid* vscint_tmp = new G4ExtrudedSolid("vscint_tmp", vdim, zsec);
  G4EllipticalTube* vsci_hole = new G4EllipticalTube("vsci_hole",1.3*mm,1.95*mm,600*mm);
#ifdef VIS
  G4ExtrudedSolid* vscint_int = new G4ExtrudedSolid("vscint_tmp", vdim, zsec);
#else
  G4SubtractionSolid* vscint_int = new G4SubtractionSolid("vscint_int",  vscint_tmp, vsci_hole);
#endif
  G4LogicalVolume* vscint_intLV = new G4LogicalVolume(vscint_int,Scinti,"vscint_intLV");
  vscint_intLV->SetVisAttributes(sciVisAtt);
  
  //INGRID horizontal scintillator
  G4ExtrudedSolid* hscint_tmp = new G4ExtrudedSolid("hscint_tmp", hdim, zsec);
  G4EllipticalTube* hsci_hole = new G4EllipticalTube("hsci_hole",1.95*mm,1.3*mm,600*mm);
#ifdef VIS
  G4ExtrudedSolid* hscint_int = new G4ExtrudedSolid("hscint_tmp", hdim, zsec);
#else
  G4SubtractionSolid* hscint_int = new G4SubtractionSolid("hscint_int",  hscint_tmp, hsci_hole);
#endif
  G4LogicalVolume* hscint_intLV = new G4LogicalVolume(hscint_int,Scinti,"hscint_intLV");
  hscint_intLV->SetVisAttributes(sciVisAtt);

  //SciBar vertical scintillator
  G4ExtrudedSolid* vscint2_tmp = new G4ExtrudedSolid("vscint2_tmp", svdim, zsec);
  G4EllipticalTube* vsci_hole2 = new G4EllipticalTube("vsci_hole2",0.9*mm,0.9*mm,600*mm);
#ifdef VIS
  G4ExtrudedSolid* vscint2_int = new G4ExtrudedSolid("vscint2_tmp", svdim, zsec);
#else
  G4SubtractionSolid* vscint2_int = new G4SubtractionSolid("vscint2_int",  vscint2_tmp, vsci_hole2);
#endif
  G4LogicalVolume* vscint2_intLV = new G4LogicalVolume(vscint2_int,Scinti,"vscint2_intLV");
  vscint2_intLV->SetVisAttributes(sciVisAtt);

  //SciBar horizontal scintillator
  G4ExtrudedSolid* hscint2_tmp = new G4ExtrudedSolid("hscint2_tmp", shdim, zsec);
  G4EllipticalTube* hsci_hole2 = new G4EllipticalTube("hsci_hole2",0.9*mm,0.9*mm,600*mm);
#ifdef VIS
  G4ExtrudedSolid* hscint2_int = new G4ExtrudedSolid("hscint2_tmp", shdim, zsec);
#else
  G4SubtractionSolid* hscint2_int = new G4SubtractionSolid("hscint2_int",  hscint2_tmp, hsci_hole2);
#endif
  G4LogicalVolume* hscint2_intLV = new G4LogicalVolume(hscint2_int,Scinti,"hscint2_intLV");
  hscint2_intLV->SetVisAttributes(sciVisAtt);

  //INGRID vertical scintillator for PM
  G4ExtrudedSolid* vscint3_tmp = new G4ExtrudedSolid("vscint3_tmp", ivdim, zsec);
  G4EllipticalTube* vsci_hole3 = new G4EllipticalTube("vsci_hole3",1.3*mm,1.95*mm,600*mm);
#ifdef VIS
  G4ExtrudedSolid* vscint3_int = new G4ExtrudedSolid("vscint3_tmp", ivdim, zsec);
#else
  G4SubtractionSolid* vscint3_int = new G4SubtractionSolid("vscint3_int",  vscint3_tmp, vsci_hole3);
#endif
  G4LogicalVolume* vscint3_intLV = new G4LogicalVolume(vscint3_int,Scinti,"vscint3_intLV");
  vscint3_intLV->SetVisAttributes(sciVisAtt);

  //INGRID horizontal scintillator for PM
  G4ExtrudedSolid* hscint3_tmp = new G4ExtrudedSolid("hscint3_tmp", ihdim, zsec);
  G4EllipticalTube* hsci_hole3 = new G4EllipticalTube("hsci_hole3",1.95*mm,1.3*mm,600*mm);
#ifdef VIS
  G4ExtrudedSolid* hscint3_int = new G4ExtrudedSolid("hscint3_tmp", ihdim, zsec);
#else
  G4SubtractionSolid* hscint3_int = new G4SubtractionSolid("hscint3_int",  hscint3_tmp, hsci_hole3);
#endif
  G4LogicalVolume* hscint3_intLV = new G4LogicalVolume(hscint3_int,Scinti,"hscint3_intLV");
  hscint3_intLV->SetVisAttributes(sciVisAtt);

  //SMRD scintillator
  G4Box* smsci_int = new G4Box("smsci_int",SciSMRDX,SciSMRDY,SciSMRDZ);
  G4LogicalVolume* smsci_intLV = new G4LogicalVolume(smsci_int,Scinti,"smrd_vlayerLV");
  smsci_intLV->SetVisAttributes(sciVisAtt);

  //Baby-MIND horizontal scintillator
  G4Box* bmhsci_int = new G4Box("bmhsci_int",SciBMHX,SciBMHY,SciBMHZ);
  G4LogicalVolume* bmhsci_intLV = new G4LogicalVolume(bmhsci_int,Scinti,"bm_hlayerLV");
  bmhsci_intLV->SetVisAttributes(sciVisAtt);

  //Baby-MIND vertical scintillator
  G4Box* bmvsci_int = new G4Box("bmvsci_int",SciBMVX,SciBMVY,SciBMVZ);
  G4LogicalVolume* bmvsci_intLV = new G4LogicalVolume(bmvsci_int,Scinti,"bm_vlayerLV");
  bmvsci_intLV->SetVisAttributes(sciVisAtt);

  //WAGASCI vertical scintillator
  G4ExtrudedSolid* vwscint_tmp = new G4ExtrudedSolid("vwscint_tmp", wvdim, wzsec);
  G4Box* vglue = new G4Box("vglue",0.61*mm,0.61*mm,500.1*mm); //add 0.1mm to avoid bug
#ifdef VIS
  G4ExtrudedSolid* vwscint_int = new G4ExtrudedSolid("vwscint_tmp", wvdim, wzsec);
#else
  G4SubtractionSolid* vwscint_int = new G4SubtractionSolid("vwscint_int", vwscint_tmp, vglue, 0, G4ThreeVector(-3.9*mm,-0.9*mm,0*mm));
#endif
  G4LogicalVolume* vwscint_intLV = new G4LogicalVolume(vwscint_int,Scinti,"vwscint_intLV");
  vwscint_intLV->SetVisAttributes(sciVisAtt);

  //WAGASCI horizontal scintillator
  G4ExtrudedSolid* hwscint_tmp = new G4ExtrudedSolid("hwscint_tmp", whdim, wzsec);
  G4Box* hglue = new G4Box("hglue",0.61*mm,0.61*mm,500.1*mm); //add 0.1mm to avoid bug
#ifdef VIS
  G4ExtrudedSolid* hwscint_int = new G4ExtrudedSolid("hwscint_tmp", whdim, wzsec);
#else
  G4SubtractionSolid* hwscint_int = new G4SubtractionSolid("hwscint_int", hwscint_tmp, hglue, 0, G4ThreeVector(0.9*mm,-3.9*mm,0*mm));
#endif
  G4LogicalVolume* hwscint_intLV = new G4LogicalVolume(hwscint_int,Scinti,"hwscint_intLV");
  hwscint_intLV->SetVisAttributes(sciVisAtt);


  //WAGASCI vertical grid scintillator
#ifdef VIS
  G4VSolid* vwgridscint_int = new G4ExtrudedSolid("vwgridscint_tmp", wvdim, wzsec);
#else
  G4VSolid* vwgridscint_tmp = new G4ExtrudedSolid("vwgridscint_tmp", wvdim, wzsec);
  G4VSolid* vwgridscint_int = new G4SubtractionSolid("vwgridscint_int", vwgridscint_tmp, vglue, 0, G4ThreeVector(-3.9*mm,-0.9*mm,0*mm));
  G4VSolid* vcut = new G4Box("vcut",13.1/2.*mm,3.1/2.*mm,3.5/2.*mm); //add 0.1mm to avoid bug
  for(int i=0;i<20;i++){
    G4VSolid* vwgridscint_tmp2 = new G4SubtractionSolid("", vwgridscint_int, vcut, 0, G4ThreeVector(5.5*mm,0*mm,(wgs_offsetxy_grid + wgs_scinti_thick/2. + i*wgs_cutgap)*cm));
    vwgridscint_int = vwgridscint_tmp2;
  }

#endif
  G4LogicalVolume* vwgridscint_intLV     = new G4LogicalVolume(vwgridscint_int,Scinti,"vwgridscint_intLV");
  vwgridscint_intLV->SetVisAttributes(sciVisAtt);

  //WAGASCI horizontal grid scintillator
#ifdef VIS
  G4VSolid* hwgridscint_int = new G4ExtrudedSolid("hwgridscint_tmp", whdim, wzsec);
#else
  G4VSolid* hwgridscint_tmp = new G4ExtrudedSolid("hwgridscint_tmp", whdim, wzsec);
  G4VSolid* hwgridscint_int = new G4SubtractionSolid("hwgridscint_int", hwgridscint_tmp, hglue, 0, G4ThreeVector(0.9*mm,-3.9*mm,0*mm));
  G4VSolid* hcut = new G4Box("hcut",3.1/2.*mm,13./2.*mm,3.5/2.*mm); //add 0.1mm to avoid bug
  for(int i=0;i<20;i++){
    G4VSolid* hwgridscint_tmp2 = new G4SubtractionSolid("", hwgridscint_int, hcut, 0, G4ThreeVector(0*mm,5.5*mm,(wgs_offsetxy_grid + wgs_scinti_thick/2. + i*wgs_cutgap)*cm));
    hwgridscint_int = hwgridscint_tmp2;
  }
#endif
  G4LogicalVolume* hwgridscint_intLV = new G4LogicalVolume(hwgridscint_int,Scinti,"hwgridscint_intLV");
  hwgridscint_intLV->SetVisAttributes(sciVisAtt);


  //********** Scintillator for veto plane**********//

  //Visualization setting
  G4VisAttributes* vetoVisAtt = new G4VisAttributes(G4Color(0.,0.6,0.5));
  vetoVisAtt->SetForceWireframe(true);

  //INGRID Long Veto plane
  G4Box* Lveto_box = new G4Box("Lveto_box",Lveto_x,Lveto_y,Lveto_z);
  G4LogicalVolume *LvetoLV = new G4LogicalVolume(Lveto_box,Scinti,"LvetoLV");
  LvetoLV->SetVisAttributes(vetoVisAtt);
  
  //INGRID short Veto plane
  G4Box* Sveto_box = new G4Box("Sveto_box",Sveto_x,Sveto_y,Sveto_z);
  G4LogicalVolume *SvetoLV = new G4LogicalVolume(Sveto_box,Scinti,"SvetoLV");
  SvetoLV->SetVisAttributes(vetoVisAtt);

  //Proton module Long Veto plane
  G4Box* PLveto_box = new G4Box("PLveto_box",PLveto_x,PLveto_y,PLveto_z);
  G4LogicalVolume *PLvetoLV = new G4LogicalVolume(PLveto_box,Scinti,"PLvetoLV");
  PLvetoLV->SetVisAttributes(vetoVisAtt);
  
  //Proton module Short Veto plane
  G4Box* PSveto_box = new G4Box("PSveto_box",PSveto_x,PSveto_y,PSveto_z);
  G4LogicalVolume *PSvetoLV = new G4LogicalVolume(PSveto_box,Scinti,"PSvetoLV");
  PSvetoLV->SetVisAttributes(vetoVisAtt);

  //Rotation matrix for veto planes
  G4RotationMatrix *TB2LR = new G4RotationMatrix(G4ThreeVector(0,0,1.),90.*degree);


  //*********POSITIONNING OF ALL THE ELEMENTS *********//
  for(int k=start_mod;k<stop_mod;k++){

    //Proton Module
    if(k==0){
      //18 tracking planes
      for(int i=0;i<18;i++){
	G4double z;
	if (i==0){
	  z = Pscibar_start + scibar_z;
	  G4cout << i << " "<<z<<G4endl;
	}
	else{
	  z = Pscibar_start + scibar_z + dist_first + dist + 2*dist*(i-1);
	  G4cout << i << " "<<z<<G4endl;
	}

	//First plane similar to an INGRID module, will be a veto plane
	if (i==0){
	  for(int j=0;j<24;j++){
	    sprintf(name,"x%d",j+k*264);
	    G4double x_scinti =  scibar_xy_start + 2*j*scibar_y;
	    new G4PVPlacement(xrot,G4ThreeVector(x_scinti,0.,z+dist),vscint3_intLV,name,moduleLV[k],false,j+k*PLNMAX*CHMAX); // in Module
	    sprintf(name,"y%d",j+k*264);
	    G4double y_scinti =   scibar_xy_start + 2*j*scibar_y;
	    new G4PVPlacement(yrot,G4ThreeVector(0.,y_scinti,z),hscint3_intLV,name,moduleLV[k],false,j+k*PLNMAX*CHMAX); // in Module
	  }
	}

	//Other 17 tracking planes
	else{
	  // 8 INGRID type scintillators
	  for(int j=0;j<8;j++){
	    sprintf(name,"vlayer[%d][%d][%d]",k,i,j);
	    G4double x_scinti = scibar_xy_start + 2*j*scibar_y;
	    new G4PVPlacement(xrot,G4ThreeVector(x_scinti,0.,z+dist),vscint3_intLV,name,moduleLV[k],false,j+i*CHMAX+k*PLNMAX*CHMAX); // vertical bars
	    sprintf(name,"hlayer[%d][%d][%d]",k,i,j);
	    G4double y_scinti = scibar_xy_start + 2*j*scibar_y;
	    new G4PVPlacement(yrot,G4ThreeVector(0,y_scinti,z),hscint3_intLV,name,moduleLV[k],false,j+i*CHMAX+k*PLNMAX*CHMAX); // horizontal bars
	  }

	  //16 SciBar type scintillators
	  for(int j=8;j<24;j++){
	    sprintf(name,"vlayer[%d][%d][%d]",k,i,j);
	    G4double x_scinti = scibar_xy_start + 15*scibar_y +(2*(j-8)+1)*scibar2_y;
	    new G4PVPlacement(xrot,G4ThreeVector(x_scinti,0.,z+dist),vscint2_intLV,name,moduleLV[k],false,j+i*CHMAX+k*PLNMAX*CHMAX); // vertical bars
	    sprintf(name,"hlayer[%d][%d][%d]",k,i,j);
	    G4double y_scinti = scibar_xy_start + 15*scibar_y +(2*(j-8)+1)*scibar2_y;
	    new G4PVPlacement(yrot,G4ThreeVector(0,y_scinti,z),hscint2_intLV,name,moduleLV[k],false,j+i*CHMAX+k*PLNMAX*CHMAX); // horizontal bars
	  }

	  // 8 INGRID type scintillators
	  for(int j=24;j<32;j++){
	    sprintf(name,"vlayer[%d][%d][%d]",k,i,j);
	    G4double x_scinti = scibar_xy_start + 15*scibar_y +2*16*scibar2_y+(2*(j-24)+1)*scibar_y;
	    new G4PVPlacement(xrot,G4ThreeVector(x_scinti,0.,z+dist),vscint3_intLV,name,moduleLV[k],false,j+i*CHMAX+k*PLNMAX*CHMAX); // vertical bars
	    sprintf(name,"hlayer[%d][%d][%d]",k,i,j);
	    G4double y_scinti = scibar_xy_start + 15*scibar_y +2*16*scibar2_y+(2*(j-24)+1)*scibar_y;
	    new G4PVPlacement(yrot,G4ThreeVector(0,y_scinti,z),hscint3_intLV,name,moduleLV[k],false,j+i*CHMAX+k*PLNMAX*CHMAX); // horizontal bars
	  }
	}
      }//end of tracking planes

      //Veto Planes
      for(int i=0;i<17;i++){
        G4double z = Pveto_start + Sveto_z + 2*i*Sveto_z;
        sprintf(vetoname[0],"veto[%d][0][%d]",k,i);
        sprintf(vetoname[1],"veto[%d][1][%d]",k,i);
        sprintf(vetoname[2],"veto[%d][2][%d]",k,i);
        sprintf(vetoname[3],"veto[%d][3][%d]",k,i);

        new G4PVPlacement(0,G4ThreeVector(-0.5*cm,65.5*cm,z),PLvetoLV,vetoname[3],moduleLV[k],false, i+2*CHMAX+k*PLNMAX*CHMAX);  // UP
        new G4PVPlacement(0,G4ThreeVector(1.5*cm,-65.5*cm,z),PSvetoLV,vetoname[2],moduleLV[k],false, i+0*CHMAX+k*PLNMAX*CHMAX); // DOWN
        new G4PVPlacement(TB2LR,G4ThreeVector(65.5*cm,-2.5*cm,z),PLvetoLV,vetoname[1],moduleLV[k],false, i+1*CHMAX+k*PLNMAX*CHMAX); // LEFT
        new G4PVPlacement(TB2LR,G4ThreeVector(-65.5*cm,0.5*cm,z),PLvetoLV,vetoname[0],moduleLV[k],false, i+3*CHMAX+k*PLNMAX*CHMAX); // RIGTH
      }//end of veto planes
    }//end of Proton Module

    //WAGASCI
    else if(k==1||k==2){
      sprintf(name,"sustarget[%d]",k);
      new G4PVPlacement(0,G4ThreeVector(0,0,0),SUSLV[k-1],name,moduleLV[k],false,k); //water tank
      sprintf(name,"watertarget[%d]",k);
      new G4PVPlacement(0,G4ThreeVector(0,0,0),WaterLV[k-1],name,SUSLV[k-1],false,k); //water target
      for(int i=0;i<8;i++){
	for(int j=0;j<40;j++){
	  G4double x,y,z;

	  //vertical layer
	  fdim -> get_pos_wgs( k, 1, i, j, 0, &x, &y, &z); //mod view pln ch grid
	  sprintf(name,"vlayer[%d][%d][%d]",k,i,j);
	  new G4PVPlacement(xrot2,G4ThreeVector(x*cm,y*cm,z*cm),vwscint_intLV,name,WaterLV[k-1],false,j+i*CHMAX+k*PLNMAX*CHMAX);

	  //horizontal layer
	  fdim -> get_pos_wgs( k, 0, i, j, 0, &x, &y, &z); //mod view pln ch grid
	  sprintf(name,"hlayer[%d][%d][%d]",k,i,j);
	  new G4PVPlacement(yrot2,G4ThreeVector(x*cm,y*cm,z*cm),hwscint_intLV,name,WaterLV[k-1],false,j+i*CHMAX+k*PLNMAX*CHMAX);

	  if(j<20){
	    //vertical grid layer
	    fdim -> get_pos_wgs( k, 1, i, j, 1, &x, &y, &z); //mod view pln ch grid
	    sprintf(name,"vlayer1[%d][%d][%d]",k,i,j);
	    new G4PVPlacement(rotgridX_v,G4ThreeVector(x*cm,y*cm,z*cm),vwgridscint_intLV,name,WaterLV[k-1],false,40+j+i*CHMAX+k*PLNMAX*CHMAX);
	    fdim -> get_pos_wgs( k, 1, i, j, 2, &x, &y, &z); //mod view pln ch grid
	    sprintf(name,"vlayer2[%d][%d][%d]",k,i,j);
	    new G4PVPlacement(rotgridY_v,G4ThreeVector(x*cm,y*cm,z*cm),vwgridscint_intLV,name,WaterLV[k-1],false,60+j+i*CHMAX+k*PLNMAX*CHMAX);

	    //horizontal grid layer
	    fdim -> get_pos_wgs( k, 0, i, j, 1, &x, &y, &z); //mod view pln ch grid
	    sprintf(name,"hlayer1[%d][%d][%d]",k,i,j);
	    new G4PVPlacement(rotgridX_h,G4ThreeVector(x*cm,y*cm,z*cm),hwgridscint_intLV,name,WaterLV[k-1],false,40+j+i*CHMAX+k*PLNMAX*CHMAX);
	    fdim -> get_pos_wgs( k, 0, i, j, 2, &x, &y, &z); //mod view pln ch grid
	    sprintf(name,"hlayer2[%d][%d][%d]",k,i,j);
	    new G4PVPlacement(rotgridY_h,G4ThreeVector(x*cm,y*cm,z*cm),hwgridscint_intLV,name,WaterLV[k-1],false,60+j+i*CHMAX+k*PLNMAX*CHMAX);
	  }
	}
      }
    }//end of WAGASCI

    //SMRD
    else if(k==3 || k==4){
      G4double dir;
      if(k==3) dir=-1;
      else     dir=1;

      // 11 iron-blocs per module -----------------------------------------------------
      for(int i=0;i<11;i++){
        sprintf(ironname,"iron[%d][%d]",k,i);
        G4double x = (-Start_ironx_smrd + IronSMRDX + SMRD_Gap*i) * dir;
	if(i==0||i==10){
	  new G4PVPlacement(0,G4ThreeVector(x,0,0),firon_smrdLV,ironname,moduleLV[k],false,i+k*PLNMAX);
	}
	else{
	  new G4PVPlacement(0,G4ThreeVector(x,0,0),iron_smrdLV,ironname,moduleLV[k],false,i+k*PLNMAX);
	}
      }

      // 2 side iron-blocs per module -----------------------------------------------------
      sprintf(ironname,"iron[%d][%d]",k,11+0);
      new G4PVPlacement(0,G4ThreeVector(0,-SIronSMRDY+IronSMRDY,-IronSMRDZ-SIronSMRDZ),siron_smrdLV,ironname,moduleLV[k],false,11+0+k*PLNMAX);
      sprintf(ironname,"iron[%d][%d]",k,11+1);
      new G4PVPlacement(0,G4ThreeVector(0,-SIronSMRDY+IronSMRDY,IronSMRDZ+SIronSMRDZ),siron_smrdLV,ironname,moduleLV[k],false,11+1+k*PLNMAX);

      // 10 planes of scintillator per module-----------------------------------------
      for(int i=0;i<10;i++){
        for(int j=0;j<8;j++){
	  G4double x,z;
	  fdim -> get_pos_smrd( k, i, j, &x, &z); //mod pln ch
	  x = x*cm + (-Start_ironx_smrd + IronSMRDX*2 + SciSMRDX + 3*mm) * dir;//need revision
	  z = z*cm - Start_sciz_smrd + SciSMRDZ;
	  sprintf(name,"vlayer[%d][%d][%d]",k,i,j);
	  new G4PVPlacement(0,G4ThreeVector(x,0,z),smsci_intLV,name,moduleLV[k],false,j+i*CHMAX+k*PLNMAX*CHMAX);
	}
      }
    }//end of SMRD

    //Baby-MIND
    else if(k==5){
      
      G4double iron_module_z[33] = {7, 14.2, 21,49.7, 60.7, 71.7,
				    95.9, 102.9, 113.7, 121, 145.2, 151.9,
				    163.1, 170, 193.8, 200.8, 207.8, 214.8,
				    243.7, 250.6, 257.8, 286.5, 293.7, 300.4,
				    307.4, 331.3, 338.3, 345.4, 352.1, 376,
				    383.3, 390.2, 397.1};

      G4double babymind_install = -2.0*m;

      //Magnetic module (iron plate)
      for(int i = 0; i<33; i++){
        sprintf(ironname,"iron[%d][%d]",k,i);
	new G4PVPlacement(0, G4ThreeVector(0,0,babymind_install+iron_module_z[i]*cm),
			  iron_bm_LV,ironname,moduleLV[k],false,i+k*PLNMAX);
      }

      //Detector module (scintillator tracking planes)
      for(int i = 0; i<18; i++){

	//Horizontal scintillator
        for(int j=0;j<95;j++){
	  G4double x,y,z;
	  fdim -> get_pos_bm(k, 0, i, j, &x, &y, &z); //mod view pln ch
	  x = x*cm;
	  y = y*cm - SciBMVY+SciBMHY;
	  z = z*cm + babymind_install;
	  sprintf(name,"hlayer[%d][%d][%d]",k,i,j);
	  new G4PVPlacement(0,G4ThreeVector(x,y,z),bmhsci_intLV,name,moduleLV[k],false,j+i*CHMAX+k*PLNMAX*CHMAX);
	}

	//Vertical scintillator
        for(int j=0;j<16;j++){
	  G4double x,y,z;
	  fdim -> get_pos_bm(k, 1, i, j, &x, &y, &z); //mod view pln ch
	  x = x*cm-SciBMHX+SciBMVX;
	  y = y*cm;
	  z = z*cm + babymind_install;
	  sprintf(name,"vlayer[%d][%d][%d]",k,i,j);
	  new G4PVPlacement(0,G4ThreeVector(x,y,z),bmvsci_intLV,name,moduleLV[k],false,j+i*CHMAX+k*PLNMAX*CHMAX);
	}

      }

    }//end of Baby-MIND

    //INGRID Module (optional)
    else if(k==6){
      // 9 iron-blocs per module
      for(int i=0;i<9;i++){
        sprintf(ironname,"iron[%d][%d]",k,i);
        G4double z = iron_start + Gap*i;
        new G4PVPlacement(0,G4ThreeVector(0.,0.,z),ironLV,ironname,moduleLV[k],false,i+k*PLNMAX); // the world in Module     
      }
      // 11 planes of scintillator per module
      for(int i=0;i<11;i++){
	G4double z = scibar_start + scibar_z + Gap*i;
	for(int j=0;j<24;j++){
	  sprintf(name,"vlayer[%d][%d][%d]",k,i,j);
	  G4double x_scinti =  scibar_xy_start + 2*j*scibar_y;
	  new G4PVPlacement(xrot,G4ThreeVector(x_scinti,0.,z),vscint_intLV,name,moduleLV[k],false,j+i*CHMAX+k*PLNMAX*CHMAX);
	  sprintf(name,"hlayer[%d][%d][%d]",k,i,j);
	  G4double y_scinti =  scibar_xy_start + 2*j*scibar_y;
	  new G4PVPlacement(yrot,G4ThreeVector(0.,y_scinti,z+2*scibar_z+0.1*mm),hscint_intLV,name,moduleLV[k],false,j+i*CHMAX+k*PLNMAX*CHMAX);
	}
      }
      // 22 veto-bars per veto-plane (4 veto plances)
      for(int i=0;i<22;i++){
	G4double z = veto_start + 2*i*Sveto_z;
	sprintf(vetoname[0],"veto[%d][0][%d]",k,i);
	sprintf(vetoname[1],"veto[%d][1][%d]",k,i);
	sprintf(vetoname[2],"veto[%d][2][%d]",k,i);
	sprintf(vetoname[3],"veto[%d][3][%d]",k,i);
	new G4PVPlacement(0,G4ThreeVector(5.9*cm,68.4*cm,z),LvetoLV,vetoname[3],moduleLV[k],false, i+3*CHMAX+k*PLNMAX*CHMAX);  // UP
	new G4PVPlacement(0,G4ThreeVector(-0.9*cm,-65.9*cm,z),SvetoLV,vetoname[2],moduleLV[k],false, i+2*CHMAX+k*PLNMAX*CHMAX); // DOWN
	new G4PVPlacement(TB2LR,G4ThreeVector(70.9*cm,0.3*cm,z),LvetoLV,vetoname[1],moduleLV[k],false, i+1*CHMAX+k*PLNMAX*CHMAX); // LEFT
	new G4PVPlacement(TB2LR,G4ThreeVector(-70.575*cm,-3.7*cm,z),LvetoLV,vetoname[0],moduleLV[k],false, i+0*CHMAX+k*PLNMAX*CHMAX); // RIGTH
      }
    }//end of INGRID

  }//enf of module loop


  //*******Magnetic field*******/
  if(magfld){
    G4cout<<"Magnetic field on"<<G4endl;
    B2MagneticField* myfield = new B2MagneticField;
    G4FieldManager* fieldMgr = G4TransportationManager::GetTransportationManager()->GetFieldManager();
    fieldMgr->SetDetectorField(myfield);
    fieldMgr->CreateChordFinder(myfield);
  }

  //*******Assignment of sensitive detectors*******/
  G4SDManager* SDman = G4SDManager::GetSDMpointer();

  G4String hlayerSDname = "B2/hlayerSD";
  ahlayerSD = new B2HLayerSD( hlayerSDname, mode );
  SDman->AddNewDetector( ahlayerSD );
  hscint_intLV->SetSensitiveDetector( ahlayerSD );//INGRID module
  hscint2_intLV->SetSensitiveDetector( ahlayerSD );//SciBar type for Proton Module
  hscint3_intLV->SetSensitiveDetector( ahlayerSD );//INGRID type for Proton Module
  hwscint_intLV->SetSensitiveDetector( ahlayerSD );//Plane scintillator for WAGASCI
  hwgridscint_intLV->SetSensitiveDetector( ahlayerSD );//Grid scintillator for WAGASCI
  bmhsci_intLV->SetSensitiveDetector( ahlayerSD );//BabyMIND

  G4String vlayerSDname = "B2/vlayerSD";
  avlayerSD = new B2VLayerSD( vlayerSDname, mode );
  SDman->AddNewDetector( avlayerSD );
  vscint_intLV->SetSensitiveDetector( avlayerSD );//INGRID module
  vscint2_intLV->SetSensitiveDetector( avlayerSD );//SciBar type for Proton Module
  vscint3_intLV->SetSensitiveDetector( avlayerSD );//INGRID type for Proton Module
  vwscint_intLV->SetSensitiveDetector( avlayerSD );//Plane scintillator for WAGASCI
  vwgridscint_intLV->SetSensitiveDetector( avlayerSD );//Grid scintillator for WAGASCI
  smsci_intLV->SetSensitiveDetector( avlayerSD );//SMRD
  bmvsci_intLV->SetSensitiveDetector( avlayerSD );//BabyMIND
 
  G4String vetoSDname = "B2/vetoSD";
  avetoSD = new B2VetoSD( vetoSDname, mode );
  SDman->AddNewDetector( avetoSD );
  SvetoLV->SetSensitiveDetector( avetoSD );//Short veto for INGRID module
  LvetoLV->SetSensitiveDetector( avetoSD );//Long veto for INGRID module
  PSvetoLV->SetSensitiveDetector( avetoSD );//Short veto for Proton Module
  PLvetoLV->SetSensitiveDetector( avetoSD );//Long veto for Proton Module
  return worldPV;
}

//___________________________________________________________________________________________________________

void B2DetectorConstruction::DefineSpace()
{

  //Module box position
  if(mode==1){
    WagaboxposZ2=0;
    WagaboxposZ1=-0.58*m;
    PMboxposZ=-1.36*m;
  }
  else if(mode==2){
    WagaboxposZ2=0;
    PMboxposZ=-0.78*m;
    WagaboxposZ1=-1.56*m;
  }
  else if(mode==3){
    PMboxposZ=-0.2*m;
    WagaboxposZ2=-0.98*m;
    WagaboxposZ1=-1.56*m;
  }

  PMboxposX=0;
  PMboxposY=0;

  WagaboxposX1=0;
  WagaboxposY1=0;

  WagaboxposX2=0;
  WagaboxposY2=0;

  IngboxposX=0;
  IngboxposY=0;
  IngboxposZ=0;

  SMRDboxposX=1.08*m;
  SMRDboxposY=0;
  SMRDboxposZ=-0.33*m;

  BMboxposX=0.60*m;
  BMboxposY=0;
  BMboxposZ=2.75*m;

  //The length at World volume, volume of 7 modules, and module volume means the half length.
  WorldX = 25*m;
  WorldY = 25*m;
  WorldZ = 25*m;

  //B2 box
  B2hallposX=-5.722*m+5.722*m;
  B2hallposY=-4.444*m+4.444*m;
  B2hallposZ=4.2*m-4.2*m;

  //The dimension of Hall dirt volume
  HallDirtRadiusMin =  8.95*m;
  HallDirtRadiusMax = HallDirtRadiusMin+9*m;
  HallDirtHeight    = 10. *m;
  HallDirtSPhi      = 0;
  HallDirtDPhi      = 3.1415926535897932384626*2;
  HallDirtX         = -3.222*m+5.722*m;
  HallDirtY         = 0. *m+4.444*m;
  HallDirtZ         = 1.7*m-4.2*m;
  HallX             = HallDirtX;
  HallZ             = HallDirtZ;

  Floorup = -5.944*m;

  PillarX = 0.5*m;
  PillarY = 1.5*m;
  PillarZ = 2.0*m;

  PillarposX = -8.435*m+5.722*m+0.2*m;
  PillarposY = -4.443*m+4.444*m;
  PillarposZ = 2.631*m-4.2*m;
  PillarposX_rev = (-8.035 + 5.0)*m+5.722*m-0.2*m; 

  SMRD_Gap = 4.3*cm;
  Start_ironx_smrd = 23.*cm;
  Start_sciz_smrd = 80.*cm;

}

//___________________________________________________________________________________________________________

void B2DetectorConstruction::DefineStructures()
{
  fdim = new B2_Dimension();
  
  //B2 box
  B2hallX=1.95*m;
  B2hallY=1.45*m;
  B2hallZ=4.0*m;
  
  //Module box
  PMboxX=75*cm;
  PMboxY=75*cm;
  PMboxZ=45*cm;
  
  WagaboxX=75*cm;
  WagaboxY=75*cm;
  WagaboxZ=27*cm;
  
  IngboxX=75*cm;
  IngboxY=75*cm;
  IngboxZ=65*cm;
  
  SMRDboxX=25*cm;
  SMRDboxY=125*cm;
  SMRDboxZ=87*cm;
  
  BMboxX=200*cm;
  BMboxY=200*cm;
  BMboxZ=216*cm;
  
  //Iron blocks for INGRID
  iron_z= 3.25*cm;
  iron_xy= 62.0*cm;
  Gap= 10.7*cm;           // 6.5 + 2 +2*1.1 means " scinti-scinti-gap-iron-gap-scinti-scinti-... "
  iron_start= -48.15*cm;  // Position of the center of the first iron block  
                          // = -54.5cm(1st plane surface) + 2cm(width of 1 plane) + 1.1cm(gap of scinti-iron) +3.25cm(half of iron)

  //Central iron blocks for SMRD
  IronSMRDX = 3.0*cm/2;
  IronSMRDY = 1.8*m/2;
  IronSMRDZ = 1.61*m/2;

  //Front and rear iron blocks for SMRD
  FIronSMRDX = 3.0*cm/2;
  FIronSMRDY = 2.05*m/2;
  FIronSMRDZ = 1.61*m/2;

  //Front and rear iron blocks for SMRD
  SIronSMRDX = 0.46*m/2;
  SIronSMRDY = 1.90*m/2;
  SIronSMRDZ = 1.0*cm/2;

  //Iron blocks for Baby-MIND
  IronBMX=3.5*m/2;
  IronBMY=2.0*m/2;
  IronBMZ=3.0*cm/2;
  
  //Same as before, but doubles instead of G4doubles for use by other functions
  Niron_start=-48.15;
  Niron_z=3.25;
  Nscibar_start=-54.5;
  NGap=10.7;

  //Scintillators
  scibar_x= 0.6*m;        // 120cm long
  scibar_y=2.5*cm;        // 5cm wide
  scibar_z= 5.0*mm;       // 1cm Thick

  //Scibar type for proton module added by kikawa
  scibar2_y=1.25*cm;        // 2.5cm wide
  scibar2_z= 6.5*mm;       // 1.3cm thick

  //Proton Module
  Pscibar_start=-40.95*cm;  
  Pveto_start=-40.95*cm-1.55*cm;  

  scibar_start=-54.5*cm;  // in Module , which is surface of 1st tracking x-layer
  scibar_xy_start= -57.5*cm; // in Module   57.5 = 12mai*5cm - 5cm/2

  //Long veto planes
  Lveto_x= 0.65*m;        // 130cm long
  Lveto_y= 0.5*cm;        // 1cm thick
  Lveto_z= 2.5*cm;        // 5cm wide 
  veto_start=-52.5*cm;

  //Short veto planes
  Sveto_x=0.56*m;         // 112cm long
  Sveto_y=0.5*cm;         // 1cm thick
  Sveto_z=2.5*cm;         // 5cm wide 

  //Proton Module Long veto planes added by kikawa
  PLveto_x= 0.625*m;        // 125cm long
  PLveto_y= 0.5*cm;        // 1cm thick
  PLveto_z= 2.5*cm;        // 5cm wide 

  //Proton Module Short veto planes added by kikawa
  PSveto_x=0.6*m;         // 120cm long
  PSveto_y=0.5*cm;         // 1cm thick
  PSveto_z=2.5*cm;         // 5cm wide 

  //Distance between planes of Proton Module added by kikawa
  dist_first=2.7*cm;
  dist=2.3*cm;

  //SMRD scintillator dimension
  SciSMRDX = 0.7*cm/2;  
  SciSMRDY = 1.8*m/2;
  SciSMRDZ = 20*cm/2;

  //Baby-MIND horizontal scintilaltor dimension
  SciBMHX = 288.*cm/2;
  SciBMHY = 3.1*cm/2;
  SciBMHZ = 0.75*cm/2;

  //Baby-MIND horizontal scintilaltor dimension
  SciBMVX = 21.*cm/2;
  SciBMVY = 195.*cm/2;
  SciBMVZ = 0.75*cm/2;

  //INGRID Scintillator dimension for INGRID
  SciVertex_x[0] = -23.8*mm; 
  SciVertex_x[1] = -24.3*mm; 
  SciVertex_x[2] = -24.3*mm; 
  SciVertex_x[3] = -23.8*mm; 
  SciVertex_x[4] =  23.8*mm; 
  SciVertex_x[5] =  24.3*mm; 
  SciVertex_x[6] =  24.3*mm; 
  SciVertex_x[7] =  23.8*mm;

  SciVertex_y[0] = - 5. *mm; 
  SciVertex_y[1] = - 0.5*mm; 
  SciVertex_y[2] =   0.5*mm; 
  SciVertex_y[3] =   5. *mm; 
  SciVertex_y[4] =   5. *mm; 
  SciVertex_y[5] =   0.5*mm; 
  SciVertex_y[6] =  -0.5*mm; 
  SciVertex_y[7] =  -5. *mm; 

  //SciBar Scintillator dimension for PM
  SciVertex2_x[0] = -11.7*mm; 
  SciVertex2_x[1] = -12.25*mm; 
  SciVertex2_x[2] = -12.25*mm; 
  SciVertex2_x[3] = -11.7*mm; 
  SciVertex2_x[4] =  11.7*mm; 
  SciVertex2_x[5] =  12.25*mm; 
  SciVertex2_x[6] =  12.25*mm; 
  SciVertex2_x[7] =  11.7*mm;

  SciVertex2_y[0] = - 6.5 *mm; 
  SciVertex2_y[1] = - 3.5*mm; 
  SciVertex2_y[2] =   3.5*mm; 
  SciVertex2_y[3] =   6.5 *mm; 
  SciVertex2_y[4] =   6.5 *mm; 
  SciVertex2_y[5] =   3.5*mm; 
  SciVertex2_y[6] =  -3.5*mm; 
  SciVertex2_y[7] =  -6.5 *mm; 

  //INGRID Scintillator dimension for PM
  SciVertex3_x[0] = -23.65*mm; 
  SciVertex3_x[1] = -24.15*mm; 
  SciVertex3_x[2] = -24.15*mm; 
  SciVertex3_x[3] = -23.65*mm; 
  SciVertex3_x[4] =  23.65*mm; 
  SciVertex3_x[5] =  24.15*mm; 
  SciVertex3_x[6] =  24.15*mm; 
  SciVertex3_x[7] =  23.65*mm;

  SciVertex3_y[0] = - 5. *mm; 
  SciVertex3_y[1] = - 0.5*mm; 
  SciVertex3_y[2] =   0.5*mm; 
  SciVertex3_y[3] =   5. *mm; 
  SciVertex3_y[4] =   5. *mm; 
  SciVertex3_y[5] =   0.5*mm; 
  SciVertex3_y[6] =  -0.5*mm; 
  SciVertex3_y[7] =  -5. *mm; 

  //WAGASCI Scintillator dimension
  SciVertex4_x[0] = -10.4*mm;
  SciVertex4_x[1] = -11.8*mm;
  SciVertex4_x[2] = -11.8*mm;
  SciVertex4_x[3] = -10.4*mm;
  SciVertex4_x[4] =  10.9*mm;
  SciVertex4_x[5] =  11.8*mm;
  SciVertex4_x[6] =  11.8*mm;
  SciVertex4_x[7] =  10.9*mm;

  SciVertex4_y[0] = - 1.5*mm;
  SciVertex4_y[1] = - 0.75*mm;
  SciVertex4_y[2] =   0.75*mm;
  SciVertex4_y[3] =   1.5*mm;
  SciVertex4_y[4] =   1.5*mm;
  SciVertex4_y[5] =   1.0*mm;
  SciVertex4_y[6] =  -1.0*mm;
  SciVertex4_y[7] =  -1.5*mm;
}

//___________________________________________________________________________________________________________

void B2DetectorConstruction::DefineMaterial()
{
  G4double a;  // atomic mass
  G4double z;  // atomic number
  G4double density;
  G4String name, symbol;
  G4int nel;
  G4double iz;

  a = 14.01*g/mole;
  G4Element* elN = new G4Element(name="Nitrogen", symbol="N", iz=7., a);
  a = 16.00*g/mole;
  G4Element* elO = new G4Element(name="Oxigen", symbol="O", iz=8., a);
  a = 1.01*g/mole;
  G4Element* elH = new G4Element(name="Hydrogen", symbol="H", z=1., a);
  a = 12.01*g/mole;
  G4Element* elC = new G4Element(name="Carbon", symbol="C", z=6., a);
  a = 28.1*g/mole;
  G4Element* elSi = new G4Element(name="Silicon", symbol="Si", z=14., a);
  a = 40.1*g/mole;
  G4Element* elCa = new G4Element(name="Calusium", symbol="Ca", z=20., a);
  a = 23.0*g/mole;
  G4Element* elNa = new G4Element(name="Sodium", symbol="Na", z=11., a);
  a = 55.8*g/mole;
  G4Element* elFe = new G4Element(name="Iron", symbol="Fe", z=26., a);
  a = 27.0*g/mole;
  G4Element* elAl = new G4Element(name="Aluminium", symbol="Al", z=13., a);
  a = 58.69*g/mole;
  G4Element* elNi = new G4Element(name="Nickel", symbol="Ni", z=28., a);
  a = 51.99*g/mole;
  G4Element* elCr = new G4Element(name="Chromium", symbol="Cr", z=24., a);

  //Air
  density = 1.29*mg/cm3;
  Air = new G4Material(name="Air", density, nel=2);
  Air->AddElement(elN, .7);
  Air->AddElement(elO, .3);

  //Iron
  a = 55.845*g/mole;
  density = 7.86*g/cm3;
  Fe = new G4Material(name="Iron", z=26., a, density);

  //Water
  density = 1.000*g/cm3;
  Water = new G4Material(name="Water",density,nel=2);
  Water->AddElement(elH,2);
  Water->AddElement(elO,1);

  //Scintillator
  density = 1.032*g/cm3;
  Scinti = new G4Material(name="Scintillator", density, nel=2);
  Scinti->AddElement(elC, 9);
  Scinti->AddElement(elH, 10);

  //Concrete
  density = 2.2*g/cm3;
  Concrete = new G4Material(name="Concrete", density, nel=6);

  Concrete->AddElement(elO, .53);
  Concrete->AddElement(elSi, .335);
  Concrete->AddElement(elCa, 0.06);
  Concrete->AddElement(elNa, 0.015);
  Concrete->AddElement(elFe, 0.02);
  Concrete->AddElement(elAl, 0.04);

  //SUS304
  density = 7.93*g/cm3;
  SUS304 = new G4Material(name="SUS304", density, nel=3);
  SUS304->AddElement(elFe, 0.72);
  SUS304->AddElement(elCr, 0.19);
  SUS304->AddElement(elNi, 0.09);

}
