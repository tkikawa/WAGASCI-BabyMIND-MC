#ifndef NEUTINFOSUMMARY_H
#define NEUTINFOSUMMARY_H
#include <iostream>
#include <vector>
using namespace std;
#include "TObject.h"

#define MAX_Numnu 30

//......................................................................

class NeutInfoSummary : public TObject
{
public:
    NeutInfoSummary();
    NeutInfoSummary(const NeutInfoSummary& neut);
    virtual ~NeutInfoSummary();
    
    void Clear   (Option_t* option="");
    void Print(Option_t* option) const;

public:
    // Particle Information of primary state 
    int             Mode;             // Neutrino interaction mode
    int             Numnu;            // # of generated particle
    int             Ipnu[MAX_Numnu];        // particle id
    float           Abspnu[MAX_Numnu];      // absolute momentum [MeV]
    float           Pnu[MAX_Numnu][3];        // direction

private:

    ClassDef(NeutInfoSummary, 4) // Neut Info Summary
};

#endif // NEUTINFOSUMMARY_H
////////////////////////////////////////////////////////////////////////
