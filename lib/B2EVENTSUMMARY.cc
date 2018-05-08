#include "./B2EVENTSUMMARY.h"

#include <vector>
#include <algorithm>
using namespace std;

//......................................................................

B2EventSummary::B2EventSummary():
  fB2SimHit(0),fB2Hit(0), 
  fSimParticle(0), fSimVertex(0),
  fBeamSummary(0), fBasicRecon(0), fB2Recon(0), fB2Ana(0),
  fB2Track(0),
  fNeutInfo(0)//,
{ 
    this->NewTClonesArrays();
    this->Clear("C");



}

//......................................................................

B2EventSummary::B2EventSummary(const B2EventSummary& evt) 
{ 

    this->NewTClonesArrays();
    this->Clear("C");

    run      = evt.run;
    event    = evt.event;
    runmode  = evt.runmode;
    trgid    = evt.trgid;
    version  = evt.version;
    date     = evt.date;
    time     = evt.time;
    trgtime  = evt.trgtime;
  
    for(int i=0; i<23;i++)bunch_flag[i] = evt.bunch_flag[i];

    nb2simhits = 0;
    nb2hits    = 0;
    nbasicrecons   = 0;
    nb2recons   = 0;
    nb2anas   = 0;
    nbeamsummarys  = 0;
    nsimvertexes   = 0;
    nsimparticles  = 0;
    nneutinfos      = 0;

    for(int mod=0; mod<17; mod++){
      for(int cyc=0; cyc<23; cyc++){
	nidmodhits    [mod * 23 + cyc].clear();
	nb2modhits[mod][cyc] = 0;
      }
    }


    for(int mod=0; mod<17; mod++){
      for(int cyc=0; cyc<23; cyc++){
        for(int view=0; view<2; view++){
          nidb2modrecons    [(mod * 23 + cyc )*2 + view].clear();
          nb2modrecons[mod][cyc][view] = 0;
        }
      }
    }



    //nb2tracks  = 0;

    // Copy all arrays of detector summaries
    int i;
    for (i=0; i < evt.nb2simhits;    ++i) {
      AddB2SimHit(evt.GetB2SimHit(i));
    }
    for (i=0; i < evt.nb2hits;       ++i) {
      B2HitSummary*  tingsum = evt.GetB2Hit(i);
      int                    mod = tingsum -> mod;
      int                    cyc = tingsum -> cyc;
      AddB2ModHit   ( evt.GetB2Hit(i), mod, cyc);
    }
    for (i=0; i < evt.nbeamsummarys;    ++i) {
      AddBeamSummary(evt.GetBeamSummary(i));
    }
    for (i=0; i < evt.nbasicrecons;    ++i) {
      AddBasicRecon(evt.GetBasicRecon(i));
    }
    for (i=0; i < evt.nb2recons;    ++i) {
      B2ReconSummary*  tb2sum = evt.GetB2Recon(i);
      int                    mod = tb2sum -> hitmod;
      int                    cyc = tb2sum -> hitcyc;
      int                   view = tb2sum -> view;
      AddB2ModRecon   ( evt.GetB2Recon(i), mod, cyc, view);
    }
    for (i=0; i < evt.nb2anas;    ++i) {
      AddB2Ana(evt.GetB2Ana(i));
    }
    for (i=0; i < evt.nb2tracks;    ++i) {
      AddTrack(evt.GetTrack(i));
    }
    for (i=0; i < evt.nneutinfos; ++i) {
      AddNeut(evt.GetNeut(i));
    }

}

//......................................................................

