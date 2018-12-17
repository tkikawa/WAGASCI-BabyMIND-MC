#include "B2Response.hh"
#include <CLHEP/Random/Randomize.h>
#include <algorithm>
#include <TRandom.h>

using namespace std;

const G4double scilen_pm  = 120.;  //cm
const G4double scilen_wgs = 100.;  //cm
const G4double longlen = 130.;  //cm
const G4double longlen_pm = 120.;  //cm
const G4double shortlen = 112.;  //cm
const G4double attleng = 241.7; //WLS fiber attenulation length (cm)
const G4double sciattleng_pm = 10.46; //Proton Module scintillator attenulation lencth (cm)
const G4double sciattleng_wgs = 4.0; //WAGASCI scintillator attenulation lencth (cm)
const G4double SciBarFactor = 1.77;  //P.E. factor for SciBar scintillator
const G4double WagasciFactor = 3.0;  //P.E. factor for WAGASCI scintillator
const G4double CBIRKS = 0.0208; // Birks' constant used in SciBooNE MC
const G4double TransTimeInFiber = 1./28.;  //  1cm/2.8e10[cm/s] * 1e9 [ns]
const G4double Pedestal = 0;// pedeltal of ADC counts
const G4double Gain = 10;  // Gain ADC counts of high gain channel
const G4double LowGain  = 1;  // Gain ADC counts of low gain channel
const G4double ElecNoise = 1.7;  // Sigma of high gain electronics noise
const G4double LowElecNoise = 1.2;  // Sigma of low gain electronics noise
const G4double PixelGainVari_pm  = 0.031;  // Gain variation among pixels for Proton Module
const G4double PixelGainVari_wgs = 0.031;  // Gain variation among pixels for WAGASCI

const G4double MeV2PE_pm = 44.8;//MeV to PE conversion for Proton Module
const G4double MeV2PE_wgs = 31.5;//MeV to PE conversion for WAGASCI
const G4double MPPCPixel = 667.;//Number of MPPC pixels
const G4double Eff_PDE_pm = -0.275;//PDE for Proton Module
const G4double Eff_PDE_wgs = -1.;//PDE for WAGASCI
const G4double ETC_CONST = 5.0;
const G4double rPDE = 1.7;
const G4double cross_after_rate_pm = 0.09;//Cross talk and after pulse rate for Proton Module
const G4double cross_after_rate_wgs = 0.18;//Cross talk and after pulse rate for WAGASCI

const G4double glueshift = 0.39; // Glue shift in WAGASCI (cm)
const G4double fiberbundle = 20; // Fiber length between scinti and bundle for WAGASCI (cm)

//
const G4int topview = 1;
const G4int sideview = 0;

//
B2Response::B2Response()
{
  fdim = new B2_Dimension();
}

//
B2Response::~B2Response()
{
}

//
void B2Response::ApplyScintiResponse(G4double* edep, G4Track* aTrack)
{
  //Quenching effect by Birks' law (commonly used for all kinds of scintillators)
  G4double              kineticE = aTrack->GetKineticEnergy();
  G4ParticleDefinition* particle = aTrack->GetDefinition();
  G4Material*           material = aTrack->GetMaterial();
  if(particle->GetPDGCharge()==0) return;
  G4double dedx = emcal.GetDEDX(kineticE, particle, material)/(MeV/cm);
  (*edep) = (*edep)/(1. + CBIRKS*dedx);
  return;
}

