#include "G4VPhysicalVolume.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4SDManager.hh"

#include "B2HLayerSD.hh"


B2HLayerSD::B2HLayerSD(const G4String& name, int MODE)
  : G4VSensitiveDetector(name)
{
  collectionName.insert("hlayerHitCollection");
  mode = MODE;
  ingresp = new B2Response();
}


B2HLayerSD::~B2HLayerSD()
{
    if(ingresp!=NULL) delete ingresp;
}


void B2HLayerSD::Initialize(G4HCofThisEvent* HCTE)
{
  hlayerHitCollection = 
    new B2HLayerHitsCollection(SensitiveDetectorName,collectionName[0]);
  TotalhlayerDep = 0.;
  
  static int HCID = -1;
  if(HCID<0) HCID = GetCollectionID(0); 
  HCTE->AddHitsCollection(HCID,hlayerHitCollection);
}


G4bool B2HLayerSD::ProcessHits(G4Step* aStep, 
				G4TouchableHistory* ROhist)
{
  // only when there is energy deposit in a sensitive detector,
  // create a new hit.
   
  G4Track* track = aStep->GetTrack();
  G4double edep = aStep->GetTotalEnergyDeposit();
  G4double length = aStep->GetStepLength();

  if(edep==0.) return false;
 
  TotalhlayerDep += edep;

  // volume information must be extracted from Touchable of "PreStepPoint"
  const G4VTouchable* Touchable = aStep->GetPreStepPoint()->GetTouchable();
  G4int detID = Touchable->GetVolume(0)->GetCopyNo();
  G4int trackID = track->GetTrackID();
  G4int PDG = track->GetDefinition()->GetPDGEncoding();
  G4ThreeVector hitPos = aStep->GetPreStepPoint()->GetPosition();
  G4double hittime = aStep->GetPreStepPoint()->GetGlobalTime();

  //apply quenching effect
  G4double edep_q = edep;
  ingresp->ApplyScintiResponse(&edep_q,track);
  
  //
  B2HLayerHit* aHit 
    = new B2HLayerHit(detID,PDG,trackID,edep,edep_q,hitPos,hittime,mode);
  
  B2HLayerHit* bHit;
  
  for(int k=0;k<hlayerHitCollection->entries();k++){
    bHit = (*hlayerHitCollection)[k];
    
    if(bHit->CompareID(*aHit)){
      bHit->AddEdep(edep,edep_q);
      
      if(bHit->isFaster(*aHit)) { 
	bHit->SetTime(aHit->GetTime()); 
      }
      if(bHit->LargerEdep(*aHit)) { 
	bHit->SetParticle(aHit->GetParticle()); 
      }
      return true;
    }
  }
  
  hlayerHitCollection->insert( aHit );
  
  return true; 
}

void B2HLayerSD::EndOfEvent(G4HCofThisEvent* HCTE)
{
  B2HLayerHit *cHit;

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

  // apply detector response
  for(G4int k=0;k<hlayerHitCollection->entries();k++) {

    cHit = (*hlayerHitCollection)[k];
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

    //convert edep -> p.e. &  p.e. statistic & cross-talk & after-pulse
    ingresp->ApplyMPPCResponse(edep_tmp,&pe,mod);

    //apply ADC responce
    ingresp->ApplyADCResponse(&pe,&lope,&adc,&loadc,mod);

    //fill variable to hitcollection
    cHit->SetPE(pe);
    cHit->SetLOPE(lope);
    cHit->SetDelayTime(time_tmp);
  }
}

void B2HLayerSD::DrawAll()
{
  for(G4int k=0; k < hlayerHitCollection->entries(); k++)
   (*hlayerHitCollection)[k]->Draw(); 
}

void B2HLayerSD::PrintAll()
{
   for(G4int k=0; k < hlayerHitCollection->entries(); k++)
     (*hlayerHitCollection)[k]->Print(); 
   //hlayerHitCollection-> PrintAllHits();
}
