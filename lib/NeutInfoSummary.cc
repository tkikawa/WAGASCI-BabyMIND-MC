#include "NeutInfoSummary.h"

//......................................................................

NeutInfoSummary::NeutInfoSummary()
{ 
  Mode     = 0;
  Numnu    = 0;
  for(int i=0;i<MAX_Numnu;i++) {
    Ipnu[i] = 0;
    Abspnu[i] = 0.;
    for(int j=0;j<3;j++) Pnu[i][j] = 0.;
  }
}

//......................................................................

NeutInfoSummary::NeutInfoSummary(const NeutInfoSummary& neut) 
{ 
  Mode = neut.Mode;
  Numnu = neut.Numnu;
  for(int i=0;i<(neut.Numnu);i++) {
    Ipnu[i] = neut.Ipnu[i];
    Abspnu[i] = neut.Abspnu[i];
    for(int j=0;j<3;j++) Pnu[i][j] = neut.Pnu[i][j];
  }
}

//......................................................................

NeutInfoSummary::~NeutInfoSummary() 
{
}

//......................................................................


void NeutInfoSummary::Clear(Option_t* option)
{
  Mode     = 0;
  Numnu    = 0;
  for(int i=0;i<MAX_Numnu;i++) {
    Ipnu[i] = 0;
    Abspnu[i] = 0.;
    for(int j=0;j<3;j++) Pnu[i][j] = 0.;
  }
}

//......................................................................

void NeutInfoSummary::Print(Option_t* option) const
{

}


//......................................................................

ClassImp(NeutInfoSummary)

////////////////////////////////////////////////////////////////////////
