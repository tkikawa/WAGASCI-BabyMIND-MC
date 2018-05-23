#ifndef B2VLayerSD_H
#define B2VLayerSD_H
 
#include "G4VSensitiveDetector.hh"

#include "B2VLayerHit.hh"
#include "B2Response.hh"

class G4HCofThisEvent;
class G4Step;
//class G4TouchableHistory;

class B2VLayerSD : public G4VSensitiveDetector {
  G4THitsCollection<B2VLayerHit>* vlayerHitCollection;
  B2Response *ingresp;
  G4double TotalvlayerDep;
  int mode;
 
public:
  B2VLayerSD(const G4String& name, int MODE);
  virtual ~B2VLayerSD();

  // virtual methods
  virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);
  virtual void Initialize(G4HCofThisEvent* HCTE);
  virtual void EndOfEvent(G4HCofThisEvent* HCTE);

  virtual void DrawAll();
  virtual void PrintAll(); 

   
  inline G4int GetNhit()
  { return vlayerHitCollection->entries(); }
  inline G4double GetTotalDep()
  { return TotalvlayerDep; }

};

#endif
