#include "B2TrackingAction.hh"
#include  "B2DetectorConstruction.hh"
//#include "B2TrueTrackInfo.hh"

#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include  "G4VProcess.hh"
#include <iostream>
#include <vector>

//#define DEBUG 

/////////////////////////////////////////////////////////
B2TrackingAction::B2TrackingAction(B2RunAction *rac,B2EventAction *evt)
  :runaction(rac)
//////////////////////////////////////////////////////////
{
  eventaction = evt;

  simpart = new B2SimParticleSummary();

}

B2TrackingAction::~B2TrackingAction()
{
    if(simpart) delete simpart;
}

//////////////////////////////////////////////////////////////////////
void B2TrackingAction::PreUserTrackingAction(const G4Track* aTrack)
//////////////////////////////////////////////////////////////////////
{
  //G4cout << "\n------ in PreUserTrackingAction ------" << G4endl;
  int id = aTrack->GetParentID();
  
  for(int i=0;i<3;i++) {
    posi[i] = (aTrack->GetPosition()/cm)[i];
    momi[i] = (float)(aTrack->GetMomentum())[i];
    
    //
    simpart -> ipos[i] = (float)(aTrack->GetPosition()/cm)[i];
    simpart -> momentum[i] = (float)(aTrack->GetMomentum()/GeV)[i];
    simpart -> dir[i] = (float)(aTrack->GetMomentumDirection())[i];
  }
  
  initE = aTrack->GetTotalEnergy();
  
  simpart -> ipos[3] = aTrack->GetGlobalTime()*ns;
  simpart -> momentum[3] = aTrack->GetKineticEnergy()/GeV;
  simpart -> iposflag = 1;
  
  
  if( id==0 ) {
#ifdef DEBUG
    G4cout << "init  Pos " 
	   << (aTrack->GetPosition()/cm)[0] << " " 
	   << (aTrack->GetPosition()/cm)[1] << " "
	   << (aTrack->GetPosition()/cm)[2] << G4endl; 
    G4cout << "init  Momentum " 
	   << (aTrack->GetMomentum()/GeV)[0] << " " 
	   << (aTrack->GetMomentum()/GeV)[1] << " "
	   << (aTrack->GetMomentum()/GeV)[2] << " "
	   << (aTrack->GetKineticEnergy()/GeV) << G4endl; 
#endif
  }
  
}


///////////////////////////////////////////////////////////////////////
void B2TrackingAction::PostUserTrackingAction(const G4Track* aTrack)
///////////////////////////////////////////////////////////////////////
{
    //G4cout << "\n----- in PostUserTrackingAction -----" << G4endl;

//  G4double Eth = 0.5*MeV;
    int parentID = aTrack->GetParentID();
    int trackID = aTrack->GetTrackID();
    int pid = aTrack->GetDefinition()->GetPDGEncoding();  

		G4double trackL = aTrack->GetTrackLength();

    simpart -> parentid = parentID;
    simpart -> trackid = trackID;
    simpart -> pdg = pid;
    simpart -> length = trackL/cm;
    for(int i=0;i<3;i++) {
        simpart -> fpos[i] = (float)(aTrack->GetPosition()/cm)[i];
    }
    simpart -> fposflag = 1;

   if(parentID == 0 ){ // parent ID = 0 means first point of track

     float posf[3];
     float momf[3];
     
     
     G4cout.precision( 4 );
     
     for(int i=0;i<3;i++) {
       posf[i] = (aTrack-> GetPosition()/cm)[i];
       momf[i] = (float)(aTrack->GetMomentum()/MeV)[i];
       simpart -> fpos[i] = (float)(aTrack->GetPosition()/cm)[i];
     }
     
#ifdef DEBUG
     G4cout << "Track:" << trackID;
     //G4cout << "  Parent ID " << id;    
     G4cout << "  " << aTrack->GetDefinition()->GetParticleName()  
	    << "(" << aTrack->GetDefinition()->GetPDGEncoding() << ")\n";  
     G4cout << "Ei:" << initE
	    << " Momi:{" << momi[0] << "," << momi[1] << "," << momi[2] <<G4endl;
     G4cout << "Start:{"<< posi[0] << "," << posi[1] << "," << posi[2] << "}"
	    << " Stop:{"<< posf[0] << "," << posf[1] << "," << posf[2] << "}" <<G4endl;
     G4cout << "Length:" << trackL/cm << G4endl;
#endif
     
     // fill ingrid track class
     runaction -> GetEvtSum() -> AddSimParticle (simpart);
     
   } 
}
