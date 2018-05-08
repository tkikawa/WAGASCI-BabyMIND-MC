#include "B2HitSummary.h"

//......................................................................

B2HitSummary::B2HitSummary():
  nsimhits(0)
{ 

    view     = -1;
    mod      = -1;
    pln      = -1;
    ch       = -1;
    adc      = -1;
    loadc    = -1;
    cyc      = -1;
    pe       = -1.e5;
    lope     = -1.e5;
    pecorr   = -1.e5;
    vise     = -1.e5;
    visecorr = -1.e5;
    tdc      = -1.e5;
    time     = -1.e5;
    tnearhit = -1.e5;
    timecorr = -1.e5;
    xy       = -1.e5;
    z        = -1.e5;
    addbasicrecon  = 0;
    dummy          = 0;
    gocosmic = false;
    hitcosmic= false;
    isohit   = false;
}

//......................................................................

B2HitSummary::B2HitSummary(const B2HitSummary& hit) :
  nsimhits(0)
{ 

    mod      = hit.mod;
    view     = hit.view;
    pln      = hit.pln;
    ch       = hit.ch;
    cyc      = hit.cyc;
    adc      = hit.adc;
    loadc    = hit.loadc;
    pe       = hit.pe;
    lope     = hit.lope;
    pecorr   = hit.pecorr;
    vise     = hit.vise;
    visecorr = hit.visecorr;
    tdc      = hit.tdc;
    time     = hit.time;
    tnearhit = hit.tnearhit;
    timecorr = hit.timecorr;
    xy       = hit.xy;
    z        = hit.z;
    addbasicrecon  = hit.addbasicrecon;
    dummy          = hit.dummy;
    gocosmic       = hit.gocosmic;
    hitcosmic      = hit.hitcosmic;
    isohit         = hit.isohit;

    for (int i=0; i<B2HIT_MAXSIMHITS; ++i) {
        fB2SimHit[i] = TRef(NULL);
    }

    for (int i=0; i < hit.nsimhits; ++i) 
        AddB2SimHit(hit.GetB2SimHit(i));
}

//......................................................................

B2HitSummary::~B2HitSummary() 
{
}

//......................................................................


void B2HitSummary::Clear(Option_t* option)
{
    for (int i=0; i<B2HIT_MAXSIMHITS; ++i)
        fB2SimHit[i] = TRef(NULL);
    nsimhits = 0;
}

//......................................................................

void B2HitSummary::Print()
{
  std::cout << "-------------------" <<std::endl
	    << "Module# = " << mod   <<std::endl
	    << "View    = " << view  <<std::endl
	    << "Plane # = " << pln   <<std::endl
	    << "Ch. #   = " << ch    <<std::endl
	    << "ADC     = " << adc   <<std::endl
	    << "TDC     = " << tdc   <<std::endl
	    << "xy      = " << xy    <<std::endl
	    << "z       = " << z     <<std::endl
	    << std::endl;
}


//......................................................................

void B2HitSummary::AddB2SimHit(B2SimHitSummary* sbhitsum) 
{
    if (nsimhits < B2HIT_MAXSIMHITS) {
        fB2SimHit[nsimhits] = TRef((B2SimHitSummary*) sbhitsum);
        ++nsimhits;
    }
}
//......................................................................


B2SimHitSummary* B2HitSummary::GetB2SimHit(int i) const
{ 
    return (B2SimHitSummary*)fB2SimHit[i].GetObject();
}

//......................................................................


ClassImp(B2HitSummary)

////////////////////////////////////////////////////////////////////////
