#include "B2EventAction.hh"
#include "B2RunAction.hh"
#include "B2HLayerSD.hh"
#include "B2VLayerSD.hh"
#include "B2VetoSD.hh"

#include "B2_Dimension.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4VisManager.hh"
#include "G4ios.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4Square.hh"

#include "Randomize.hh"

#include <iostream>
#include <assert.h>
#include "Riostream.h"

//#define DEBUG

//#define THRESHOLD 2.5 // p.e.
#define THRESHOLD 1.5 // p.e
#define VETO_THRESHOLD 3.5 // p.e.

using namespace CLHEP;

//
B2EventAction::B2EventAction(B2RunAction* rac)
  :runaction(rac)
{
  inghit = new B2HitSummary();
  ingsimhit = new B2SimHitSummary();
  Flag=0;
  fdim = new B2_Dimension();
}

B2EventAction::~B2EventAction()
{
  if(inghit) delete inghit;
  if(ingsimhit) delete ingsimhit;
}

//
void B2EventAction::BeginOfEventAction(const G4Event* anEvent)
{
}

// 
void B2EventAction::EndOfEventAction(const G4Event* anEvent)
{
  
  if(Flag!=-1) {

  G4int event_id = anEvent->GetEventID();
  
  // get number of stored trajectories
  G4TrajectoryContainer* trajectoryContainer = anEvent->GetTrajectoryContainer();
  G4int n_trajectories = -1;
  if (trajectoryContainer) {
    n_trajectories = trajectoryContainer->entries();
  }
  
  // periodic printing
  if (event_id>0&&event_id%1000 == 0) {
    G4cout << "Event: " << anEvent->GetEventID() << G4endl;
    G4cout << "    " << n_trajectories 
	   << " trajectories stored in this event." << G4endl;
  }

  G4SDManager* SDManager= G4SDManager::GetSDMpointer();


  //
  //Get Hit Collection of This Event
  G4HCofThisEvent* HCTE= anEvent-> GetHCofThisEvent();
  if(! HCTE) {
    G4cout << "no hits in this events. nothing to do!" << G4endl;
    return;
  }

  static G4int idcalx= -1;
  static G4int idcaly= -1;
  static G4int idcalv= -1;

  if(idcalx<0) idcalx= SDManager-> GetCollectionID("vlayerHitCollection");  
  if(idcaly<0) idcaly= SDManager-> GetCollectionID("hlayerHitCollection");
  if(idcalv<0) idcalv= SDManager-> GetCollectionID("vetoHitCollection");
  
  B2VLayerHitsCollection* vlhitcol =   (B2VLayerHitsCollection*)HCTE-> GetHC(idcalx);
  B2HLayerHitsCollection* hlhitcol =   (B2HLayerHitsCollection*)HCTE-> GetHC(idcaly);
  B2VetoHitsCollection* vehitcol =   (B2VetoHitsCollection*)HCTE-> GetHC(idcalv);  
    
  //
  int detid=0;
  int trkid=0;
  float edep=0.;
  double time=0.;
  float pos[3];
  int mod=-1, ch=-1, view=-1, pln=-1, pid=-1;
  double pe;
  double lope;
  int nsimhits;
  int nhitsvlayer = 0;
  int nhitshlayer = 0; 
  int nhitsveto = 0; 

  if(vlhitcol) nhitsvlayer = vlhitcol->entries(); 
  if(hlhitcol) nhitshlayer = hlhitcol->entries();   
  if(vehitcol) nhitsveto = vehitcol->entries();   

  //////// Action of hlayer Hits //////////
  if(hlhitcol){

    for(int l=0; l<nhitshlayer; l++){

      pid = (*hlhitcol)[l]->GetParticle();
      detid = (*hlhitcol)[l]->GetDetID();
      trkid = (*hlhitcol)[l]->GetTrackID();
      edep = (*hlhitcol)[l]->GetEdep();
      for(int i=0;i<3;i++) pos[i]=((*hlhitcol)[l]->GetPosition())[i];
      //time = (*hlhitcol)[l]->GetTime();
      time = (*hlhitcol)[l]->GetDelayTime();
      mod = (*hlhitcol)[l]->GetMod();
      pln = (*hlhitcol)[l]->GetPln();
      view = (*hlhitcol)[l]->GetView();
      ch = (*hlhitcol)[l]->GetCh();
      pe = (*hlhitcol)[l]->GetPE();
      lope = (*hlhitcol)[l]->GetLOPE();
      
      double posxy, posz, tmp;
      if(mod==0||mod==6)fdim -> get_posXY( mod, view, pln, ch, &posxy, &posz );
      else if(mod==1||mod==2)fdim -> get_pos_wgs_xy( mod, view, pln, ch, &posxy, &posz );
      else if(mod==3||mod==4)fdim -> get_pos_smrd( mod, pln, ch, &posxy, &posz );
      else if(mod==5&&view==0)fdim -> get_pos_bm( mod, view, pln, ch, &tmp, &posxy, &posz );
      else if(mod==5&&view==1)fdim -> get_pos_bm( mod, view, pln, ch, &posxy, &tmp, &posz );
      //
      inghit   -> Clear("C");
      inghit   -> mod  = mod;
      inghit   -> view = view;
      inghit   -> pln  = pln;
      inghit   -> ch   = ch;
      inghit   -> xy   = posxy;
      inghit   -> z    = posz;
      inghit   -> time = time;
      inghit   -> pe   = pe; 
      inghit   -> lope   = lope; 
      //
      ingsimhit-> Clear("C");
      ingsimhit-> edeposit = edep;
      ingsimhit-> trackid  = trkid;
      ingsimhit-> pdg      = pid;
      
      // set temporary hit efficiency
      if( inghit -> pe > THRESHOLD) {
	assert( runaction->GetEvtSum()->AddB2SimHit( ingsimhit ) );
	
	nsimhits = runaction->GetEvtSum()->NB2SimHits();
	inghit->AddB2SimHit( runaction->GetEvtSum()->GetB2SimHit( nsimhits-1 ) );
	runaction->GetEvtSum()->AddB2ModHit( inghit, mod, 4 );
      }
      
#ifdef DEBUG
      G4cout << "\n=== hits in horizontal layer (cyan) ===\n";
      if( pe > THRESHOLD ) 
	(*hlhitcol)[l]->Print();
#endif
      
      if( pe > THRESHOLD ) 
	(*hlhitcol)[l]->Draw();
    }
  }
  

  //////// Action of vlayer Hits //////////
  if( vlhitcol ){

    for(int l=0; l<nhitsvlayer; l++){

      pid = (*vlhitcol)[l]->GetParticle();
      detid = (*vlhitcol)[l]->GetDetID();
      trkid = (*vlhitcol)[l]->GetTrackID();
      edep = (*vlhitcol)[l]->GetEdep();
      for(int i=0;i<3;i++) pos[i]=((*vlhitcol)[l]->GetPosition())[i];
      //time = (*vlhitcol)[l]->GetTime();
      time = (*vlhitcol)[l]->GetDelayTime();
      mod = (*vlhitcol)[l]->GetMod();
      pln = (*vlhitcol)[l]->GetPln();
      view = (*vlhitcol)[l]->GetView();
      ch = (*vlhitcol)[l]->GetCh();
      pe = (*vlhitcol)[l]->GetPE();
      lope = (*vlhitcol)[l]->GetLOPE();



      double posxy, posz, tmp;
      if(mod==0||mod==6)fdim -> get_posXY( mod, view, pln, ch, &posxy, &posz );
      else if(mod==1||mod==2)fdim -> get_pos_wgs_xy( mod, view, pln, ch, &posxy, &posz );
      else if(mod==3||mod==4)fdim -> get_pos_smrd( mod, pln, ch, &posxy, &posz );
      else if(mod==5&&view==0)fdim -> get_pos_bm( mod, view, pln, ch, &tmp, &posxy, &posz );
      else if(mod==5&&view==1)fdim -> get_pos_bm( mod, view, pln, ch, &posxy, &tmp, &posz );
      //
      inghit   -> Clear("C");
      inghit   -> mod  = mod;
      inghit   -> view = view;
      inghit   -> pln  = pln;
      inghit   -> ch   = ch;
      inghit   -> xy   = posxy;
      inghit   -> z    = posz;
      inghit   -> time = time;//temporary
      inghit   -> pe   = pe; //temporary
      inghit   -> lope   = lope; //temporary
      //
      ingsimhit-> Clear("C");
      ingsimhit-> edeposit = edep;
      ingsimhit-> trackid  = trkid;
      ingsimhit-> pdg      = pid;
      
      //  if( (runaction->HitEff()) > G4UniformRand() ) {      
      if( inghit -> pe > THRESHOLD) {
	assert( runaction->GetEvtSum()->AddB2SimHit( ingsimhit ) );
	nsimhits = runaction->GetEvtSum()->NB2SimHits();
	
	inghit->AddB2SimHit( runaction->GetEvtSum()->GetB2SimHit( nsimhits-1 ) );
	runaction->GetEvtSum()->AddB2ModHit( inghit, mod, 4 );
      }
      
#ifdef DEBUG
      G4cout << "\n=== hits in vertical layer (yellow) ===\n";
      if( pe > THRESHOLD ) 
	(*vlhitcol)[l]->Print();
#endif
      
      if( pe > THRESHOLD ) 
	(*vlhitcol)[l]->Draw();
    }
  }


   ////// Action of veto Hits ////////// 
  if( vehitcol ) {

    for(int l=0; l<nhitsveto; l++){
      pid = (*vehitcol)[l]->GetParticle();
      detid = (*vehitcol)[l]->GetDetID();
      trkid = (*vehitcol)[l]->GetTrackID();
      edep = (*vehitcol)[l]->GetEdep();
      for(int i=0;i<3;i++) pos[i]=((*vehitcol)[l]->GetPosition())[i];
      //time = (*vehitcol)[l]->GetTime();
      time = (*vehitcol)[l]->GetDelayTime();
      mod = (*vehitcol)[l]->GetMod();
      pln = (*vehitcol)[l]->GetPln();
      view = (*vehitcol)[l]->GetView();
      ch = (*vehitcol)[l]->GetCh();
      pe = (*vehitcol)[l]->GetPE();
      lope = (*vehitcol)[l]->GetLOPE();
      
      double posxy, posz;
      fdim -> get_posXY( mod, view, pln, ch,
			 &posxy, &posz	 );
      //
      inghit   -> Clear("C");
      inghit   -> mod  = mod;
      inghit   -> view = view;
      inghit   -> pln  = pln;
      inghit   -> ch   = ch;
      inghit   -> xy   = posxy;
      inghit   -> z    = posz;
      inghit   -> time = time;//temporary
      inghit   -> pe   = pe ;//temporary
      inghit   -> lope   = lope ;//temporary

      ingsimhit-> Clear("C");
      ingsimhit-> edeposit = edep;
      ingsimhit-> trackid  = trkid;
      ingsimhit-> pdg      = pid;
      
      if( pe > VETO_THRESHOLD) {
	assert( runaction->GetEvtSum()->AddB2SimHit( ingsimhit ) );
	nsimhits = runaction->GetEvtSum()->NB2SimHits( );
	
	inghit->AddB2SimHit( runaction->GetEvtSum()->GetB2SimHit( nsimhits-1 ) );
	runaction->GetEvtSum()->AddB2ModHit( inghit, mod, 4 );
      }
      
      
#ifdef DEBUG 
      G4cout << "\n=== hits in veto (red) ===\n";
      if( pe > THRESHOLD )
	(*vehitcol)[l]->Print();
#endif
      
      if( pe > THRESHOLD )
	(*vehitcol)[l]->Draw();
      
    }
  }
  
      
  //
  // extract the trajectories and draw them 
  G4VVisManager* vis = G4VVisManager::GetConcreteInstance();

  if (vis) {

     for (G4int i=0; i<n_trajectories; i++){
       
       G4Trajectory* trj = (G4Trajectory*)
	 ((*(anEvent->GetTrajectoryContainer()))[i]);
	       
       if(trj->GetParentID()== 0) { //means particle created at neutrino interaction 

         // trj->G4Trajectory::DrawTrajectory(50);
  	 trj->G4Trajectory::DrawTrajectory();
       }

     } //end loop of n_trajecto 


    // Draw vertex of neutrino interaction
     G4ThreeVector vertex; 
     //for( int i=0;i<3;i++ ) vertex[i] = (runaction->GetTNeut()->vertex)[i]*cm;
     for( int i=0;i<3;i++ ) vertex[i] = (runaction->vertex)[i]*cm;
     G4Square square(vertex);
     square.SetScreenSize(6.5);
     square.SetFillStyle(G4Square::filled);
     G4Colour colour(1.,1.,1.);	 //means white
     G4VisAttributes attribs(colour);
     square.SetVisAttributes(attribs);
     vis->Draw(square);
     
  } // end of vis
  
  //

  // Fill Tree
  /*
  if(nhitsvlayer >2 && nhitshlayer>2){runaction->GetTree()->Fill();}
  runaction->GetEvtSum()->Clear("C");
  if(nhitsvlayer >2 && nhitshlayer>2){runaction->GetSKTree()->Fill();}//t2kreweight 
  */
  runaction->GetTree()->Fill();
  runaction->GetEvtSum()->Clear("C");
  runaction->GetSKTree()->Fill();//t2kreweight
    
  } // end of Flag
  
  else
    G4cout << "------- End of Event Action -------\n" << G4endl;
  
}