B2EventSummary::~B2EventSummary() 
{ 
  this->Clear("C");

  if (fB2SimHit) {
    delete fB2SimHit;
    fB2SimHit = 0;
  }

  if (fB2Hit) {
    delete fB2Hit;
    fB2Hit = 0;
  }

  if (fSimParticle) {
    delete fSimParticle;
      fSimParticle = 0;
  }

  if (fSimVertex) {
    delete fSimVertex;
      fSimVertex = 0;
  }

  if (fBeamSummary) {
    delete fBeamSummary;
      fBeamSummary = 0;
  }
  if (fBasicRecon) {
    delete fBasicRecon;
    fBasicRecon = 0;
  }

  if (fB2Recon) {
    delete fB2Recon;
    fB2Recon = 0;
  }
  if (fB2Ana) {
    delete fB2Ana;
    fB2Ana = 0;
  }


  if (fB2Track) {
    delete fB2Track;
    fB2Track = 0;
  }
  if (fNeutInfo) {
    delete fNeutInfo;
    nneutinfos = 0;
  }

}

//......................................................................

void B2EventSummary::NewTClonesArrays()  
{ 

    fB2SimHit = new TClonesArray("B2SimHitSummary", 1000);
    fB2Hit    = new TClonesArray("B2HitSummary"   , 1000);
    fSimVertex    = new TClonesArray("B2SimVertexSummary", 1000);
    fSimParticle  = new TClonesArray("B2SimParticleSummary"   , 1000);
    fBeamSummary  = new TClonesArray("BeamInfoSummary"   , 1000);
    fBasicRecon   = new TClonesArray("B2BasicReconSummary"   , 1000);
    fB2Recon      = new TClonesArray("B2ReconSummary"   , 1000);
    fB2Ana        = new TClonesArray("B2AnaSummary"   , 1000);
    fB2Track  = new TClonesArray("B2TrackSummary"   , 1000);
    fNeutInfo     = new TClonesArray("NeutInfoSummary"   , 1000);
}

//......................................................................

void B2EventSummary::Clear(Option_t* option) 
{ 

    if (fB2SimHit) fB2SimHit -> Clear(option);
    if (fB2Hit)    fB2Hit    -> Clear(option);
    if (fSimParticle)  fSimParticle  -> Clear(option);
    if (fSimVertex)    fSimVertex    -> Clear(option);
    if (fBeamSummary)  fBeamSummary  -> Clear(option);
    if (fB2Recon)      fB2Recon      -> Clear(option);
    if (fB2Ana)        fB2Ana        -> Clear(option);
    if (fBasicRecon)   fBasicRecon   -> Clear(option);
    if (fB2Track)  fB2Track  -> Clear(option);
    if (fNeutInfo)     fNeutInfo     -> Clear(option);

    nb2simhits  = 0;
    nb2hits     = 0;
    nsimparticles   = 0;
    nsimvertexes    = 0;
    nbeamsummarys   = 0;
    nbasicrecons    = 0;
    nb2recons       = 0;
    nb2anas         = 0;
    nb2tracks   = 0;
    nneutinfos      = 0;
    for(int mod=0; mod<17; mod++){
      for(int cyc=0; cyc<23; cyc++){
	nidmodhits[mod*23+cyc].clear();
	nb2modhits[mod][cyc] = 0;
      }
    }
    for(int mod=0; mod<17; mod++){
      for(int cyc=0; cyc<23; cyc++){
	for(int view=0; view<2; view++){
	  nidb2modrecons    [(mod * 23 + cyc )*2 + view].clear();
	  nb2modrecons[mod][cyc][view] = 0;
	}
      }
    }

    // initialize rest of data members
    run     = 0;
    event   = 0;
    runmode = -10;
    trgid   = -10;
    version = ' ';
    date = -1;
    time = -1;
    trgtime = -1;
    for(int i=0;i<23;i++)bunch_flag[i]=false;
}

//......................................................................

void B2EventSummary::Print()
{
    std::cout << "Event summary: " << std::endl;

}

//......................................................................


B2SimHitSummary* B2EventSummary::AddB2SimHit(B2SimHitSummary* b2hitsum) 
{

    TClonesArray &b2simhit_s = *fB2SimHit;
    
//    use memberwise copy
    new(b2simhit_s[nb2simhits++]) B2SimHitSummary(*b2hitsum);

    return (B2SimHitSummary*)(fB2SimHit->At(nb2simhits-1));

}


//......................................................................

