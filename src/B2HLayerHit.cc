#include "B2HLayerHit.hh"

#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include <fstream>

#include "B2_Dimension.hh"
#include "Const.hh"

#define SideView 0 // resolution of vertical-direction

using namespace CLHEP;

// allocator
G4Allocator<B2HLayerHit> B2HLayerHitAllocator;

B2HLayerHit::B2HLayerHit(G4int dID0, G4int P0, G4int trkid, G4double e, G4double eq, G4ThreeVector pos, G4double t, G4int MODE) 
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
  pln = (dID0-mod*PLNMAX*CHMAX)/CHMAX;
  ch = dID0-mod*PLNMAX*CHMAX-pln*CHMAX;

  view = SideView;
 
  if(ch>CHMAX||ch<0)G4cout<<"error ch"<<ch<<endl;/////////////////
  if(pln>PLNMAX||pln<0)G4cout<<"error pln"<<pln<<endl;/////////////

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

B2HLayerHit::~B2HLayerHit() 
{
}

const B2HLayerHit& B2HLayerHit::operator=(const B2HLayerHit &right)
{
  detID = right.detID;
  edep       = right.edep;
  Particle = right.Particle; 
  
  for(int i=0;i<3;i++) position[i] = right.position[i];
  eventID = right.eventID;


  return *this;
}

G4int B2HLayerHit::operator==(const B2HLayerHit &right) const
{
  return (this==&right) ? 1 : 0;
}  

G4int B2HLayerHit::CompareID(const B2HLayerHit right) 
{
  return (detID==right.detID) ? 1 : 0;
}


G4int B2HLayerHit::CompareP(const B2HLayerHit right) 
{
  return (Particle==right.Particle) ? 1 : 0;
}

G4int B2HLayerHit::isFaster(const B2HLayerHit right)
{
  return (time>right.time) ? 1 : 0;
}
G4int B2HLayerHit::LargerEdep(const B2HLayerHit right)
{
  //return (Particle==11||right.Particle!=11) ? 1 : 0;
  if(Particle==11) return 1;
  else if(right.Particle==11) return 0;
  else return (edep<right.edep) ? 1 : 0;
}

void B2HLayerHit::Draw()
{
    double size = edep + 7.;
    if( edep>9 ) size = 16.;
  #if 1
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
    {
      G4ThreeVector pos; 
      for(int i=0;i<3;i++) pos[i] = position[i]*mm;
      G4Circle circle(pos);
      circle.SetScreenSize(size); 
      circle.SetFillStyle(G4Circle::filled);
      G4Colour colour(0.,1.,1.3); //cyan
      G4VisAttributes attribs(colour);
      circle.SetVisAttributes(attribs);
      pVVisManager->Draw(circle);
    }
  #endif
}

void B2HLayerHit::Print()
{
  G4cout.precision(4);
  
  G4cout << " Mod:" << mod 
    //<< " detID:" << detID
         << " Pln:" << pln 
	 << " Ch:" << ch 
	 << " Time:" << time
    //<< "  Edep:" << G4BestUnit(edep,"Energy")
	 << " Edep:" << edep
	 << " p.e.:" << pe
         << " PID:" << Particle
    //		 << G4endl;
	 << " Trk:" << trackID
         << " pos:{" << position[0]/cm << ", "
         << position[1]/cm << ", " << position[2]/cm
         << "}" << G4endl;
  
}


void B2HLayerHit::Print_Hit()
{

  std::ofstream yokoHit_ofs;
  yokoHit_ofs.open("yokoHit", std::ofstream::app);
  if (yokoHit_ofs.good()){
    yokoHit_ofs  << eventID << "   " << detID << "   " << Particle << "   " << edep << "   ";
  }
  
  yokoHit_ofs.close();
  
}
