#ifndef _B2_Dimension_H
#define _B2_Dimension_H

#include<iostream>
#include<sstream>
#include<fstream>
#include "TFile.h"
#include "TTree.h"
#include"Const.hh"
using namespace std;

//Proton Module and INGRID
const static double IronThick       =   6.5   ;     //cm
const static double ScintiWidth     =   5.0   ;     //cm
const static double ScintiThick     =   1.0   ;     //cm
const static double PlnThick        =   4.2   ;     //cm
const static double VetoOffsetZX    =   -2.0  ;     //cm
const static double VetoStartZ      =   +1.0  ;     //cm
const static double VetoOffsetZY    =   -1.0  ;     //cm
const static double VetoOffsetRight = -10.575-2.5 ; //cm
const static double VetoOffsetLeft  = 130.9  -2.5 ; //cm
const static double VetoOffsetBottom=  -5.9   ;     //cm
const static double VetoOffsetUp    = 128.4   ;     //cm

const static double ScibarWidth     =   2.5   ;     //cm
const static double PlnThick_PM     =   4.6   ;     //cm
const static double PlnThick_front_PM=   5.0   ;     //cm
const static double PlnDist_PM      =   2.3   ;     //cm
const static double VetoOffsetZX_PM    =   -1.55  ;     //cm
const static double VetoOffsetZY_PM    =   -1.55  ;     //cm
const static double VetoOffsetRight_PM = -6 ; //cm
const static double VetoOffsetLeft_PM  = 125 ; //cm
const static double VetoOffsetBottom_PM=  -6   ;     //cm
const static double VetoOffsetUp_PM    = 125   ;     //cm
const static double VetoStartZ_PM      =   -0.55  ;     //cm

//WAGASCI
const int    wgs_chnum           = 40;
const double wgs_watersurface_z  = -23.3;//cm upstream water surface  now half of waterZ length (46.6cm)
const double wgs_firstdistance_z = 0.5;  //cm distance between water target and scintillator
const double wgs_firstoffset_z   = wgs_watersurface_z + wgs_firstdistance_z;//cm distance between water target and scintillator
const double wgs_scinti_thick    = 0.3;  //cm thickness of scinti
const double wgs_scinti_width    = 2.5;  //cm width of scinti
const double wgs_offsetxy        = - wgs_scinti_width * wgs_chnum/2.;
const double wgs_gap             = 5.7;  //cm gap between hlayer and hlayer
const double wgs_offset_hv       = 2.85; //cm gap between hlayer and vlayer
//WAGASCI grid layer
const double wgs_cutgap          = 5.0;   //cm gap of grid cut
const double wgs_cutwidth        = 0.35;  //cm width of grid cut
const double wgs_cutthick        = 1.3;   //cm thickness of grid cut
const double wgs_offsetxy_grid   = - wgs_scinti_width * wgs_chnum/2. +2.335;//cm distance between first grid and edge of scintillator


//SMRD
const double smrd_pln_gap     = 4.3;  //cm gap of SMRD scintillator planes
const double smrd_sci_width   = 20.0; //cm width of SMRD scintillator


//B2///////////////////
const static double B2ScintiWidthfor = 5.0;
const static double B2ScintiThickfor = 1.0;
const static double B2PlnThickfor = 4.2;
const static double B2IronThickfor = 3.5;

const static double B2ScintiWidthside = 5.0;
const static double B2ScintiThickside = 1.0;
const static double B2PlnThickside = 4.2;
const static double B2IronThickside = 3.5;

const static double B2ScintiWidthback = 5.0;
const static double B2ScintiThickback = 1.0;
const static double B2PlnThickback = 5.0;
//const static double B2IronThickback = 3.5;

//const static double B2ScintiWidtharoundwater = 5.0;
//const static double B2ScintiThickaroundwater = 1.0;
//const static double B2PlnThickaroundwater = 0.0;

const static double B2ScintiWidthinwater = 5.0;
const static double B2ScintiThickinwater = 0.3;
const static double B2PlnThickinwater = 15.0;



class B2_Dimension{
private:
  TFile* f;
  TTree* t;
  double position_xy[VIEWMAX][PLNMAX][CHMAX];
  double position_z [VIEWMAX][PLNMAX][CHMAX];
  double VETOOffsetZ;
public:
  B2_Dimension();
  ~B2_Dimension();

  bool get_pos(int mod, int pln, int ch, bool tpl, bool veto, double *posxy, double *posz);


  bool get_posXY(int mod, int view, int pln, int ch,
		 double *posxy, double *posz);
  bool get_posVeto(int mod, int view, int pln, int ch, 
		   double *posxy, double *posz);//for new data structure (not complete)

  bool get_expch(int mod, int pln, int *ch, bool tpl, bool veto, double a, double b);
  bool get_expchXY(int mod, int view, int pln, int *ch, double a, double b);

  bool get_pos_wgs(int mod, int view, int pln, int ch, int grid,
		    double *posx, double *posy, double *posz);
  bool get_pos_wgs(int mod, int view, int pln, int ch,
		    double *posx, double *posy, double *posz);
  bool get_grid_wgs(int mod, int view, int pln, int ch,
		     int *grid, int *gridch);
  bool get_pos_wgs_xy(int mod, int view, int pln, int ch,
		       double* posxy, double* posz);
  bool get_wgs_gridcell_id(int mod, int view, int pln, int ch, double posx, double posy, double posz,
			    int* gridcell_id_x1, int* gridcell_id_x2, int* gridcell_id_y1, int* gridcell_id_y2);
  bool get_pos_smrd(int mod, int pln, int ch, double *posx, double *posz);
  bool get_pos_bm(int mod, int view, int pln, int ch, double *posx, double *posy, double *posz);
  


};
#endif

