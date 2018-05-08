#include "G4VPhysicalVolume.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4SDManager.hh"

#include "B2VLayerSD.hh"


B2VLayerSD::B2VLayerSD(const G4String& name)
  : G4VSensitiveDetector(name)
{
  collectionName.insert("vlayerHitCollection");
  ingresp = new B2Response();
}


B2VLayerSD::~B2VLayerSD()
{
    if(ingresp!=NULL) delete ingresp;
}


void B2VLayerSD::Initialize(G4HCofThisEvent* HCTE)
{
    //G4cout << "Initialization of vlayerSD" << G4endl;

  vlayerHitCollection = 
    new B2VLayerHitsCollection(SensitiveDetectorName,collectionName[0]);
  TotalvlayerDep = 0.;
  
  static int HCID = -1;
  if(HCID<0) HCID = GetCollectionID(0); 
  HCTE->AddHitsCollection(HCID,vlayerHitCollection);
}



G4bool B2VLayerSD::ProcessHits(G4Step* aStep, 
				G4TouchableHistory* ROhist)
{
  // only when there is energy deposit in a sensitive detector,
  // create a new hit.
   
  G4Track* track = aStep->GetTrack();
  G4double edep = aStep->GetTotalEnergyDeposit();
  G4double length = aStep->GetStepLength();

  //  G4cout << "length : " << length << G4endl;

  if(edep==0.) return false;
 

  TotalvlayerDep += edep;
  // volume information must be extracted from Touchable of "PreStepPoint"
  // e.g.
  const G4VTouchable* Touchable = aStep->GetPreStepPoint()->GetTouchable();
  G4int detID = Touchable->GetVolume(0)->GetCopyNo();
    
  G4int trackID = track->GetTrackID();

  G4int PDG = track->GetDefinition()->GetPDGEncoding();
  G4ThreeVector hitPos = aStep->GetPreStepPoint()->GetPosition();
  G4double hittime = aStep->GetPreStepPoint()->GetGlobalTime();
  
  //
  G4double edep_q = edep;
  ingresp->ApplyScintiResponse(&edep_q,track);

  //
  B2VLayerHit* aHit 
    = new B2VLayerHit(detID,PDG,trackID,edep,edep_q,hitPos,hittime);
    
  B2VLayerHit* bHit;
 
  for(int k=0;k<vlayerHitCollection->entries();k++){
    bHit = (*vlayerHitCollection)[k];

    if(bHit->CompareID(*aHit)){
      bHit->AddEdep(edep,edep_q);

			if(bHit->isFaster(*aHit)) { 
			  bHit->SetTime( aHit->GetTime() );
			}
			if(bHit->LargerEdep(*aHit)) { 
			  bHit->SetParticle(aHit->GetParticle()); 
			}
      return true;
    }
  }

  vlayerHitCollection->insert( aHit );

  return true;
}

void B2VLayerSD::EndOfEvent(G4HCofThisEvent* HCTE)
{
	B2VLayerHit *cHit;

	G4double edep_tmp;
	G4double time_tmp;
	G4ThreeVector posinmod;
	G4int mod;
	G4int view;
	G4int adc;
	G4int loadc;
	G4double pe;
	G4double lope;
        G4int pln;
	G4int ch;

	//
	// apply ingrid response
	for(G4int k=0;k<vlayerHitCollection->entries();k++) {
		cHit = (*vlayerHitCollection)[k];
		edep_tmp = cHit->GetEdepQ();
		time_tmp = cHit->GetTime();
		posinmod = cHit->GetPosInMod();
		mod = cHit->GetMod();
		view = cHit->GetView();
                pln = cHit->GetPln();
		ch = cHit->GetCh();

		//apply light collection
		ingresp->ApplyLightCollection(&edep_tmp,mod,view,posinmod,pln,ch);

		//apply fiber attenuation
		ingresp->ApplyFiberResponse(&edep_tmp,&time_tmp,mod,view,posinmod,pln);

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

void B2VLayerSD::DrawAll()
{
  for(G4int k=0; k < vlayerHitCollection->entries(); k++)
   (*vlayerHitCollection)[k]->Draw(); 
}

void B2VLayerSD::PrintAll()
{
   for(G4int k=0; k < vlayerHitCollection->entries(); k++)
     (*vlayerHitCollection)[k]->Print(); 
   //vlayerHitCollection-> PrintAllHits();
}

