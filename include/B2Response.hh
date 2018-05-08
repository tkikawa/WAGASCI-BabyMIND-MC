#ifndef B2_RESPONSE
#define B2_RESPONSE

#include "G4ThreeVector.hh"
#include "G4EmCalculator.hh"
#include "G4Track.hh"
#include "B2_Dimension.hh"

class B2Response {
public:
  B2Response();
  ~B2Response();

  void ApplyScintiResponse(G4double* edep, G4Track* aTrack);
  //void ApplyLightCollection(G4double* edep, G4int mod, G4int view, G4ThreeVector pos);//added
  void ApplyLightCollection(G4double* edep, G4int mod, G4int view, G4ThreeVector pos, G4int pln, G4int ch);
  //void ApplyFiberResponse(G4double* edep, G4double* time, G4int view, G4ThreeVector pos);
  //void ApplyFiberResponse(G4double* edep,G4int mod, G4double* time, G4int view, G4ThreeVector pos);//added B2
  void ApplyFiberResponse(G4double* edep, G4double* time, G4int mod, G4int view, G4ThreeVector pos, G4int pln);//added B2 2014/1/27
  void ApplyFiberResponseV(G4double* edep, G4double* time, G4ThreeVector pos, G4int pln);
  void ApplyMPPCResponse(G4double edep, G4double* pe, G4int mod);
  void ApplyADCResponse(G4double* pe, G4double* lope, G4int* adc, G4int* loadc, G4int mod);
  void ApplyTDCResponse(G4double time, G4int* tdc);
private:
  G4EmCalculator emcal;
  B2_Dimension *fdim;
};

#endif
