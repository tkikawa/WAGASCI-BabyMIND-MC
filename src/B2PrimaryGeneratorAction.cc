//#define DEBUG
#include "CLHEP/Random/Random.h"
#include "CLHEP/Random/RandGauss.h"
#include "Randomize.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "globals.hh"

#include "B2PrimaryGeneratorAction.hh"
#include "B2DetectorConstruction.hh"
#include "B2RunAction.hh"
#include "Const.hh"

using namespace CLHEP;

//const double offsetpm = 120; //cm
const double total_mass_fe = 99.54; //ton
const double total_mass_sci = 3.74; //ton
const double scinti_start = -54.5;  // the surface of the first scintillator
const double iron_start = scinti_start + 2.0 + 1.1;  // the surface fo the first scinti + 2 scnti + air gap
const double width_fe = 6.5; // cm
const double width_sci = 2.0; // 1.0cm * 2
const double GAP = 10.7; // 6.5 + 2 + 2.2

const double HallX = -322.2;  //cm
const double HallZ = 170.;    //cm
const double HallRadiusMax = 1695.;//cm
const double HallRadiusMin = 895.;//cm

//for Proton Module
const double total_mass_sci_pm = 0.569208 ;//ton (total mass)
const double total_mass_front_pm = 0.0288528 ;//ton (total mass of front veto)
const double ingrid_width = 1 ;//(cm) (total mass of ingrid type)
const double scibar_width = 1.3; //(cm) (total mass of scibar type)
const double width_ingrid =1.0; //INGRID type 
const double width_scibar =1.3; //SciBar type 
const double scibar_region=44.2;//(cm)
const double sciing_region=39.1;//(cm)
const double ingrid_region=34;//(cm)
const double Pscinti_start=-40.95;//(cm)
const double distance_first=2.7;//(cm)
const double distance_pln=2.3;//(cm)
const double diff=-0.15;//(cm) difference of scibar and ingrid type start


B2PrimaryGeneratorAction::B2PrimaryGeneratorAction(B2RunAction* rac, B2EventAction* evt,int nd, int vtx, int flavor0)
  :runaction(rac)
{
  eventaction = evt;
  module_mode = nd-1;
  vtxmod = vtx-1;
  neutrino_flavor = flavor0;
  G4int n_particle = 1;
  particleGun = new G4ParticleGun(n_particle);
  particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  double offz[3][3]=
    {-136.,-58.,0.,
     -78.,-156.,0.,
     -20.,-156.,-98.};
  offsetz = offz[module_mode][vtxmod];
  runaction->NotEntry = 0;
}


B2PrimaryGeneratorAction::~B2PrimaryGeneratorAction()
{
  if( particleGun!=NULL ) { delete particleGun; particleGun=NULL; }
}


