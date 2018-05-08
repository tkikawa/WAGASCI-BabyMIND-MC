GEANT4 simulation code for WAGASCI+BabyMIND at J-PARC
========

This is the GEANT4 Monte Calro simulation code for WAGASCI project at J-PARC. All the detector components, WAGASCI, Proton Module, Side MRD and Baby-MIND, are included.

## External libraries

### GEANT4

[GEANT4](http://geant4.cern.ch/) is a toolkit for the simulation of the passage of particles through matter developed by CERN. GEANT4.09.02.p01 has been tested.

### ROOT

[ROOT](https://root.cern.ch/) is an object-oriented program and library developed by CERN.
ROOT 5.22 has been tested.

## Run the simulation

### Usage
- B2MC [options]

### Options
- -i : Set input neut file  
- -o : Set output root file  
- -f : Neutrino flavor  
1: Muon neutrino  
2: Anti-muon neutrino  
3: Electron neutrino  
4: Anti-electron neutrino  
- -d : Set detector position setting  
1: [Proton Mod.] [WAGASCI1] [WAGASCI2]  
2: [WAGASCI1] [Proton Mod.] [WAGASCI2]  
3: [WAGASCI1] [WAGASCI2] [Proton Mod.]  
     Upstream <------> Downstream  
- -v : Set neutrino interaction detector  
1: Neutrino interaction in Proton Module  
2: Neutrino interaction in WAGASCI1  
3: Neutrino interaction in WAGASCI2  
- -w : Water in/out setting for WAGASCI  
0: WAGASCI1: water out, WAGASCI2: water out  
1: WAGASCI1: water in,  WAGASCI2: water out  
2: WAGASCI1: water out, WAGASCI2: water in  
3: WAGASCI1: water in,  WAGASCI2: water in  
- -m : Magnetic field on/off setting for BabyMIND  
- -b : Batch command  
