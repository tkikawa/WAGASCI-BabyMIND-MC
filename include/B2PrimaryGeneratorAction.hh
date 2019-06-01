#ifndef B2PrimaryGeneratorAction_h
#define B2PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleDefinition.hh"

#include "B2RunAction.hh"
#include "B2EventAction.hh"

#include "Neut.hh"

class G4ParticleGun;
class G4Event;

class B2PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  B2PrimaryGeneratorAction(B2RunAction*, B2EventAction*, int, int, int);
  ~B2PrimaryGeneratorAction();

public:
  void GeneratePrimaries(G4Event* anEvent);

private:
  G4ParticleGun* particleGun;
  G4ParticleTable* particleTable;

  int module_mode;
  int neutrino_flavor;
  int vtxmod;
  double offsetz;

  B2RunAction* runaction;
  B2EventAction* eventaction;
};

#endif
