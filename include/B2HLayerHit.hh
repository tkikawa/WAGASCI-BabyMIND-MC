#ifndef B2HLayerHit_h
#define B2HLayerHit_h 1
 
#include "G4ThreeVector.hh"
#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

class B2HLayerHit : public G4VHit 
{
public:
  B2HLayerHit(G4int, G4int, G4int, G4double, G4double, G4ThreeVector, G4double, G4int);
  ~B2HLayerHit();

  const B2HLayerHit& operator=(const B2HLayerHit &right);
  G4int operator==(const B2HLayerHit &right) const;
  G4int CompareID(const B2HLayerHit right) ;
  G4int CompareP(const B2HLayerHit right);
  G4int isFaster(const B2HLayerHit right);
  G4int LargerEdep(const B2HLayerHit right);

  // new/delete operators
  inline void *operator new(size_t);
  inline void operator delete(void  *aHit);

  void Draw();
  void Print();
  void Print_Hit();

private:
  G4int detID;
  G4int trackID;
  G4int mod;
  G4int view;
  G4int pln;
  G4int ch;
  G4double edep;
  G4double edep_q;
  G4double pe;
  G4double lope;
  G4int Particle;
  G4ThreeVector position;
  G4ThreeVector posinmod;
  G4int eventID;
  G4double time;
  G4double delay_time;

  G4int mode;
  
  //set/get functions 
public:
  inline void AddEdep(G4double e, G4double eq) { edep += e; edep_q += eq; }
  inline void SetEventID(G4int event_ID) { eventID = event_ID;}
  inline void SetPE(G4double p) { pe = p; }
  inline void SetLOPE(G4double p) { lope = p; }
  inline void SetTime(G4double t) { time = t; }
  inline void SetParticle(G4int p) { Particle = p; }
  inline void SetDelayTime(G4double dt) { delay_time = dt; }
  
  //
  inline G4int GetDetID() { return detID; }// ID of scintillator
  inline G4int GetTrackID() { return trackID; }
  inline G4int GetParticle() { return Particle; }
  inline G4double GetEdep() { return edep; }
  inline G4double GetEdepQ() { return edep_q; }
  inline G4ThreeVector GetPosition() { return position; }
  inline G4ThreeVector GetPosInMod() { return posinmod; }
  inline G4double GetTime() { return time; }
  inline G4double GetDelayTime() { return delay_time; }
  inline G4int GetEventID() { return eventID; }
  inline G4int GetMod() { return mod; }
  inline G4int GetView() { return view; }
  inline G4int GetPln() { return pln; }
  inline G4int GetCh() { return ch; }
  inline G4double GetPE() { return pe; }
  inline G4double GetLOPE() { return lope; }
  
};


// ====================================================================
// inline functions
// ====================================================================

// externally instanciated.
typedef G4THitsCollection<B2HLayerHit> B2HLayerHitsCollection;

extern G4Allocator<B2HLayerHit> B2HLayerHitAllocator; 

inline void* B2HLayerHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) B2HLayerHitAllocator.MallocSingle();
  return aHit;
}

inline void B2HLayerHit::operator delete(void *aHit)
{
  B2HLayerHitAllocator.FreeSingle((B2HLayerHit*) aHit);
}

#endif