void B2Response::ApplyLightCollection(G4double* edep, G4int mod, G4int view, G4ThreeVector pos, G4int pln, G4int ch ){
  
  G4double x = 0.;
  G4int i = 0;

  if(mod==0){//Proton Module
    if( view==topview ) x = fabs(scilen_pm/2. + pos[0]/cm);
    else if( view==sideview ) x = fabs(scilen_pm/2. + pos[1]/cm);
  
    if(pln==0){
      i=(int)x/5;
      x=fabs(x-i*5-2.5);
      *edep *= exp(-1.*x/sciattleng_pm);
    }
    else{
      if(x<40||x>80){
	i=(int)x/5;
	x=fabs(x-i*5-2.5);
	*edep *= exp(-1.*x/sciattleng_pm);
      }
      else{
	i=(int)x/2.5;
	x=fabs(x-i*2.5-1.25);
	*edep *= exp(-1.*x/sciattleng_pm)*SciBarFactor;
      }
    }
  }
  else if(mod==1 || mod==2){//WAGASCI
    double scposx,scposy,scposz;
    int grid,gridch;
    fdim -> get_pos_wgs( mod, view, pln, ch, &scposx, &scposy, &scposz);
    fdim -> get_grid_wgs( mod, view, pln, ch, &grid, &gridch );
    if( view==topview ){
      if(grid==0){
	x = fabs(pos[0]/cm - (scposx-glueshift) );
      }
      else if(grid==1 || grid==2){
	x = fabs(pos[2]/cm - (scposz+glueshift) );
      }
    }
    else if( view==sideview ){
      if(grid==0){
	x = fabs(pos[1]/cm - (scposy-glueshift) );
      }
      else if(grid==1 || grid==2){
	x = fabs(pos[2]/cm - (scposz-glueshift) );
      }
    }
  }
  else if(mod==3||mod==4){//SMRD
    //Must be implemented based in the future
  }
  else if(mod==5){//BabyMIND
    //Must be implemented based in the future
  }
  else if(mod==6){//INGRID module

    if( view==topview ) x = fabs(scilen_pm/2. + pos[0]/cm);
    else if( view==sideview ) x = fabs(scilen_pm/2. + pos[1]/cm);

    i=(int)x/5;
    x=fabs(x-i*5-2.5);
    *edep *= exp(-1.*x/sciattleng_pm);
  }

  return;
}


void B2Response::ApplyFiberResponse(G4double* edep, G4double* time, G4int mod, G4int view, G4ThreeVector pos, G4int pln)
{
  G4double x = 0.;
  
  if(mod==0||mod==6){//Proton Module and INGRID
    if( view==topview ) x = fabs(scilen_pm/2. - pos[1]/cm);
    else if( view==sideview ) x = fabs(scilen_pm/2. + pos[0]/cm);
  }
  else if(mod==1||mod==2){//WAGASCI
    if( view==topview ) x = fabs(scilen_wgs/2. - pos[1]/cm) + fiberbundle;
    else if( view==sideview ) x = fabs(scilen_wgs/2. + pos[0]/cm) + fiberbundle;
  }  
  else if(mod==3||mod==4){//SMRD
    //Must be implemented based in the future
  }
  else if(mod==5){//BabyMIND
    //Must be implemented based in the future
  }
  
  // attenuation in fiber
  *edep *= exp(-1.*x/attleng);
  
  // delay in fiber
  *time += TransTimeInFiber*x;
  *time = gRandom->Gaus(*time,1.0);
  
  return;  
}


//
void B2Response::ApplyFiberResponseV(G4double* edep, G4double* time, G4ThreeVector pos, G4int pln)
{
  G4double x = 0.;

  /*
    if( pln==11 ) x = fabs(longlen/2. - (pos[1]/cm + 3.7) );
    if( pln==12 ) x = fabs(longlen/2. + (pos[1]/cm - 0.3) );
    if( pln==13 ) x = fabs(shortlen/2. - (pos[0]/cm + 0.9) );
    if( pln==14 ) x = fabs(longlen/2. + (pos[0]/cm - 5.9) );
  */ 
  
  // attenuation
  *edep *= exp(-1.*x/attleng);
  
  // delay in fiber
  *time += TransTimeInFiber*x;
  
  return;
}



