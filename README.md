# GEANT4 simulation code for WAGASCI+BabyMIND at J-PARC

This is the GEANT4 Monte Carlo simulation code for WAGASCI project at
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
server. As for Ubuntu, it is the most wide-spread Linux distribution and is
useful if you want to test the software on your machine.

To install the software with all its dependencies I have written a bash shell
script called `install.bash`. Basically you just have to launch it and the
script will install everything automatically. Anyway, the script makes a
couple of assumptions behind the hood:

- If the OS is SL6, it is assumed that we are working in a KEKCC account
so we have no sudo access. This makes things a little harder but anyway the user is
relieved from any dirty work.
 - If the OS is Ubuntu, it is assumed that the user has complete access to the
 system.  So it is possible to run command with sudo.

To install the software first make sure that you have git installed (in Ubuntu
`sudo apt install git`, while git is already installed on the KEKCC server),
then issue the following commands in a bash shell:

```
cd
git clone https://github.com/tkikawa/WAGASCI-BabyMIND-MC
cd WAGASCI-BabyMIND-MC
chmod a+x install.bash
./install.bash
```

After the installation is concluded, it is better to exit from the ssh session
and login again to make sure that the environment is correctly set for later use
(the .bashrc file is automatically sourced only during login).

Note: if you already have an installation of ROOT and/or Geant4 on your machine, make
sure to source the thisroot.sh and/or geant4make.sh files before running the
script.

Note: the script takes quite long (a couple of hours depending on your hardware,
while on the KEKCC server only half an hour) to complete because it has to
compile a lot of code. In Ubuntu you need occasionally to input your sudo
password, so keep an eye on the shell.

### KEKCC

