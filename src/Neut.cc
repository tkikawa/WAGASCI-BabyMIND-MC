/*
  Note: the "-no-pie" flag is needed for gcc 5+, e.g.:
  g++ -no-pie -Wno-write-strings -DgFortran -I${CERN}/${CERN_LEVEL}/include/cfortran -o trial trial.cxx `cernlib packlib` -lgfortran
*/

// system includes
#include <iostream>
#include <cstdlib>

// CERNLIB includes 
#include "cfortran/cfortran.h"
#include "cfortran/packlib.h"

// user includes
#include "Neut.hh"

int zero = 0;
int ierr = 0;
NeutVector Vector;

int NtupleReadInit(char *ntpfile)
{
  // first things first!

  PAWC.PAW[0] = 0; // precaution: make sure that the PAWC is really allocated
  HLIMIT(PAWC_SIZE);

  /*
    Note: some C++ compilers generate a memset error when a character string
    is passed directly as argument to a F77 function. Using an intermediate
    variable makes it work.
  */

  std::cout << "Read NEUT file ......" << std::endl;
  int istat = 0, lrec = 1024;
  HROPEN(60, "neut", ntpfile, " ", lrec, istat);
  if (istat) {
    // std::cout << "Error in opening file ..." << std::endl;
    return 0;
  }
  std::cout << "===== lrec: " << lrec << std::endl;
  std::cout << "==== ntpfile istat: " << istat << std::endl;

  HRIN(NTNEUT, 999999, 0);
  HBNAME(NTNEUT, " ", zero, "$CLEAR");

  HBNAME(NTNEUT, "NEUTINFO", Vector.Primary,     "$SET");
  HBNAME(NTNEUT, "FSIHIST",  Vector.Fsi,         "$SET"); // for t2kreweight
  HBNAME(NTNEUT, "VECINFO",  Vector.Secondary,   "$SET");
  HBNAME(NTNEUT, "CRSINFO",  Vector.neutcrs,     "$SET"); // for t2kreweight
  HBNAME(NTNEUT, "NUINFO",   Vector.Neutrino,    "$SET");
  HBNAME(NTNEUT, "TARGINFO", Vector.target_info, "$SET");

  istat = 0;
  HGNTB(NTNEUT, "NEUTINFO", 1, istat); // retrieve the first entry
  istat = 0;
  HGNTB(NTNEUT, "FSIHIST",  1, istat);
  istat = 0;
  HGNTB(NTNEUT, "VECINFO",  1, istat);
  istat = 0;
  HGNTB(NTNEUT, "CRSINFO",  1, istat);
  istat = 0;
  HGNTB(NTNEUT, "NUINFO",   1, istat);
  istat = 0;
  HGNTB(NTNEUT, "TARGINFO", 1, istat);

  HNOENT(NTNEUT, Vector.NumberOfEvents);

  HGNT(NTNEUT, 1, ierr);
  
  return Vector.NumberOfEvents;
}