//
void B2Response::ApplyMPPCResponse(G4double edep, G4double* pe, G4int mod)
{
  G4double npe;
  
  // Energy to p.e. conversion
  if(mod==0||mod==6){//Proton Module and INGRID
    npe = edep*(MeV2PE_pm);
  }
  else if(mod==1||mod==2){//WAGASCI
    npe = edep*(MeV2PE_wgs);
  }
  else{//Others
    //Must be tuned in the future.
    npe = edep*(MeV2PE_pm);
  }
  
  // PDE 
  //npe = edep * rPDE * ETC_CONST;

  // MPPC linearity
  if(mod==0||mod==6){//Proton Module and INGRID
    npe = MPPCPixel * (1. - exp( Eff_PDE_pm * npe / MPPCPixel ));
  }
  else if(mod==1||mod==2){//WAGASCI
    npe = MPPCPixel * (1. - exp( Eff_PDE_wgs * npe / MPPCPixel ));    
  }
  else{//Others
    //Must be tuned in the future.
    npe = MPPCPixel * (1. - exp( Eff_PDE_pm * npe / MPPCPixel ));
  }
  
  // Fake signal p.e. from cross-talk & after pulse
  if(mod==0||mod==6){//Proton Module and INGRID
    npe = npe / (1. - cross_after_rate_pm);
  }
  else if(mod==1||mod==2){//WAGASCI
    npe = npe / (1. - cross_after_rate_wgs);
  }
  else{//Others
    //Must be tuned in the future.
    npe = npe / (1. - cross_after_rate_pm);
  }

  // Poisson statistics & 1 pe resolution
  npe = CLHEP::RandPoisson::shoot(npe);
  if(mod==0||mod==6){//Proton Module and INGRID
    npe = gRandom->Gaus(npe,npe*PixelGainVari_pm);
  }
  else if(mod==1||mod==2){//WAGASCI
    npe = gRandom->Gaus(npe,npe*PixelGainVari_wgs);
  }
  else{//Others
    //Must be tuned in the future.
    npe = gRandom->Gaus(npe,npe*PixelGainVari_pm);
  }

  *pe = npe;
  return;
}


//
void B2Response::ApplyADCResponse(G4double *pe, G4double *lope, G4int* adc, G4int* loadc, G4int mod)
{
  G4double adc_tmp, loadc_tmp,Q,loQ;

  //Response of Trip-t ADC (Proton Module and INGRID)
  if(mod==0 || mod==6){
    //PE to ADC
    adc_tmp = Pedestal + (*pe)*Gain;
    loadc_tmp = Pedestal + (*pe)*LowGain*14.29/13.55;
    
    //Electronics noise
    adc_tmp = gRandom->Gaus(adc_tmp,ElecNoise);
    loadc_tmp = gRandom->Gaus(loadc_tmp,LowElecNoise);
    
    //ADC to Charge
    Q=(adc_tmp)/135.5;
    loQ=(loadc_tmp)/14.29;
    
    //Non linearlity of high gain ADC
    if(Q<0.65)*adc=(int)135.5*Q;
    else if(Q<3.2)*adc=(int)217*Q-53;
    else if(Q<4.2)*adc=(int)158.6*Q+133.9;
    else if(Q<14)*adc=(int)5.1*Q+778.6;
    else *adc=850;
    
    //Non linearlity of low gain ADC
    if(loQ<7)*loadc=(int)14.29*loQ;
    else if(loQ<27)*loadc=(int)26*loQ-82;
    else if(loQ<35.5)*loadc=(int)21.12*loQ+48.24;
    else if(loQ<178.4)*loadc=(int)0.7*loQ+775.1;
    else *loadc=900;
    
    //ADC to PE
    *pe = (float)((*adc) - Pedestal)/Gain;
    *lope = (float)((*loadc) - Pedestal)/LowGain;
  }
  else if(mod==1||mod==2||mod==3||mod==4){//Response of SPIROC ADC (WAGASCI, SMRD)
    //Must be implemented based in the future
  }
  else if(mod==5){//Response of CITIROC ADC (BabyMIND)
    //Must be implemented based in the future
  }

}

//
void B2Response::ApplyTDCResponse(G4double time, G4int* tdc)
{
  *tdc = 0;
}
