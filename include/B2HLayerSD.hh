#ifndef B2HLayerSD_H
#define B2HLayerSD_H
 
#include "G4VSensitiveDetector.hh"

#include "B2HLayerHit.hh"
#include "B2Response.hh"

class G4HCofThisEvent;
class G4Step;
//class G4TouchableHistory;

class B2HLayerSD : public G4VSensitiveDetector {
  G4THitsCollection<B2HLayerHit>* hlayerHitCollection;
  B2Response *ingresp;
  G4double TotalhlayerDep;
 
public:
  B2HLayerSD(const G4String& name);
  virtual ~B2HLayerSD();

  virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);
  virtual void Initialize(G4HCofThisEvent* HCTE);
  virtual void EndOfEvent(G4HCofThisEvent* HCTE);

  virtual void DrawAll();
  virtual void PrintAll(); 

  
  inline G4int GetNhit()
  { return hlayerHitCollection->entries(); }
  inline G4double GetTotalDep()
  { return TotalhlayerDep; }
  

};

#endif
