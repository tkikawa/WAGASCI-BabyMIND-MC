#include "B2ReconSummary.h"


//......................................................................

B2ReconSummary::B2ReconSummary():
  nhits(0), ntracks(0)
{ 
  view        = -1;
  clstime     = -1.e5;
  //CTtimecorr  = -1.e5;
  clstimecorr = -1.e5;
  //exptime     = -1.e5;
  //nhitclster  = -1;
  nactpln     = -1;
  //actinarow   = -1;
  layerpe     = -1.e5;
  hitmod      = -1;
  hitcyc      = -1;
  spill_flag  =  false;
  bunch_flag  =  false;
  //upstreamtpl = -1;
  //upstreamVETO=  false;
  //upstreamedge=  false;
  //newfid      =  false;
  //newfidcosmic=  false;
  //vinternal   =  false;
  ontime      =  false;
  vetodist =  -1e-5;
  vetowtracking   =  false;
  edgewtracking   =  false;
  matchtrk    =  false;
  hastrk      =  false;
  //trg_sec     = -1e-5;
  //vpe         = -1e-5;
  angle       = -1e-5;
  //thetax      = -1e-5;
  //thetay      = -1e-5;
  //vertexz     = -1;
  startz     = -1e-5;
  endz     = -1e-5;
  //vertexxz    = -1;
  //vertexyz    = -1;
  vertexxy     = -1;
  //trgbit      = -1;
  //inarowbit   = -1;
  //horn250     =  false;

  slope     = -1e-5;
  intcpt     = -1e-5;

  //vertexx.clear();
  //vertexy.clear();

  //startxpln = -1;
  //startypln = -1;
  startpln = -1;
  //startxch  = -1;
  //startych  = -1;
  startxy  = -1e-5;
  //endxpln   = -1;
  //endypln   = -1;
  endpln   = -1;
  //endxch    = -1;
  //endych    = -1;
  endxy    = -1e-5;
  modfc     = false;
  //penIron   = -1;
  //muE       = -1e-5;
  //nuErec    = -1e-5;

  //nhitx     = -1;
  //nhity     = -1;
  //ntrackhitx= -1;
  //ntrackhity= -1;

  //retracktest= false;
  //retrackx   = false;
  //retracky   = false;
  //totpe =  -1e-5;
  //isohit =  -1;
  //memset(pdg,-1,sizeof(pdg));

}
//......................................................................
B2ReconSummary::B2ReconSummary(const B2ReconSummary& basicsum) :
  nhits(0), ntracks(0)
{ 
  view        = basicsum. view;
  clstime     = basicsum. clstime ;
  //CTtimecorr  = basicsum. CTtimecorr;
  clstimecorr = basicsum. clstimecorr;
  //exptime     = basicsum. exptime;
  //nhitclster  = basicsum. nhitclster;
  nactpln     = basicsum. nactpln;
  //actinarow   = basicsum. actinarow;
  layerpe     = basicsum. layerpe;
  //upstreamVETO= basicsum. upstreamVETO;
  //upstreamedge= basicsum. upstreamedge;
  //vinternal   = basicsum. vinternal;
  hitmod      = basicsum. hitmod;
  hitcyc      = basicsum. hitcyc;
  spill_flag  = basicsum. spill_flag;
  bunch_flag  = basicsum. bunch_flag;
  //upstreamtpl = basicsum. upstreamtpl;
  ontime      = basicsum. ontime;
  //vpe         = basicsum. vpe;
  //trg_sec     = basicsum. trg_sec ;
  //trgbit      = basicsum. trgbit ;
  vetodist       = basicsum. vetodist ;
  vetowtracking  = basicsum. vetowtracking ;
  edgewtracking  = basicsum. edgewtracking ;
  matchtrk       = basicsum. matchtrk ;
  hastrk         = basicsum. hastrk ;
  //inarowbit      = basicsum. inarowbit ;
  ntracks = 0;
  nhits = 0;
  angle       = basicsum. angle;
  //thetax      = basicsum. thetax;
  //thetay      = basicsum. thetay;
  //vertexz     = basicsum. vertexz;
  startz     = basicsum. startz;
  endz     = basicsum. endz;
  //vertexxz    = basicsum. vertexxz;
  //vertexyz    = basicsum. vertexyz;
  vertexxy    = basicsum. vertexxy;
  //vertexx     = basicsum. vertexx;
  //vertexy     = basicsum. vertexy;
  //horn250     = basicsum. horn250;

  slope       = basicsum. slope;
  intcpt      = basicsum. intcpt;

  //startxpln   = basicsum. startxpln;
  //startypln   = basicsum. startypln;
  startpln    = basicsum. startpln;
  //startxch    = basicsum. startxch;
  //startych    = basicsum. startych;
  startxy     = basicsum. startxy;
  //endxpln     = basicsum. endxpln;
  //endypln     = basicsum. endypln;
  endpln      = basicsum. endpln;
  //endxch      = basicsum. endxch;
  //endych      = basicsum. endych;
  endxy       = basicsum. endxy;
  modfc       = basicsum. modfc;
  //penIron     = basicsum. penIron;
  //muE         = basicsum. muE;
  //nuErec      = basicsum. nuErec;

  //nhitx       = basicsum. nhitx;
  //nhity       = basicsum. nhity;
  //ntrackhitx  = basicsum. ntrackhitx;
  //ntrackhity  = basicsum. ntrackhity;
  
  //retracktest = basicsum. retracktest;
  //retrackx    = basicsum. retrackx;
  //retracky    = basicsum. retracky;
  //totpe       = basicsum. totpe;
  //isohit       = basicsum. isohit;
  /*
  for (int i=0; i<4; ++i) {
    pdg[i]       = basicsum. pdg[i];
  }
  */
  for (int i=0; i<B2HIT_MAXHITS; ++i) {
    fB2Hit[i] = TRef(NULL);
  }
  for (int i=0; i < basicsum.nhits; ++i) 
    AddB2Hit(basicsum.GetB2Hit(i));

  for (int i=0; i<B2RECON_MAXTRACKS; ++i) {
    fTrack[i] = TRef(NULL);
  }
  for (int i=0; i < basicsum.ntracks; ++i) 
    AddTrack(basicsum.GetTrack(i));

}

