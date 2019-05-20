#include<iostream>
#include<sstream>
#include<fstream>
//#include <iomanip.h>                                                
#include <sys/stat.h>
#include <algorithm>

#include <TROOT.h>
#include <TStyle.h>
#include <TApplication.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TClonesArray.h>
#include <TObject.h>
#include <TEventList.h>
#include <TBranch.h>
#include <TH1.h>
#include <TH2.h>

#include <TGraph.h>
#include <TGaxis.h>
#include <TMarker.h>
#include <TText.h>
#include <TMath.h>
#include <TSpectrum.h>
#include <TBox.h>
#include <TLatex.h>
#include <TString.h>
#include <TSystem.h>

#include "TApplication.h"
#include "DDimension.h"
#include "Dimension.h"
#include "mestimation.h"

using namespace std;

int main(int argc, char *argv[1])
{
  
  double expmom_r, expmom_c;
  double refa;
  int cid;
  int smode;
  TH2D *reReconTrack = new TH2D("reReconTrack", "reReconTrack", 200,
				0, 4100, 95, 0, 95);
  TGraph *Fittedplot;
  bool curvature = true;
  double chivalue[2];
  double initialloss = 0;

  vector<vector<double >>ReconTrack;

  vector<double> data;

  data.clear();
  data.push_back(1);
  data.push_back(34);
  ReconTrack.push_back(data);

  data.clear();
  data.push_back(2);
  data.push_back(35);
  ReconTrack.push_back(data);

  data.clear();
  data.push_back(3);
  data.push_back(37);
  ReconTrack.push_back(data);

  data.clear();
  data.push_back(4);
  data.push_back(40);
  ReconTrack.push_back(data);

  data.clear();
  data.push_back(5);
  data.push_back(45);
  ReconTrack.push_back(data);

  data.clear();
  data.push_back(6);
  data.push_back(53);
  ReconTrack.push_back(data);

  data.clear();
  data.push_back(7);
  data.push_back(63);
  ReconTrack.push_back(data);

  msuitei(&ReconTrack, &expmom_r, &expmom_c, refa, &cid, smode, reReconTrack, Fittedplot, curvature, chivalue, initialloss);




  return 0;
}