int NtupleReadEvent(SecondaryVector &Secondary, float* direction)
{
  static int iev=0;  
  iev++;

  // **************************************
  // code ajouste...

  if(iev%1000==0) {
    std::cout << iev << std::endl;
  }
  
#if 0
  std::cout << "====== Neut file infomation =========" << std::endl;
  std::cout << "Event number: " << Vector.EventNumber << "\n\n";
  std::cout << "Primary:\n";
  std::cout << "Neutirno Interaction Mode: " << Vector.Primary.Mode << "\n";
  std::cout << "Number of primary particles: " << Vector.Primary.NumParticle << "\n";

  std::cout << "Primary Particles:\n\n";
  //for (int j=0;j<Vector.Primary.NumParticle+5;j++){
  for (int j=0;j<Vector.Primary.NumParticle+1;j++){
	std::cout << "#" << j << "\n";
	std::cout << "ID: " << Vector.Primary.ParticleID[j] << "\n";
	std::cout << "Absolute momentum: " << Vector.Primary.AbsMomentum[j] << "\n";
	std::cout << "Momentum: ";
	for (int k=0;k<3;k++)
	  std::cout << Vector.Primary.Momentum[j][k] << " ";
	std::cout << "\n\n";
  }
    
  std::cout << "Neutrino:\n";
  std::cout << "ParentID: " << Vector.Neutrino.ParentID << "\n";
  std::cout << "Production Mode: " << Vector.Neutrino.ProductionMode << "\n";
  std::cout << "FDID: " << Vector.Neutrino.FDID << "\n";
  std::cout << "Weighting Factor: " << Vector.Neutrino.WeightingFactor << "\n";
  std::cout << "Energy: " << Vector.Neutrino.Energy << "\n";

  // In this programme, neutrino-ID = 0 (mother-particle), so there is no data for Parent Decay and Parent Production
   
  std::cout << "Parent Decay:\n ";
  //std::cout << "absMomentum: " << Vector.Neutrino.ParentDecay.AbsMomentum << "\n";
  std::cout << "vertex: ";
  for (int k=0;k<3;k++)
	std::cout << Vector.Neutrino.ParentDecay.Vertex[k] << " ";
  std::cout << "\ndirection: ";
  for (int k=0;k<3;k++)
	std::cout << Vector.Neutrino.ParentDecay.Direction[k] << " ";
  std::cout << "\ncosBeam: " << Vector.Neutrino.ParentDecay.CosBeam << "\n";
  std::cout << "Parent Production:\n ";
  std::cout << "absMomentum: " << Vector.Neutrino.ParentProduction.AbsMomentum << "\n"; 
  std::cout << "vertex: ";
  for (int k=0;k<3;k++)
	std::cout << Vector.Neutrino.ParentProduction.Vertex[k] << " ";
  std::cout << "\ndirection: ";
  for (int k=0;k<3;k++)
	std::cout << Vector.Neutrino.ParentProduction.Direction[k] << " ";
  std::cout << "\ncosBeam: " << Vector.Neutrino.ParentProduction.CosBeam << "\n";


  std::cout << "r: " << Vector.Neutrino.r << " x: " << Vector.Neutrino.x << " y: " << Vector.Neutrino.y << "\n";
  std::cout << "Neutrino direction: ";
  for (int k=0;k<3;k++)
	std::cout << Vector.Neutrino.Direction[k] << " ";
  std::cout << "\n\n";

  std::cout << "Secondary Infomation:\n";
  std::cout << "Number of secondary particles: " << Vector.Secondary.NumParticle << "\n";
  std::cout << "Secondary Particles:\n\n";
  //for (int j=0;j<Vector.Secondary.NumParticle+12;j++){
  for (int j=0;j<Vector.Secondary.NumParticle;j++){
	std::cout << "Index: " << j << "\n";
	std::cout << "ParticleID: " << Vector.Secondary.ParticleID[j] << "\n";
	std::cout << "ParentID: " << Vector.Secondary.ParentID[j] << "\n";
	std::cout << "Tracking Flag: " << Vector.Secondary.TrackingFlag[j] << "\n";
	std::cout << "Interaction Code: " << Vector.Secondary.InteractionCode[j] << "\n";
	std::cout << "Absolute Momentum: " << Vector.Secondary.AbsMomentum[j] << "\n";
	std::cout << "Momentum: ";
	for (int k=0;k<3;k++)
	  std::cout << Vector.Secondary.Momentum[j][k] << " ";
	std::cout << "Vertex: ";
	for (int k=0;k<3;k++)
	  std::cout << Vector.Secondary.Vertex[k] << " ";
	std::cout << "\n\n";
  }
  std::cout << "\nVertex: ";
  for (int k=0;k<3;k++)
    std::cout << Vector.Secondary.Vertex[k] << " ";
  std::cout << "\n\n********\n\n";

#endif

#if 1
  if(iev > Vector.NumberOfEvents) {
	//if(iev>10) {
    std::cout << "End Read Event" << std::endl;
    Vector.Primary.Mode = -1111;
    return Vector.Primary.Mode;
  }
#endif

  //

  HGNTF(NTNEUT, iev, ierr);

  //
  /*
	std::cout << "\n**************************************************************" << std::endl;
	std::cout << "************ Input Neut file infomation (after HGNTF) ***********" << std::endl;
	std::cout << "**************************************************************" << std::endl;
	std::cout << " Norm: " << Vector.Neutrino.Norm << "\n";
	std::cout << " GPOS[0]: " << Vector.Neutrino.primary_particle.gpos0[0] << "\n";
	std::cout << " GPOS[1]: " << Vector.Neutrino.primary_particle.gpos0[1] << "\n";
	std::cout << " GPOS[2]: " << Vector.Neutrino.primary_particle.gpos0[2] << "\n";
	std::cout << " Energy: " << Vector.Neutrino.Energy << "\n";
	std::cout << " Crsenergy: " <<Vector.neutcrs.Crsenergy << std::endl;
	std::cout << " Totcrsne: " <<Vector.neutcrs.Totcrsne << std::endl;
  */

  //
  Secondary = Vector.Secondary;
	
  for(int i=0;i<3;i++){
    direction[i] = Vector.Neutrino.Direction[i];
  }

  return Vector.Primary.Mode;

}
