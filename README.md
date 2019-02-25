GEANT4 simulation code for WAGASCI+BabyMIND at J-PARC ========

This is the GEANT4 Monte Calro simulation code for WAGASCI project at
J-PARC. All the detector components, WAGASCI, Proton Module, Side MRD and
Baby-MIND, are included.

## External libraries

### GEANT4

[GEANT4](http://geant4.cern.ch/) is a toolkit for the simulation of the passage
of particles through matter developed by CERN. GEANT4.09.02.p01 and GEANT4
v10.5.0 have been tested.

### ROOT

[ROOT](https://root.cern.ch/) is an object-oriented program and library
developed by CERN.  ROOT 5.22 and ROOT 6.14.06 have been tested.

### Installation

Only two operative systems are tested and supported:
 - Ubuntu 18.04 64bit
 - Scientific Linux 6.10 64bit
 
The reason behind this choice is that the KEKCC work server is running on
Scientific Linux 6.10 and the few WAGASCI collaborators that will ever dare to
install and use this software, most likely will want to run it on the KEKCC
server. As for Ubuntu, it is the most wide-spread linux distribution so it is
probable that some collaborator might want to test the software on its machine.

To install the software with all its dependencies I have written a bash shell
script called `install.bash`. Basically you just have to launch it and answer a
couple of simple question and the script would install everything automatically.
Anyway, the script makes a couple of assumptions behind the hoods:

- If the OS is SL6, it is assumed that we are working in a KEKCC account
so we have no sudo access. This makes things harder but anyway the user is
relieved from any dirty work.
 - If the OS is Ubuntu, it is assumed that the user has complete access to the
 system.  So it is possible to run command with sudo.

To install the software first make sure that you have git installed (in Ubuntu
`sudo apt install git`, git is already install in the KEKCC server), then issue
the following commands in a bash shell:

```
cd
git clone https://github.com/tkikawa/WAGASCI-BabyMIND-MC
cd WAGASCI-BabyMIND-MC
chmod a+x install.bash
./install.bash
```

Disclaimer: consider that the script is still work in progress. You may (or may
not) need to adjust a few variables to reflect your particular needs.

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
