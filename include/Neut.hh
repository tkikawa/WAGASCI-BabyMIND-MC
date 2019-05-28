#ifndef Neut_h
#define Neut_h 1

// CERNLIB includes 
#include "cfortran/cfortran.h"
#include "cfortran/packlib.h"

#define NVEC 100
#define NG 20
#define JNUBEAM10C
#define NTNEUT 10

/*
  Note: the PAWC and all ntuple variables and structures MUST be here,
  64 bit architectures require this to be loaded in the 32 bit address space.
*/

// define the PAWC common
#define PAWC_SIZE 500000
typedef struct { float PAW[PAWC_SIZE]; } PAWC_DEF;
#define PAWC COMMON_BLOCK(PAWC, pawc)
COMMON_BLOCK_DEF(PAWC_DEF, PAWC);

//
typedef struct {
  int NumParticle;
  int ParticleID[NVEC];
  int ParentID[NVEC];
  int TrackingFlag[NVEC];
  int InteractionCode[NVEC];
  float AbsMomentum[NVEC];
  float Momentum[NVEC][3];
  float Vertex[3];
} SecondaryVector;

//
typedef struct {
  int gipart;
  float gpos0[3];
  float gvec0[3];
  float gamom0;
} PrimaryParticle;

typedef struct {//for t2kreweight
  float         Crsx;
  float         Crsy;
  float         Crsz;
  float         Crsphi;
  float         Crsq2;
} CrsInfo;

typedef struct {//for t2kreweight
  int           Nvert;
  float         Posvert[100][3];
  int           Iflgvert[100];
  int           Nvcvert;
  float         Dirvert[300][3];
  float         Abspvert[300];
  float         Abstpvert[300];
  int           Ipvert[300];
  int           Iverti[300];
  int           Ivertf[300];
  float         Fsiprob;
  int           Ibound;
} FsiHist;


//
typedef struct {
  int spid;
  int pgen;
  float psi0,xsi0[3],nsi0[3],cossi0bm;
  float xsi[3];
  int smech;
  int intgt;
  float prvtx[3];
  int smed;
  int gppid;
  float xgpi0[3],xgpi[3],pgpi0;
  int gpmech, gpmed;
  int prmech, prmed;
  int prdght, sdght, gpdght;
  int chain;
} OptionalVariables;

//
typedef struct {
  int ng;
  float gpx[NG],gpy[NG],gpz[NG],gcosbm[NG];
  float gvx[NG],gvy[NG],gvz[NG];
  int gpid[NG],gmec[NG];
} Ancestor;

//
typedef struct {
  float Enusk;
  float normsk;
  float anorm;
} SKInfo;

//
typedef struct {
  int Numbndn;
  int Numbndp;
  int Numfrep;
  int Numatom;
} TargetInfo;

//
typedef struct {
  float Crsenergy;
  float Totcrsne;		// total cross-section
  float Difcrsne[8];
} NeutCrs;

//
typedef struct{
  int EventNumber;
  int Mode;
  int NumParticle;
  int ParticleID[NVEC];
  float AbsMomentum[NVEC];
  float Momentum[NVEC][3];
} PrimaryVector;

//
typedef struct {
  float AbsMomentum;
  float Vertex[3];
  float Direction[3];
  float CosBeam;
} ParentVector;

//
typedef struct{
  float Energy;
  int ParentID, ProductionMode;
  ParentVector ParentDecay;
  float Norm;
  int nvtx0;
  ParentVector ParentProduction;
  float r, x, y, Direction[3];
  int FDID;
  PrimaryParticle primary_particle;
  /*
  //removed before using t2kreweight
  #ifdef JNBEAM10A
  OptionalVariables optional_variables;
  #endif
  #ifdef JNUBEAM10C
  Ancestor ancestor;
  SKInfo sk_info;
  #endif
  */
} NeutNeutrino; 

typedef struct {
  int NumberOfEvent;
  int ID;
  
  PrimaryVector   Primary;
  FsiHist         Fsi;//for t2kreweight
  SecondaryVector Secondary;
  NeutNeutrino Neutrino;
	
  NeutCrs neutcrs;//for t2kreweight
  CrsInfo Crs;//for t2kreweight

  TargetInfo target_info;//for t2kreweight
  //NeutCrs neutcrs;//moved for t2kreweight
} NeutVector;

int NtupleReadInit(char*);
int NtupleReadEvent(SecondaryVector&, float*);

extern NeutVector Vector;
extern int zero;
extern int ierr;

#endif
