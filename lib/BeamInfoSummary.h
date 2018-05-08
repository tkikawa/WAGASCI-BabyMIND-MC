#ifndef BEAMINFOSUMMARY_H
#define BEAMINFOSUMMARY_H
#include <iostream>
#include "TObject.h"
#include <stdlib.h>
#include <stdio.h>
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
#include "TRefArray.h"
#include "TRef.h"


//......................................................................

class BeamInfoSummary : public TObject
{
public:
    BeamInfoSummary();
    BeamInfoSummary(const BeamInfoSummary& info);
    virtual ~BeamInfoSummary();
    
    void Clear   (Option_t* option="");
    void Print();

public:

    Int_t                 nurun;          
    Int_t            spill_flag;
    Int_t       good_spill_flag;
    Int_t               trg_sec;
    Int_t              spillnum;
    Int_t         nd280spillnum;
    Int_t              run_type;
    Double_t      target_eff[3];
    Double_t          beam_time[5][9];
    Double_t              ct_np[5][9];
    Double_t              mumon[12];
    Double_t                hct[3][5];
    Double_t                otr[13];

    bool           wohorn;
    bool           horn1;
    bool           whorn;
    bool           cutwotr;
    bool           horn250;


private:

    ClassDef(BeamInfoSummary, 5) // Beam Info Summary
};

#endif // BEAMINFOSUMMARY_H
////////////////////////////////////////////////////////////////////////
