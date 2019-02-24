#include "B2VetoHit.hh"

#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4ThreeVector.hh"
#include <fstream>

#include "B2_Dimension.hh"

#define SideView 0  // Side View (Y-view) : 0
#define TopView 1  // Top View (X-view) : 1
#define PLNOFFSET 11
#define PLNOFFSETPM 18

#define THRE 0.3 // MeV

using namespace CLHEP;

// allocator
G4Allocator<B2VetoHit> B2VetoHitAllocator;

B2VetoHit::B2VetoHit(G4int dID0, G4int P0, G4int trkid, G4double e,G4double eq, G4ThreeVector pos, G4double t, G4int MODE) 
{
  detID = dID0;
  trackID = trkid;
  edep = e;
  edep_q = eq;
  Particle = P0;
  position = pos;
  time = t;
  mode = MODE;
  
  mod = dID0/(PLNMAX*CHMAX);
  
  if(mod==0){//Proton Module
    pln = (detID-mod*PLNMAX*CHMAX)/CHMAX + PLNOFFSETPM;
    ch = detID-mod*PLNMAX*CHMAX-(pln-PLNOFFSETPM)*CHMAX;
    if( pln==19 || pln==21 ) view = TopView;
    else if( pln==18 || pln==20 ) view = SideView;
  }
  else if(mod==6){//INGRID module
    pln = (detID-mod*PLNMAX*CHMAX)/CHMAX + PLNOFFSET;
    ch = detID-mod*PLNMAX*CHMAX-(pln-PLNOFFSET)*CHMAX;
    if( pln==11 || pln==12 ) view = TopView;
    else if( pln==13 || pln==14 ) view = SideView;
  }    
  else{
    cout<<"Module "<<mod<<" does not have veto planes."<<endl;
  }

  if( mod==0 ) {
    posinmod[0] = pos[0];
    posinmod[1] = pos[1];
    if     ( mode==1 ) posinmod[2] = pos[2]+1.36*m;
    else if( mode==2 ) posinmod[2] = pos[2]+0.78*m;
    else if( mode==3 ) posinmod[2] = pos[2]+0.2*m;
  }
}

B2VetoHit::~B2VetoHit() {}

const B2VetoHit& B2VetoHit::operator=(const B2VetoHit &right)
{
  detID = right.detID;
  edep       = right.edep;
  Particle = right.Particle; 
  for(int i=0;i<3;i++) position[i] = right.position[i];  
  eventID = right.eventID;

 return *this;
}

G4int B2VetoHit::operator==(const B2VetoHit &right) const
{
  return (this==&right) ? 1 : 0;
}  

G4int B2VetoHit::CompareID(const B2VetoHit right) 
{
  return (detID==right.detID) ? 1 : 0;
}


G4int B2VetoHit::CompareP(const B2VetoHit right) 
{
  return (Particle==right.Particle) ? 1 : 0;
}

G4int B2VetoHit::isFaster(const B2VetoHit right) 
{
  return (time<right.time) ? 1 : 0;
}

void B2VetoHit::Draw()
{
  double size = edep + 7.;
  if( edep>9 ) size = 16;

#if 1
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
    {
      G4ThreeVector pos; 
      for(int i=0;i<3;i++) pos[i] = position[i]*mm;
      G4Circle circle(pos);
      circle.SetScreenSize(size); 
      circle.SetFillStyle(G4Circle::filled);
      G4Colour colour(1.,0.,0.); // red
      G4VisAttributes attribs(colour);
      circle.SetVisAttributes(attribs);
      pVVisManager->Draw(circle);
    }
#endif
}

void B2VetoHit::Print()
{
  G4cout.precision(4);

  G4cout << " Mod:" << mod
         << " View(0:Y,1:X):" << view  
         << " Pln:" << pln  
         << " Ch:" << ch  
         << " Time:" << time
         << " Edep:" << G4BestUnit(edep,"Energy")
         << " p.e.:" << pe
	 << G4endl;
  G4cout << " PID:" << Particle
         << " Trk:" << trackID
         << " pos:" << position[0]/cm << "  "
         << position[1]/cm << "  " << position[2]/cm
	 << G4endl;

}


void B2VetoHit::Print_WeakEdep()
{
  std::ofstream ofs;
  ofs.open("veto_weakEnergy", std::ofstream::app);
  if (ofs.good()){
    ofs  << " veto-detID: " << detID  
	 << "  energy deposit: "<< G4BestUnit(edep,"Energy")
	 << "  particle: " << Particle
         << "  position (mm):{" << position[0] << ", "
         << position[1] << ", " << position[2]
         << G4endl;
  }
  
  ofs.close();
}

void B2VetoHit::Print_Hit()
{

  std::ofstream vetoHit_ofs;
  vetoHit_ofs.open("vetoHit", std::ofstream::app);
  if (vetoHit_ofs.good()){
    vetoHit_ofs  << eventID << "   " << detID << "   " << Particle << "   " << edep << "   ";
  }

  vetoHit_ofs.close();  
}
