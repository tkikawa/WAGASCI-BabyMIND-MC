#include "BeamInfoSummary.h"

//......................................................................

BeamInfoSummary::BeamInfoSummary()
{ 
    nurun            = -1;          
    spill_flag       = -1;
    trg_sec          = -1;
    spillnum         = -1;
    nd280spillnum    = -1;
    run_type         = -1;
    for(int i=0; i<3; i++){
      target_eff[i] = -1.e-5;
    }
    good_spill_flag  = false;
    for(int i=0; i<5; i++){
      for(int j=0; j<9; j++){
	beam_time[i][j]=-1;
      }
    }
    for(int i=0; i<5; i++){
      for(int j=0; j<9; j++){
	ct_np[i][j]=-1;
      }
    }
    for(int i=0; i<3; i++){
      for(int j=0; j<5; j++){
	hct[i][j]=-1.e5;
      }
    }
    for(int i=0; i<12; i++){
      mumon[i]=-1;
    }
    for(int i=0; i<13; i++){
      otr[i]=-1;
    }
    whorn = false;
    wohorn= false;
    horn1 = false;
    cutwotr= false;
    horn250= false;
}

//......................................................................

BeamInfoSummary::BeamInfoSummary(const BeamInfoSummary& info) 
{ 
    nurun            = info. nurun;
    spill_flag       = info. spill_flag;
     trg_sec          = info. trg_sec;
    spillnum         = info. spillnum;
    nd280spillnum    = info. nd280spillnum;
    run_type         = info. run_type;
    good_spill_flag  = info. good_spill_flag;
    for(int i=0; i<3; i++){
      target_eff[i]    = info. target_eff[i];
    }

    for(int i=0; i<5; i++){
      for(int j=0; j<9; j++){
	beam_time[i][j] = info.beam_time[i][j];
      }
    }
    for(int i=0; i<5; i++){
      for(int j=0; j<9; j++){
	ct_np[i][j]     = info.ct_np[i][j];
      }
    }
    for(int i=0; i<3; i++){
      for(int j=0; j<5; j++){
	hct[i][j]       = info.hct[i][j];
      }
    }
    for(int i=0; i<12; i++){
      mumon[i] = info. mumon[i];
    }
    for(int i=0; i<13; i++){
      otr[i]   = info. otr[i];
    }
    whorn = info.whorn;
    wohorn= info.wohorn;
    horn1 = info.horn1;
    cutwotr= info.cutwotr;
    horn250= info.horn250;

}

//......................................................................

BeamInfoSummary::~BeamInfoSummary() 
{
}

//......................................................................


void BeamInfoSummary::Clear(Option_t* option)
{

}

//......................................................................

void BeamInfoSummary::Print()
{

}


//......................................................................

ClassImp(BeamInfoSummary)

////////////////////////////////////////////////////////////////////////