For further info about how to create an account and use the KEKCC cloud
computing servers, refer to the [WAGASCI
wiki](https://www-he.scphys.kyoto-u.ac.jp/research/Neutrino/WAGASCI/wiki/dokuwiki/doku.php?id=components:kekcc).

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

## Sample NEUT file
There is a neut file in sample_neutfile directory
in order to make sure of correct operation.

## ROOT files format
ROOT files made in WAGASCI MC have trees and classes shown below.
There is no description in a class.
Write additional information on making sense of it.

### Trees
- tree: This tree is used for reconstruction
- h1: This tree is used for T2K re-weight.
There are the same information as 2 classes, fSimParticle and fSimVertex in h1 tree.

### Classes
All the classes are in fDefaultReco class.
  - TObject
    - fUniqueID
    - fBits
  - run
  - event
  - runmode
  - trgid
  - version
  - date
  - time
  - trgtime
  - nd280nspill
  - bunch_flag[]
  - int nb2simhits: number of hits in MC
  - int nb2hits: number of hits (in MC, but it is able to use analysis of beam run, too)
  - int nsimparticles: number of particles made through neutrino interaction in MC
  - int nsimvertexes: number of vertexes
  - int nbeamsummarys
  - int nbasicrecons
  - int nb2recons
  - int nb2anas
  - int nb2tracks
  - int nneutinfos
  - int nb2modhits[][]
  - int nidmodhits[]
  - int nb2modrecons[][][]
  - int nidb2modrecons[]

#### B2SimHitSummary
  - B2SimHitSummary (fB2SimHit): the class for the true hit information in MC
    - fUniqueID
    - fBits
    - float edeposit: Energy deposit (MeV)
    - int trackid: GEANT4 particle ID of particles detected in each scintillator  
    Particle IDs are shown in below page.
    https://www.star.bnl.gov/public/comp/simu/gstar/Manual/particle_id.html
    - int pdg: Particle code (Scheme of PDG) of particles detected in each scintillator  
    Particle codes are shown in below document.
    http://pdg.lbl.gov/2007/reviews/montecarlorpp.pdf

#### B2HitSummary               
  - B2HitSummary (fB2Hit): the class for the hit information
    - fUniqueID
    - fBits
    - int mod: Module number (0-5)  
    Proton module = 0  
    WAGASCI1 (upstream) = 1  
    WAGASCI2 (downstream) = 2  
    South Side-MRD (left side) = 3  
    North Side-MRD (right side) = 4  
    Baby-MIND = 5  
    - int cyc: Cycle number (0-22), but all the values are -1 because we don't need to think of beam spills.
    - int view: horizontal or vertical layer  
    Horizontal layer = 0  
    Vertical layer = 1  
    - int pln: Plane number  
    In Side-MRDs:  
    0 1 2 --------- 7 8 9  
    (upstream) <---> (downstream)  
    - int ch: Channel number  
    In Side-MRDs:  
    0 1 2 --------- 5 6 7  
    (upstream) <---> (downstream)  
    - int adc: High gain ADC count
    - int loadc: Low gain ADC count
    - float pe: Number of p.e. with high gain ADC
    - float lope: Number of p.e. with low gain ADC
    - float pecorr: Number of p.e. supplemented in analysis
    - float vise: Visible energy  // I don't know this unit
    - float visecorr: Visible energy supplemented in analysis
    - long tdc: Time from trigger (ns)
    - float time: Hit time
    - (?) tnearhit  // I don't know this type
    - float timecorr: Hit time supplemented in analysis
    - (?) xy: Hit X or Y position (Scintillators are not sensible of both positions x and y)
    - (?) z: Hit Z position

#### B2SimParticleSummary
  - B2SimParticleSummary (fSimParticle): the class for the true secondary particle infomation in MC
    - fUniqueID
    - fBits
    - int trackid: GEANT4 particle ID of particles made through neutrino interaction  
    Particle IDs are shown in below page.
    https://www.star.bnl.gov/public/comp/simu/gstar/Manual/particle_id.html
    - int parentid: GEANT4 parent particle ID of particles made through neutrino interaction  
    Particle IDs are shown in below page.
    https://www.star.bnl.gov/public/comp/simu/gstar/Manual/particle_id.html
    - int pdg: Particle code (Scheme of PDG)  
    Particle codes are shown in ParticleCode(PDG) file in WAGASCI-BabyMIND-MC repository.
    - float momentum[4]: Particle's initial momentum
    - float ipos[4]: Particle's initial position/time (cm, ns)
    - float fpos[4]: Particle's final position/time (cm, ns)
    - int iposflag: Particle's initial position flag
    - int fposflag: Particle's final position flag
    - float dir[3]: Particle's direction
    - float length: Particle's track length

#### B2SimVertexSummary
  - B2SimVertexSummary (fSimVertex): the class for the true neutrino information in MC
    - fUniqueID
    - fBits
    - int nutype: Neutrino type  
    Muon neutrino = 1  
    Anti muon neutrino = 2  
    Electron neutrino = 3  
    Anti electron neutrino = 4  
    - float numomentum[3]: Neutrino momentum (GeV/c)  
    numomentum[0] = Px  
    numomentum[1] = Py  
    numomentum[2] = Pz  
    - float nuE: Neutrino energy (GeV)
    - float norm: Weight for JNUBEAM.
    It needs in normalization.
    - (?) ng
    - (?) gpid
    - (?) gmec
    - (?) gposx
    - (?) gposy
    - (?) gposz
    - (?) gmomx
    - (?) gmomy
    - (?) gmomz
    - (?) gcosbm
    - (?) targeta
    - (?) targetz
    - (?) targettype
    - float pfsurf: Fermi surface momentum (GeV/c)
    - float vnuclini: Nuclear potential for the target
    - int inttype: Neutrino interaction type  
    Interaction codes are shown in below document (p.59-60).
    https://www-he.scphys.kyoto-u.ac.jp/seminar/trape/FY2009/Neut_090616.pdf
    // I don't make sure that
    - int mod: Interaction vertex module  
    The same module number as fB2Hit.mod
    - float xnu: Interaction vertex X
    - float ynu: Interaction vertex Y
    - float znu: Interaction vertex Z
    - float totcrsne: Total cross section (x10^{-38} cm^2).
    It needs in normalization.

#### B2BasicReconSummary
  - B2BasicReconSummary (fBasicRecon): the class for memorizing the reconstructed track information.
  Values in this class are empty after running GEANT4 MC.
  Information is added when reconstructing tracks from hit information by using the program of track reconstruction.  
  // I don't make sure that all the members below are in this class.
    - float clstime: Time of cluster
    - float CTtimecorr: Beam timing at CT
    - float clstimecorr: Time of cluster with CT correction
    - float exptime: Difference from the expected timing
    - int nhitscluster: Number of hits
    - int nactpln: Number of active plane
    - float layerpe: Number of p.e. / active layer
    - int hitmod: Module number of the cluster
    - int hitcyc: Cycle number of the cluster
    - bool ontime: true: On-time / false: Off-time
    - bool vetowtracking: true: Upstream veto is hit
    - bool edgewtracking: true: Vertex is outside of FV (Fiducial Volume)
    - bool hastrk: true: 2D tracks are reconstructed
    - bool matchtrk: true: 2D tracks are matched
    - bool modfc: true: Track is fully contained
    - int penlron: Number of penetrated iron
    - float muE: Reconstructed muon energy
    - float nuErec: Reconstructed neutrino energy
    - int nhitx: Number of hits in X planes
    - int nhity: Number of hits in Y planes
    - int ntrackhitx: Number of hits in X planes which is associated with reconstructed track
    - int ntrackhity: Number of hits in Y planes which is associated with reconstructed track
    - int vertexz: Most upstream plane
    - int vertexxz: Most upstream plane among X planes
    - int vertexyz: Most upstream plane among Y planes
    - float angle: 3D track angle
    - float thetax: 2D track angle in XZ plane
    - float thetay: 2D track angle in YZ plane
    - int startxpln: Most upstream plane in reconstructed X track
    - int startypln: Most upstream plane in reconstructed Y track
    - int startxch: Most upstream channel in reconstructed X track
    - int startych: Most upstream channel in reconstructed Y track
    - int endxpln: Most downstream plane in reconstructed X track
    - int endypln: Most downstream plane in reconstructed Y track
    - int endxch: Most downstream plane in reconstructed X track
    - int endych: Most downstream plane in reconstructed Y track

## Normalization of MC
- Following factor must be weighted to all MC events.
********************
    norm x totcrsne x (Avogadro constant) x (Density of matter) x (Thickness of matter)
********************
- norm: Normalization factor for JNUBEAM  
- totcrsne: Normalization factor for NEUT (x10^{-38})  
- Avogadro constant = 6.02x10^{23}  
- Density of matter:  
  - Plastic scintillator = 1.03 g/cm^3
  - Water = 0.997 g/cm^3
  - Iron = 7.87 g/cm^3
  - Wall = 2.2 g/cm^3    
- Thickness of matter:  // Have to write additional description
  - WAGASCI =
  - Proton module = 46.2 cm
  - Side-MRD =
  - Baby-MIND =
- 1 MC file corresponds to 10^{21} POT after weighting the factor.
