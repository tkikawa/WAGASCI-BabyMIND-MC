#ifndef B2SIMHITSUMMARY_H
#define B2SIMHITSUMMARY_H
#include <iostream>
#include "TObject.h"


//......................................................................

class B2SimHitSummary : public TObject
{
public:
    B2SimHitSummary();
    virtual ~B2SimHitSummary();
    
    void Clear   (Option_t* option="");
    void Print();

public:
    
    float edeposit;             // Energy deposit (MeV)
    int   trackid;              // ID of GEANT4 track responsible for hit
    int   pdg;                  // PDG particle ID of GEANT4 track responsible for hit

private:

    ClassDef(B2SimHitSummary, 2) // B2 Hit Sim Summary
};

#endif // B2SIMHITSUMMARY_H
////////////////////////////////////////////////////////////////////////
