#include "G4VPhysicalVolume.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4SDManager.hh"

#include "B2VetoSD.hh"

#define DEBUG 0
B2VetoSD::B2VetoSD(const G4String& name)
  : G4VSensitiveDetector(name)
{
  collectionName.insert("vetoHitCollection");
  ingresp = new B2Response();
}

B2VetoSD::~B2VetoSD()
{
    if(ingresp!=NULL) delete ingresp;
}

void B2VetoSD::Initialize(G4HCofThisEvent* HCTE)
{
  vetoHitCollection = 
    new B2VetoHitsCollection(SensitiveDetectorName,collectionName[0]);
  TotalvetoDep = 0.;
 
  static int HCID = -1;
  if(HCID<0) HCID = GetCollectionID(0); 
  HCTE->AddHitsCollection(HCID,vetoHitCollection);
}



G4bool B2VetoSD::ProcessHits(G4Step* aStep, 
				G4TouchableHistory* ROhist)
{
  // only when there is energy deposit in a sensitive detector,
  // create a new hit.
   
  G4Track* track = aStep->GetTrack();
  G4double edep = aStep->GetTotalEnergyDeposit();
  if(edep==0.) return false;
 
  TotalvetoDep += edep;
  // volume information must be extracted from Touchable of "PreStepPoint"
  // e.g.
  const G4VTouchable* Touchable = aStep->GetPreStepPoint()->GetTouchable();
  G4int detID = Touchable->GetVolume(0)->GetCopyNo();
  
    G4int trackID = track->GetTrackID();
#if DEBUG
    G4cout << "veto trackID : " << trackID << G4endl;
#endif

  G4int PDG = track->GetDefinition()->GetPDGEncoding();

  // True position of veto-hit
  //G4ThreeVector hitPos = aStep->GetPreStepPoint()->GetPosition();

  // Position of veto-hit is the center of veto-bar
  //G4ThreeVector hitPos = Touchable->GetTranslation(0);

  // We want true X and Y of veto-hit, Z of veto-hit is the Z-center of veto-bar... 
  G4ThreeVector hitPos_True = aStep->GetPreStepPoint()->GetPosition();
  G4ThreeVector hitPos_Center = Touchable->GetTranslation(0);
  G4ThreeVector hitPos;
  hitPos[0] = hitPos_True[0]; // X
  hitPos[1] = hitPos_True[1]; // Y
  hitPos[2] = hitPos_Center[2]; // Z

  G4double hittime = aStep->GetPreStepPoint()->GetGlobalTime();
  
  //
  G4double edep_q = edep;
  ingresp->ApplyScintiResponse(&edep_q,track);

  B2VetoHit* aHit 
    = new B2VetoHit(detID,PDG,trackID,edep,edep_q,hitPos,hittime);
    
	B2VetoHit* bHit;
 
	for(int k=0;k<vetoHitCollection->entries();k++){
    bHit = (*vetoHitCollection)[k];
    if(bHit->CompareID(*aHit)){
      bHit->AddEdep(edep,edep_q);
			if(bHit->isFaster(*aHit)) { 
				bHit->SetTime( aHit->GetTime() );
				bHit->SetParticle( aHit->GetParticle() );
			}
      return true;
    }
	}
 
	vetoHitCollection->insert( aHit );

  return true;

}

void B2VetoSD::EndOfEvent(G4HCofThisEvent* HCTE)
{
	B2VetoHit *cHit;

	G4double edep_tmp;
	G4double time_tmp;
	G4ThreeVector posinmod;
	G4int mod;
	G4int pln;
	G4int adc;
	G4int loadc;
	G4double pe;
	G4double lope;

	//
	// apply ingrid response
	for(G4int k=0;k<vetoHitCollection->entries();k++) {
		cHit = (*vetoHitCollection)[k];

		edep_tmp = cHit->GetEdepQ();
		time_tmp = cHit->GetTime();
		posinmod = cHit->GetPosInMod();
		mod = cHit->GetMod();
		pln = cHit->GetPln();

		//apply fiber attenuation
		ingresp->ApplyFiberResponseV(&edep_tmp,&time_tmp,posinmod,pln);

		//convert edep -> p.e. & cross & after pulse
		ingresp->ApplyMPPCResponse(edep_tmp,&pe,mod);

		//apply ADC responce
		ingresp->ApplyADCResponse(&pe,&lope,&adc,&loadc,mod);

		//fill variable to hitcollection
		cHit->SetPE(pe);
		cHit->SetLOPE(lope);
		cHit->SetDelayTime(time_tmp);

	}

}

void B2VetoSD::DrawAll()
{
  for(G4int k=0; k < vetoHitCollection->entries(); k++)
   (*vetoHitCollection)[k]->Draw(); 
}

void B2VetoSD::PrintAll()
{
   for(G4int k=0; k < vetoHitCollection->entries(); k++)
     (*vetoHitCollection)[k]->Print(); 
}

