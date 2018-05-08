#include "B2VLayerHit.hh"

#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include <fstream>

#include "B2_Dimension.hh"
#include "Const.hh"

#define TopView 1  /// x-view (resolution of x-direction)

// allocator
G4Allocator<B2VLayerHit> B2VLayerHitAllocator;

B2VLayerHit::B2VLayerHit(G4int dID0, G4int P0, G4int trkID, G4double e, G4double eq, G4ThreeVector pos, G4double t) 
{
  detID = dID0;
  trackID = trkID;
  edep = e;
  edep_q = eq;
  Particle = P0;
  position = pos;
  time = t;

  mod = dID0/(PLNMAX*CHMAX);
  pln = (dID0-mod*PLNMAX*CHMAX)/CHMAX;
  ch = dID0-mod*PLNMAX*CHMAX-pln*CHMAX;

  view = TopView;

  if(ch>CHMAX||ch<0)cout<<"error ch"<<ch<<endl;/////////////////
  if(pln>PLNMAX||pln<0)cout<<"error pln"<<pln<<endl;///////////////////
  
}

B2VLayerHit::B2VLayerHit(G4int dID0, G4double e,G4int P0) 
{
  detID = dID0;
  edep = e;
  Particle = P0;
}

B2VLayerHit::B2VLayerHit(G4int dID0, G4double e) 
{
  detID = dID0;
  edep = e;
 

}



B2VLayerHit::~B2VLayerHit() {}

B2VLayerHit::B2VLayerHit(const B2VLayerHit &right)
  : G4VHit()
{
  detID = right.detID;
  edep       = right.edep;
  Particle = right.Particle;

  for(int i=0;i<3;i++) position[i] = right.position[i];
  eventID = right.eventID;


}

const B2VLayerHit& B2VLayerHit::operator=(const B2VLayerHit &right)
{
  detID = right.detID;
  edep       = right.edep;
  Particle = right.Particle; 
  
  for(int i=0;i<3;i++) position[i] = right.position[i];
  eventID = right.eventID;

  return *this;
}

G4int B2VLayerHit::operator==(const B2VLayerHit &right) const
{
  return (this==&right) ? 1 : 0;
}  

G4int B2VLayerHit::CompareID(const B2VLayerHit right) 
{
  return (detID==right.detID) ? 1 : 0;
}

G4int B2VLayerHit::CompareP(const B2VLayerHit right) 
{
  return (Particle==right.Particle) ? 1 : 0;
}

G4int B2VLayerHit::isFaster(const B2VLayerHit right) 
{
  return (time>right.time) ? 1 : 0;
}
G4int B2VLayerHit::LargerEdep(const B2VLayerHit right)
{
  //return (Particle==11||right.Particle!=11) ? 1 : 0;
  if(Particle==11) return 1;
  else if(right.Particle==11) return 0;
  else return (edep<right.edep) ? 1 : 0;
}


void B2VLayerHit::Draw() // seems that when # of hits in modules is larger than 0 (by akira)
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
      G4Colour colour(1.,1.,0.); //means yellow
      G4VisAttributes attribs(colour);
      circle.SetVisAttributes(attribs);
      pVVisManager->Draw(circle);

    }
  #endif
}


void B2VLayerHit::Print()
{
    G4cout.precision(4);

  G4cout << " Mod:" << mod 
         << " Pln:" << pln
         << " Ch:" << ch
         << " Time:" << time
         << " Edep:"  << edep
         << " p.e.:"  << pe
         << " PID:" << Particle
		 << " Trk:" << trackID
         << " pos:{" << position[0]/cm << ", "
         << position[1]/cm << ", " << position[2]/cm << "} "
		 << G4endl;
}


void B2VLayerHit::Print_Hit()
{

  std::ofstream tateHit_ofs;
  tateHit_ofs.open("tateHit", std::ofstream::app);
  if (tateHit_ofs.good()){
    tateHit_ofs << eventID << "   " << detID << "   " << Particle << "   " << edep << "   ";
  }
  
  tateHit_ofs.close();
  
}