//......................................................................



B2ReconSummary::~B2ReconSummary() 
{

}

//......................................................................


void B2ReconSummary::Clear(Option_t* option)
{
  for (int i=0; i<B2HIT_MAXHITS; ++i)
    fB2Hit[i] = TRef(NULL);
  nhits = 0;

  for (int i=0; i<B2RECON_MAXTRACKS; ++i)
    fTrack[i] = TRef(NULL);
  ntracks = 0;

}

//......................................................................

void B2ReconSummary::Print()
{

}
//......................................................................
void B2ReconSummary::AddB2Hit(B2HitSummary* hit) 
{

    if (nhits < B2HIT_MAXHITS) {
        fB2Hit[nhits] = TRef((B2HitSummary*) hit);
        ++nhits;
    }

}
//......................................................................


B2HitSummary* B2ReconSummary::GetB2Hit(int i) const
{ 

  return (B2HitSummary*)fB2Hit[i].GetObject();
}

//......................................................................

void B2ReconSummary::AddTrack(B2TrackSummary* trk) 
{

    if (ntracks < B2RECON_MAXTRACKS) {
        fTrack[ntracks] = TRef((B2TrackSummary*) trk);
        ++ntracks;
    }

}
//......................................................................


B2TrackSummary* B2ReconSummary::GetTrack(int i) const
{ 

  return (B2TrackSummary*)fTrack[i].GetObject();
}

//......................................................................


ClassImp(B2ReconSummary)

////////////////////////////////////////////////////////////////////////