B2SimHitSummary* B2EventSummary::GetB2SimHit(int i) const
{ 

    if (i < nb2simhits && i>=0 ) return (B2SimHitSummary*)(fB2SimHit->At(i));
    return 0;

}


//......................................................................

B2HitSummary* B2EventSummary::AddB2Hit(B2HitSummary* b2hitsum) 
{

    TClonesArray &b2hit_s = *fB2Hit;
    
//    use memberwise copy
    new(b2hit_s[nb2hits++]) B2HitSummary(*b2hitsum);

    return (B2HitSummary*)(fB2Hit->At(nb2hits-1));

}



//......................................................................

B2HitSummary* B2EventSummary::GetB2Hit(int i) const
{ 

    if (i < nb2hits && i>=0 ) return (B2HitSummary*)(fB2Hit->At(i));
    return 0;

}



B2HitSummary* B2EventSummary::AddB2ModHit(B2HitSummary* b2hitsum, int nmod, int ncyc) 
{

    TClonesArray &b2hit_s = *fB2Hit;
    
    //##### push back nb2hits to hit # ID vector ######
    nidmodhits    [ nmod * 23 + ncyc ].push_back( nb2hits );
    nb2modhits[ nmod ][ ncyc ]++;


    new(b2hit_s[nb2hits++]) B2HitSummary(*b2hitsum);
    return (B2HitSummary*)(fB2Hit->At(nb2hits-1));

}


B2HitSummary* B2EventSummary::GetB2ModHit(int i, int nmod, int ncyc ) const
{ 


    if (i < nb2modhits[nmod][ncyc] && i>=0 ) return (B2HitSummary*)(fB2Hit->At( nidmodhits[nmod * 23 + ncyc][i] ));
    return 0;


}

B2SimVertexSummary* B2EventSummary::AddSimVertex(B2SimVertexSummary* simvertex) 
{

    TClonesArray &simvertex_s = *fSimVertex;
    
//    use memberwise copy
    new(simvertex_s[nsimvertexes++]) B2SimVertexSummary(*simvertex);

    return (B2SimVertexSummary*)(fSimVertex->At(nsimvertexes-1));
}


//......................................................................

B2SimVertexSummary* B2EventSummary::GetSimVertex(int i) const
{ 

    if (i < nsimvertexes && i>=0 ) return (B2SimVertexSummary*)(fSimVertex->At(i));
    return 0;

}


B2SimParticleSummary* B2EventSummary::AddSimParticle(B2SimParticleSummary* simparticle) 
{

    TClonesArray &simparticle_s = *fSimParticle;
    
//    use memberwise copy
    new(simparticle_s[nsimparticles++]) B2SimParticleSummary(*simparticle);

    return (B2SimParticleSummary*)(fSimParticle->At(nsimparticles-1));
}


//......................................................................

B2SimParticleSummary* B2EventSummary::GetSimParticle(int i) const
{ 

    if (i < nsimparticles && i>=0 ) return (B2SimParticleSummary*)(fSimParticle->At(i));
    return 0;

}
//......................................................................
BeamInfoSummary* B2EventSummary::AddBeamSummary(BeamInfoSummary* beamsummary) 
{

  TClonesArray &beamsummary_s = *fBeamSummary;
    
//    use memberwise copy
    new(beamsummary_s[nbeamsummarys++]) BeamInfoSummary(*beamsummary);
    return (BeamInfoSummary*)(fBeamSummary->At(nbeamsummarys-1));
}


//......................................................................

BeamInfoSummary* B2EventSummary::GetBeamSummary(int i) const
{ 

    if (i < nbeamsummarys && i>=0 ) return (BeamInfoSummary*)(fBeamSummary->At(i));
    return 0;

}

//......................................................................
B2BasicReconSummary* B2EventSummary::AddBasicRecon(B2BasicReconSummary* basicrecon) 
{

  TClonesArray &basicrecon_s = *fBasicRecon;
    
//    use memberwise copy
    new(basicrecon_s[nbasicrecons++]) B2BasicReconSummary(*basicrecon);
    return (B2BasicReconSummary*)(fBasicRecon->At(nbasicrecons-1));
}


