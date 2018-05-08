#ifndef B2EventAction_h
#define B2EventAction_h 1

#include "G4UserEventAction.hh"
#include "B2RunAction.hh"
#include "B2_Dimension.hh"
#include <vector>

class G4Event;

//
class B2EventAction : public G4UserEventAction
{
public:

  B2EventAction(B2RunAction*); 
  ~B2EventAction();

public:
  void BeginOfEventAction(const G4Event* anEvent);
  void EndOfEventAction(const G4Event* anEvent);

  inline void SetTrackID(int i) { TrackID = i; }
  int GetTrackID() { return TrackID; }
  inline void SetWriteFlag(int j) { Flag = j; }

private:
  B2RunAction* runaction;
  int TrackID;
  int Flag;
  B2_Dimension *fdim;

    B2SimHitSummary *ingsimhit;
    B2HitSummary *inghit;

};

#endif
