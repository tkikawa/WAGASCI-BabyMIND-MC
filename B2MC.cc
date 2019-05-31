#include "G4RunManager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
//#include "QGSP.hh"
#include "QGSP_BERT.hh"
//#include "QGSP_BERT_HP.hh"
//#include "G4NeutronHPData.hh"

#include "B2DetectorConstruction.hh"
#include "B2PrimaryGeneratorAction.hh"
#include "B2RunAction.hh"
#include "B2EventAction.hh"
#include "B2TrackingAction.hh"
#include "Neut.hh"


#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

// ====================================================================
//     main
// ====================================================================
int main(int argc, char** argv) 
{
  char neutfile[300];
  char output[300];
  char cmd[300];
  
  int nd = 0;
  int waterin=-1;
  int batch = 0;
  int flav = 0;
  int vtx = 0;
  bool magfld = false;

  int c = -1;
  while ((c = getopt(argc, argv, "ho:i:d:v:w:mb:f:")) != -1) {
    switch(c){
    case 'o':
      sprintf(output,"%s",optarg);
      break;
    case 'i':
      sprintf(neutfile,"%s",optarg);
      break;
    case 'd':
      nd = atoi(optarg);
      break;
    case 'v':
      vtx = atoi(optarg);
      break;
    case 'w':
      waterin = atoi(optarg);
      break;
    case 'm':
      magfld = true;
      break;
    case 'f':
      flav = atoi(optarg);
      break;
    case 'b':
      batch = 1;
      sprintf(cmd,"%s",optarg);
      break;
    case 'h':
      std::cerr << "o: output root file name" << std::endl;
      std::cerr << "i: input neut file name" << std::endl;
      std::cerr << "d: detector position setting" << std::endl;
      std::cerr << "v: neutrino interaction posotion" << std::endl;
      std::cerr << "w: water setting" << std::endl;
      std::cerr << "f: neutrino flavor" << std::endl;
      std::cerr << "m: magnetic field" << std::endl;
      std::cerr << "b: batch command" << std::endl;
      exit(1);
    }
  }

  if( output == NULL ){
    G4cout << "Specify output root file name" << G4endl;
    exit(1);
  }
  if( neutfile == NULL ){
    G4cout << "Select input NEUT file" << G4endl;
    exit(1);
  }
  if( nd<1 || nd>3 ) {
    G4cout << "Select detector position seting" << G4endl;
    G4cout << "-d 1: [Proton Mod.] [WAGASCI1] [WAGASCI2]" << G4endl;
    G4cout << "-d 2: [WAGASCI1] [Proton Mod.] [WAGASCI2]" << G4endl;
    G4cout << "-d 3: [WAGASCI1] [WAGASCI2] [Proton Mod.]" << G4endl;
    G4cout << "     Upstream <------> Downstream" << G4endl;
    exit(1);
  }
  if( vtx<1 || vtx>3 ) {
    G4cout << "Select neutrion interaction position" << G4endl;
    G4cout << "-v 1: Neutrino interaction in Proton Module" << G4endl;
    G4cout << "-v 2: Neutrino interaction in WAGASCI1" << G4endl;
    G4cout << "-v 3: Neutrino interaction in WAGASCI2" << G4endl;
    exit(1);
  }
  if( waterin<0 || waterin>3 ) {
    G4cout << "Select Water seting" << G4endl;
    G4cout << "-w 0: WAGASCI1: water out, WAGASCI2: water out" << G4endl;
    G4cout << "-w 1: WAGASCI1: water in,  WAGASCI2: water out" << G4endl;
    G4cout << "-w 2: WAGASCI1: water out, WAGASCI2: water in"  << G4endl;
    G4cout << "-w 3: WAGASCI1: water in,  WAGASCI2: water in" << G4endl;
    exit(1);
  }
  if( flav<1 || flav>4 ) {
    G4cout << "Select neutrino flavor" << G4endl;
    G4cout << "-f 1: Muon neutrino" << G4endl;
    G4cout << "-f 2: Anti-muon neutrino" << G4endl;
    G4cout << "-f 3: Electron neutrino" << G4endl;
    G4cout << "-f 4: Anti-electron neutrino" << G4endl;
    exit(1);
  }

  // run manager
  G4RunManager* runManager= new G4RunManager;  //G4cout << G4endl;

  // Neut initialization
  // The Vector global object is initialized and partially populated 
  if ( NtupleReadInit(neutfile) <= 0 ) {
    G4cout << "Error when reading the input NEUT file"  << G4endl;
	exit(1);
  }
  
  G4cout << "Number of events :" << Vector.NumberOfEvents << G4endl;

  // set mandatory user initialization classes...

  // detector setup
  runManager-> SetUserInitialization(new B2DetectorConstruction(nd,waterin,magfld));
  G4cout << "Detector Init OK" << G4endl;

  // particles and physics processes
  //runManager-> SetUserInitialization(new QGSP);
  runManager-> SetUserInitialization(new QGSP_BERT);
  G4cout << "PhysicsList Init OK" << G4endl;

  B2RunAction * rac = new B2RunAction(output);
  G4cout << "RunAction init OK" << G4endl;
  
  B2EventAction * evt = new B2EventAction(rac);
  G4cout << "EventAction init OK" << G4endl;

  B2TrackingAction * tra = new B2TrackingAction(rac, evt);
  runManager->SetUserAction(tra);
  G4cout << "TrackingAction init OK" << G4endl;
  
  runManager-> SetUserAction(new B2PrimaryGeneratorAction(rac, evt, nd, vtx, flav));
  G4cout << "PrimaryGenerator init OK" << G4endl;
	
  // user action classes... (optional)
  runManager-> SetUserAction(rac);
  runManager-> SetUserAction(evt);

#ifdef G4VIS_USE
  // initialize visualization package
  G4VisManager* visManager= new G4VisExecutive;
  visManager-> Initialize();
  G4cout <<"visualization init OK" << G4endl;
#endif

  // Initialize G4 kernel
  runManager-> Initialize();
    
  // get the pointer to the UI manager and set verbosities
  G4UImanager* UI= G4UImanager::GetUIpointer();
  
  if(batch==1)
    // Define (G)UI terminal for interactive mode  
    { 
      // G4UIterminal is a (dumb) terminal.
      G4UIsession * session = 0;
      G4String command = "/control/execute ";
      G4String macro = cmd;
      session = new G4UIterminal(new G4UItcsh);
      UI->ApplyCommand(command+macro);
      session->SessionStart();
      delete session;
    }
  
  else { // batch mode
    runManager->BeamOn(Vector.NumberOfEvents);
  }
  
  // terminating...
#ifdef G4VIS_USE
  delete visManager;
#endif
  
  delete runManager;  //G4cout << G4endl;

  return 0;
}
