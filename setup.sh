export CVSROOT=:ext:ingrid@scbn00.hepnet.scphys.kyoto-u.ac.jp:/export/scbn00/data1/ingrid/mc
export CVS_RSH=ssh
unset CVS_SERVER
export CMTPATH=/export/scbn25/data1/nchikuma/T2K/work/basesoft
source /export/scbn25/data1/nchikuma/T2K/CMT/v1r20p20081118/mgr/setup.sh


#Set ND280 software package PATH
export ND280ROOT=/export/scbn25/data1/nchikuma/T2K/work/basesoft/ROOT/v5r22p00n00
export ND280GEANT=/export/scbn25/data1/nchikuma/T2K/work/basesoft/GEANT/v9r2p01n00
export ND280CLHEP=/export/scbn25/data1/nchikuma/T2K/work/basesoft/CLHEP/v2r0p3


#Set Env various by cmt
source $ND280GEANT/cmt/setup.sh
source $ND280ROOT/cmt/setup.sh
source $ND280CLHEP/cmt/setup.sh

#Set MY Env
export G4WORKDIR=`pwd`
export G4BIN=$G4WORKDIR/bin
export G4TMP=$G4WORKDIR/tmp