void B2PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  SecondaryVector Secondary;
  int fdid = 0;
  int mode = 0;
  float pos[3];
  float direction[3];
  int vertex_flag=0;
  int flayer;
  double prob;//for Proton Module
  int scitype;//scintillator type


  //cosmic mode
  if(neutrino_flavor==-1){

    G4ParticleDefinition* particle;
    particle = particleTable->FindParticle(13);
    particleGun->SetParticleDefinition(particle);
    
    //set momentum
    G4float mass  = particle->GetPDGMass();
    double fitparam[5]={0.01371,-2.407,149.7,-247.0,131.9};
    G4float mumom;
    float mumom_calc;
    float rand_mom_calc;
    float func_calc;
    while(1){
      mumom_calc = G4UniformRand()*99+1; //1-100GeV                                                            
      func_calc  = fitparam[0] + fitparam[1]*pow(mumom_calc,-1) + fitparam[2]*pow(mumom_calc,-2) + fitparam[3]*pow(mumom_calc,-3) + fitparam[4]*pow(mumom_calc,-4);
      rand_mom_calc = G4UniformRand()*40;
      if(func_calc>rand_mom_calc){
	mumom=mumom_calc*GeV;
	break;
      }
    }
    G4float energy = sqrt(mumom*mumom+mass*mass) - mass;
    particleGun->SetParticleEnergy(energy);


    //set theta and position                                                                                   
    float pi_calc = 4.0*atan(1.0);
    float thetamax_calc =70./180.*pi; // refer to PDG2015                                                      
    float rand_theta_calc;
    float theta_calc,cos2theta_calc;
    float costheta_calc;
    float phi_calc;
    float xpos_calc;
    float ypos_calc;
    float zpos_calc;
    float px;
    float py;
    float pz;
    while(1){
      while(1){
	costheta_calc = G4UniformRand();
	phi_calc = 2*pi_calc*G4UniformRand();
	cos2theta_calc = costheta_calc*costheta_calc;
	rand_theta_calc = G4UniformRand();
	if(cos2theta_calc>rand_theta_calc){
	  theta_calc=acos(costheta_calc);
	  break;
	}
      }
      px = sin(phi_calc)*sin(theta_calc);
      py = -cos(theta_calc);
      pz = cos(phi_calc)*sin(theta_calc);

      //set position                                                                                           
      xpos_calc=(G4UniformRand()-0.5)*3000;
      ypos_calc=350;
      zpos_calc=(G4UniformRand()-0.5)*3000-120;

      //reject far event                                                                                       
      float point_0[3]={0,0,-125};
      float t = px*(point_0[0]-xpos_calc) + py*(point_0[1]-ypos_calc) + pz*(point_0[2]-zpos_calc);
      float point_1[3]={px*t+xpos_calc, py*t+ypos_calc, pz*t+zpos_calc};
      float dist = sqrt( pow(point_0[0]-point_1[0],2) + pow(point_0[1]-point_1[1],2) + pow(point_0[2]-point_1[2],2) );
      if(dist<sqrt(50*50+50*50+30*30)){
	break;
      }
    }
    particleGun->SetParticleMomentumDirection(G4ThreeVector(px, py, pz));
    G4float xpos = xpos_calc*cm;
    G4float ypos = ypos_calc*cm;
    G4float zpos = zpos_calc*cm;
    particleGun->SetParticlePosition(G4ThreeVector(xpos, ypos, zpos));
    particleGun->SetParticleTime(0.0*ns);
    
    B2SimVertexSummary* simvertex = new B2SimVertexSummary();
    simvertex -> Clear();
    simvertex -> nutype   = neutrino_flavor;
    simvertex -> inttype  = -100;
    //simvertex -> nuE      = energy/GeV;                                                                      
    simvertex -> nuE      = mumom/GeV;
    simvertex -> xnu      = xpos/cm;
    simvertex -> ynu      = ypos/cm;
    simvertex -> znu      = zpos/cm;
    simvertex -> gmomx.push_back(px);
    simvertex -> gmomy.push_back(py);
    simvertex -> gmomz.push_back(pz);
    runaction  -> GetEvtSum() -> AddSimVertex( simvertex );
    
    particleGun->GeneratePrimaryVertex(anEvent);
    return;
  }
  
    

  //Neutirno mode
  while(1){// start loop of neut file

    mode = NtupleReadEvent(Secondary, direction);
    if( mode==-1111 ){
      G4cout <<"Aboart Run (mode =" << mode << G4endl;
      G4RunManager* runManager = G4RunManager::GetRunManager();
      eventaction->SetWriteFlag(-1); 
      runManager->AbortRun(1);
      return;
    }

    // Check flavor ====================
    int  neutrino_flavor_tmp =  (int)((Vector.Neutrino.ProductionMode)/10);
    if( neutrino_flavor_tmp != neutrino_flavor ) {
#ifdef DEBUG
      G4cout << " === This neutrino id : " << neutrino_flavor_tmp
             << " ,not selected now === " << G4endl;
#endif
      continue;
    }

    // Define neutrino interaction vertex
    fdid = Vector.Neutrino.FDID;

    // X-Y vertex
    pos[0] = Vector.Neutrino.x;
    pos[1] = Vector.Neutrino.y;

    //Vertex in Proton Module
    if( vtxmod == 0 &&
	fabs(pos[0]) <= 60 &&
	fabs(pos[1]) <= 60) {

      double front = total_mass_front_pm / total_mass_sci_pm;

      if( front > (G4UniformRand()) ){
        vertex_flag=0;
        prob=1;
      }
      else if(fabs(pos[0])<=20&&fabs(pos[1])<=20){
        vertex_flag=1;
        prob=1;
      }
      else if(fabs(pos[0])<=20){
        vertex_flag=2;
        prob=sciing_region/scibar_region;
      }
      else if(fabs(pos[1])<=20){
        vertex_flag=3;
        prob=sciing_region/scibar_region;
      }
      else{
        vertex_flag=4;
        prob=ingrid_region/scibar_region;
      }

      if(prob<(G4UniformRand()))continue;

      // unit of pos is [cm]
      if( vertex_flag==0 ) {
        flayer = (int)(2*(G4UniformRand()));      // 0 < 2*rand < 2
        pos[2] = width_ingrid*(G4UniformRand());
        pos[2] = pos[2] + Pscinti_start + flayer*distance_pln;
      }
      else if( vertex_flag==1 ) {
        flayer = (int)(34*(G4UniformRand()));     // 0 < 34*rand < 34
        pos[2] = width_scibar*(G4UniformRand());
        pos[2] = pos[2] + Pscinti_start + distance_pln + diff + distance_first + flayer*distance_pln;
      }
      else if( vertex_flag==2){
        scitype= (int)((G4UniformRand())/scibar_width*(ingrid_width+scibar_width));
        flayer = (int)(17*(G4UniformRand()));     // 0 < 17*rand < 17
        if(scitype==0){
          pos[2] = width_scibar*(G4UniformRand());
          pos[2] = pos[2] + Pscinti_start + 2*distance_pln + diff + distance_first + flayer*distance_pln*2;
        }
        else{
          pos[2] = width_ingrid*(G4UniformRand());
          pos[2] = pos[2] + Pscinti_start + distance_pln + distance_first + flayer*distance_pln*2;
	}
      }
      else if( vertex_flag==3){
        scitype= (int)((G4UniformRand())/scibar_width*(ingrid_width+scibar_width));
        flayer = (int)(17*(G4UniformRand()));     // 0 < 17*rand < 17
        if(scitype==0){
          pos[2] = width_scibar*(G4UniformRand());
          pos[2] = pos[2] + Pscinti_start + distance_pln + diff + distance_first + flayer*distance_pln*2;
        }
        else{
          pos[2] = width_ingrid*(G4UniformRand());
          pos[2] = pos[2] + Pscinti_start + 2*distance_pln + distance_first + flayer*distance_pln*2;
        }
      }
      else if( vertex_flag==4){
        flayer = (int)(34*(G4UniformRand()));     // 0 < 34*rand < 34
        pos[2] = width_ingrid*(G4UniformRand());
        pos[2] = pos[2] + Pscinti_start + distance_pln + distance_first + flayer*distance_pln;
      }
      break;
    }//end of Proton Module
    //Vertex in WAGASCI
    else if( (vtxmod == 1 || vtxmod == 2) &&
	     fabs(pos[0]) <= 125.6/2. &&
	     fabs(pos[1]) <= 125.6/2.) {
      pos[2]=-25.0+50.0*(G4UniformRand());
      break;
    }
    //Vertex in Side-MRD
    else if( (vtxmod == 3) &&
             pos[0] <= -46. &&
             pos[0] >= -70. &&
             fabs(pos[1]) <= 115.){
      pos[0] = pos[0] - 50.;
      pos[1] = pos[1]*115./70.;
      pos[2] = -40.0+80.0*(G4UniformRand());
      break;
    }
    else if( (vtxmod == 4) &&
             pos[0] >= 46. &&
             pos[0] <= 70. &&
             fabs(pos[1]) <= 115.){
      pos[0] = pos[0] + 50.;
      pos[1] = pos[1]*115./70.;
      pos[2]=-40.0+80.0*(G4UniformRand());
      break;
    }

    // count events which have vertex out of modules
#ifdef DEBUG
    G4cout << "##### Interaction vertex is out of modules #####" << G4endl;
    G4cout << "##### Skip this event                      #####" << G4endl;
#endif
    runaction->NotEntry++; 

  }// end while loop


  //Set detector offset
  pos[2] = pos[2] + offsetz;

  // Input Neut file info to output ROOT class
  Vector.ID = vtxmod;
  for(int i=0;i<3;i++) (runaction->vertex)[i] = pos[i];
  
  B2SimVertexSummary* simvertex = new B2SimVertexSummary();
  simvertex -> Clear();
  simvertex -> nutype   = neutrino_flavor;
  simvertex -> inttype  = Vector.Primary.Mode;
  simvertex -> nuE      = Vector.Neutrino.Energy;
  simvertex -> xnu      = pos[0];
  simvertex -> ynu      = pos[1];
  simvertex -> znu      = pos[2];
  simvertex -> mod      = vtxmod;
  simvertex -> norm	= Vector.Neutrino.Norm;
  simvertex -> totcrsne	= Vector.neutcrs.Totcrsne;
  
  int npi_plus=0,npi_minus=0,npi_zero=0, npro=0, nneu=0;
  for(int ipart=0; ipart<Secondary.NumParticle; ipart++) {
    if( Secondary.TrackingFlag[ipart]==1 ) {
      if(Vector.Secondary.ParticleID[ipart]==211)npi_plus++;
      if(Vector.Secondary.ParticleID[ipart]==-211)npi_minus++;
      if(Vector.Secondary.ParticleID[ipart]==111)npi_zero++;
      if(Vector.Secondary.ParticleID[ipart]==2212)npro++;
      if(Vector.Secondary.ParticleID[ipart]==2112)nneu++;
    }
  }

  runaction  -> GetEvtSum() -> AddSimVertex( simvertex );

  G4cout.precision( 3 );