//......................................................................
B2BasicReconSummary* B2EventSummary::GetBasicRecon(int i) const
{ 

    if (i < nbasicrecons && i>=0 ) return (B2BasicReconSummary*)(fBasicRecon->At(i));
    return 0;

}

//......................................................................
B2ReconSummary* B2EventSummary::AddB2Recon(B2ReconSummary* b2recon)
{

  TClonesArray &b2recon_s = *fB2Recon;

  //    use memberwise copy                                                                                        
  new(b2recon_s[nb2recons++]) B2ReconSummary(*b2recon);
  return (B2ReconSummary*)(fB2Recon->At(nb2recons-1));
}


//......................................................................
B2ReconSummary* B2EventSummary::GetB2Recon(int i) const
{

  if (i < nb2recons && i>=0 ) return (B2ReconSummary*)(fB2Recon->At(i));
  return 0;

}

//......................................................................
B2ReconSummary* B2EventSummary::AddB2ModRecon(B2ReconSummary* b2reconsum, int nmod, int ncyc, int nview)
{

  TClonesArray &b2recon_s = *fB2Recon;

  //##### push back nb2hits to hit # ID vector ######
  nidb2modrecons    [ (nmod * 23 + ncyc )*2 + nview ].push_back( nb2recons );
  nb2modrecons[ nmod ][ ncyc ][ nview ]++;


  new(b2recon_s[nb2recons++]) B2ReconSummary(*b2reconsum);
  return (B2ReconSummary*)(fB2Recon->At(nb2recons-1));

}

//......................................................................
B2ReconSummary* B2EventSummary::GetB2ModRecon(int i, int nmod, int ncyc, int nview ) const
{


  if (i < nb2modrecons[nmod][ncyc][nview] && i>=0 ) return (B2ReconSummary*)(fB2Recon->At( nidb2modrecons[(nmod * 23 + ncyc)*2 + nview][i] ));
  return 0;

}


//......................................................................
B2AnaSummary* B2EventSummary::AddB2Ana(B2AnaSummary* b2ana)
{
  TClonesArray &b2ana_s = *fB2Ana;
  //use memberwise copy                                                                                          
  new(b2ana_s[nb2anas++]) B2AnaSummary(*b2ana);
  return (B2AnaSummary*)(fB2Ana->At(nb2anas-1));
}


//......................................................................
B2AnaSummary* B2EventSummary::GetB2Ana(int i) const
{
  if (i < nb2anas && i>=0 ) return (B2AnaSummary*)(fB2Ana->At(i));
  return 0;
}


//......................................................................
NeutInfoSummary* B2EventSummary::AddNeut(NeutInfoSummary* neut) 
{
  TClonesArray &neutinfo_s = *fNeutInfo;

//    use memberwise copy
  new(neutinfo_s[nneutinfos++]) NeutInfoSummary(*neut);
  return (NeutInfoSummary*)(fNeutInfo->At(nneutinfos-1));
}


//......................................................................

NeutInfoSummary* B2EventSummary::GetNeut(int i) const
{ 
    if (i < nneutinfos && i>=0 ) return (NeutInfoSummary*)(fNeutInfo->At(i));
    return 0;
}



B2TrackSummary* B2EventSummary::AddTrack(B2TrackSummary* trk)
{
    TClonesArray &track_s = *fB2Track;
    
//    use memberwise copy
    new(track_s[nb2tracks++]) B2TrackSummary(*trk);
    return (B2TrackSummary*)(fB2Track->At(nb2tracks-1));

}


//......................................................................

B2TrackSummary* B2EventSummary::GetTrack(int i) const
{ 
    if (i < nb2tracks && i>=0 ) return (B2TrackSummary*)(fB2Track->At(i));
    return 0;

 }



ClassImp(B2EventSummary)


////////////////////////////////////////////////////////////////////////
