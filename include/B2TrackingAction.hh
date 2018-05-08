#ifndef B2_TRACKING_ACTION_H
#define B2_TRACKING_ACTION_H 1

#include "G4UserTrackingAction.hh"
#include "B2RunAction.hh"
#include "B2EventAction.hh"

#include "B2SimParticleSummary.h"

class B2TrackingAction : public G4UserTrackingAction {

public:
  B2TrackingAction(B2RunAction*,B2EventAction*);
  virtual ~B2TrackingAction();
   
  virtual void PreUserTrackingAction(const G4Track*);
  virtual void PostUserTrackingAction(const G4Track*);

private:
  B2RunAction* runaction;
  B2EventAction* eventaction;
  B2SimParticleSummary *simpart;
  int Tracking_Flag;

  float posi[3];
  float momi[3];
  double initE;
};

#endif