#ifdef DEBUG
  G4cout << "\n=== Neutrino Information from Jnubeam ===" << G4endl;
  G4cout << "Norm: " <<  Vector.Neutrino.Norm << G4endl;
  G4cout << "Totcrsne: " <<  Vector.neutcrs.Totcrsne << G4endl;
  G4cout << "ParentID: " << Vector.Neutrino.ParentID;
  G4cout << "  Neut Production Mode: " << Vector.Neutrino.ProductionMode;
  G4cout << "  Neutrino.FDID: " << Vector.Neutrino.FDID << G4endl;
  G4cout << "Neut interaction Mode: " << Vector.Primary.Mode << G4endl;
  G4cout << "Energy[GeV]: " << Vector.Neutrino.Energy;
  G4cout << "  Direction: {" << direction[0] << ", " << direction[1] << ", " << direction[2] << "}" << G4endl;
  G4cout << "Vertex(cm): {" << pos[0] << ", "<< pos[1] << ", "<< pos[2] << "}";
  G4cout << "  Module: " << vtxmod << "\n\n";
#endif

  particleGun->SetParticlePosition(G4ThreeVector(pos[0]*cm,pos[1]*cm,pos[2]*cm));
  
  // Input Neut info for T2KReWeight to SK__h1 class
  runaction -> numnu = Vector.Primary.NumParticle;
  runaction -> mode  = Vector.Primary.Mode;
  for ( int i = 0; i<50; i++ ) {
    runaction -> ipnu[i] = Vector.Primary.ParticleID[i];
    runaction -> pnu[i] = Vector.Primary.AbsMomentum[i];
    for ( int j = 0 ; j < 3 ; j++ ){
      runaction -> dirnu[i][j] = Vector.Primary.Momentum[i][j] / Vector.Primary.AbsMomentum[i];
    }
  }
  
  runaction -> Crsx   = Vector.Crs.Crsx;
  runaction -> Crsy   = Vector.Crs.Crsy;
  runaction -> Crsz   = Vector.Crs.Crsz;
  runaction -> Crsphi = Vector.Crs.Crsphi;
  
  runaction -> Nvert = Vector.Fsi.Nvert;
  for (int ivert=0; ivert<150; ivert++) {
    runaction -> Iflgvert[ivert] = Vector.Fsi.Iflgvert[ivert];
    for (int j=0; j<3; j++)
      runaction -> Posvert[ivert][j] = Vector.Fsi.Posvert[ivert][j];
  }
  
  runaction -> Nvcvert = Vector.Fsi.Nvcvert;
  for (int ip=0; ip<900; ip++) {
    
    runaction -> Abspvert[ip]  = Vector.Fsi.Abspvert[ip];
    runaction -> Abstpvert[ip] = Vector.Fsi.Abstpvert[ip];
    runaction -> Ipvert[ip]    = Vector.Fsi.Ipvert[ip];
    runaction -> Iverti[ip]    = Vector.Fsi.Iverti[ip];
    runaction -> Ivertf[ip]    = Vector.Fsi.Ivertf[ip];
    
    for (int j=0; j<3; j++)
      runaction -> Dirvert[ip][j] = Vector.Fsi.Dirvert[ip][j];
  }
  
  runaction -> Fsiprob = Vector.Fsi.Fsiprob;
  runaction -> Numbndn = Vector.target_info.Numbndn;
  runaction -> Numbndp = Vector.target_info.Numbndp;
  runaction -> Numfrep = Vector.target_info.Numfrep;
  runaction -> Numatom = Vector.target_info.Numatom;
  runaction -> Ibound  = Vector.Fsi.Ibound;
  runaction -> Npvc    = Vector.Secondary.NumParticle;
  for (int i=0; i<100; i++) {
    runaction -> Ipvc[i]    = Vector.Secondary.ParticleID[i];
    runaction -> Ichvc[i]   = Vector.Secondary.TrackingFlag[i];
    runaction -> Iorgvc[i]  = Vector.Secondary.ParentID[i];
    runaction -> Iflvc[i]   = Vector.Secondary.InteractionCode[i];
    runaction -> Abspvc[i]  = Vector.Secondary.AbsMomentum[i];
    for (int j=0; j<3; j++)
      runaction -> Pvc[i][j]     = Vector.Secondary.Momentum[i][j];
  }
  
  
  // #############################################################################
  // ### Fill primary state info of partcle generated at neutrino interaction
  // #############################################################################
  /*
    NeutInfoSummary* neutinfo = new NeutInfoSummary();
    neutinfo -> Clear();
    neutinfo -> Mode = Vector.Primary.Mode;
    neutinfo -> Numnu = Vector.Primary.NumParticle;
    for(int i=0;i<(neutinfo->Numnu);i++) {
    neutinfo -> Ipnu[i] = Vector.Primary.ParticleID[i];
    neutinfo -> Abspnu[i] = Vector.Primary.AbsMomentum[i];
    for(int j=0;j<3;j++) neutinfo -> Pnu[i][j] = Vector.Primary.Momentum[i][j];
    }
    runaction -> GetEvtSum() -> AddNeut( neutinfo );
  */
  for(int ipart=0; ipart<Secondary.NumParticle; ipart++) {
    // ##############################################################################
    // ### consider only TrackingFlag for use non interacted particle in neucleus ###
    // ##############################################################################
    if( Secondary.TrackingFlag[ipart]==1 ) {

#ifdef DEBUG
      G4cout << "Particle: " << Vector.Secondary.ParticleID[ipart] << G4endl;
      G4cout << "Index: " << ipart << G4endl;
      G4cout << "Parent Index: " << Vector.Secondary.ParentID[ipart] -1 << G4endl;
      G4cout << "Tracking Flag: " << Vector.Secondary.TrackingFlag[ipart] << G4endl;
      G4cout << "Interaction code: " << Vector.Secondary.InteractionCode[ipart] << G4endl;
      G4cout << "Momentum[MeV/c]: ";
      for (int k=0;k<3;k++)   G4cout << Vector.Secondary.Momentum[ipart][k] << " ";
      G4cout << G4endl;
#endif
      
      G4ParticleDefinition* particle;
      particle = particleTable->FindParticle(Secondary.ParticleID[ipart]);
      particleGun->SetParticleDefinition(particle);
      
      double nvec[3];
      for(int ixyz=0; ixyz<3; ixyz++)
	nvec[ixyz] = Secondary.Momentum[ipart][ixyz]/ Secondary.AbsMomentum[ipart];
      G4ThreeVector dir(nvec[0], nvec[1], nvec[2]);
      particleGun->SetParticleMomentumDirection(dir);
      
      G4double mass = particle->GetPDGMass();
      G4double mom = Secondary.AbsMomentum[ipart]*MeV;
      G4double energy = sqrt(mass*mass+mom*mom) - mass;
      particleGun->SetParticleEnergy(energy);
      
      particleGun->SetParticleTime(0.0*ns);
      
      //
      particleGun->GeneratePrimaryVertex(anEvent);
      
#ifdef DEBUG
      G4cout << "ipart: " << ipart << "\n";
      G4cout << "PID:" << Vector.Secondary.ParticleID[ipart] << "\n";
      G4cout << "Tracking Flag: " << Vector.Secondary.TrackingFlag[ipart] << "\n";
      G4cout << "Kinetic Energy[MeV]: " << energy << G4endl;;
      G4cout << "Momentum[MeV/c]: ";
      for (int k=0;k<3;k++)   G4cout << Vector.Secondary.Momentum[ipart][k] << " ";
      G4cout << G4endl;;
#endif
      
    } // end of if
  } // end of for loop
}
