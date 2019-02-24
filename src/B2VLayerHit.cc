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

using namespace CLHEP;

// allocator
G4Allocator<B2VLayerHit> B2VLayerHitAllocator;

B2VLayerHit::B2VLayerHit(G4int dID0, G4int P0, G4int trkID, G4double e, G4double eq, G4ThreeVector pos, G4double t, G4int MODE) 
{
  detID = dID0;
  trackID = trkID;
  edep = e;
  edep_q = eq;
  Particle = P0;
  position = pos;
  time = t;
  mode = MODE;

  mod = dID0/(PLNMAX*CHMAX);
  pln = (dID0-mod*PLNMAX*CHMAX)/CHMAX;
  ch = dID0-mod*PLNMAX*CHMAX-pln*CHMAX;

  view = TopView;

  if(ch>CHMAX||ch<0)cout<<"error ch"<<ch<<endl;/////////////////
  if(pln>PLNMAX||pln<0)cout<<"error pln"<<pln<<endl;///////////////////

  if( mod==0 ) {
    posinmod[0] = pos[0];
    posinmod[1] = pos[1];
    if     ( mode==1 ) posinmod[2] = pos[2]+1.36*m;
    else if( mode==2 ) posinmod[2] = pos[2]+0.78*m;
    else if( mode==3 ) posinmod[2] = pos[2]+0.2*m;
  }
  else if( mod==1 ) {
    posinmod[0] = pos[0];
    posinmod[1] = pos[1];
    if     ( mode==1 ) posinmod[2] = pos[2]+0.58*m;
    else if( mode==2 ) posinmod[2] = pos[2]+1.56*m;
    else if( mode==3 ) posinmod[2] = pos[2]+1.56*m;
  }
  else if( mod==2 ) {
    posinmod[0] = pos[0];
    posinmod[1] = pos[1];
    if     ( mode==1 ) posinmod[2] = pos[2];
    else if( mode==2 ) posinmod[2] = pos[2];
    else if( mode==3 ) posinmod[2] = pos[2]+0.98*m;
  }
  else if( mod==3 ) {
    posinmod[0] = pos[0]+1.08*m;
    posinmod[1] = pos[1];
    posinmod[2] = pos[2]+0.33*m;
  }
  else if( mod==4 ) {
    posinmod[0] = pos[0]-1.08*m;
    posinmod[1] = pos[1];
    posinmod[2] = pos[2]+0.33*m;
  }
  else if( mod==5 ) {
    posinmod[0] = pos[0]-0.45*m;
    posinmod[1] = pos[1];
    posinmod[2] = pos[2]-2.75*m;
  }
  
}

B2VLayerHit::~B2VLayerHit() {}

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


