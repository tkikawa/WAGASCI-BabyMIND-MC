#include <iostream>
#include <vector>
#include <unistd.h>
#include <vector>

#include <TStyle.h>
#include <TSystem.h>
#include <TCanvas.h>
#include <algorithm>
#include <TApplication.h>
#include <stdio.h>
#include <string.h>
#include <exception>
#include <fstream>
#include <vector>
#include "TFile.h"
#include "TDirectory.h"
#include "TMacro.h"
#include <TTree.h>
#include <iostream>
#include <sstream>
#include "TH2.h"
#include "TH1.h"
#include "TH3.h"
#include "TF1.h"
#include <iterator>

using namespace std;

void Fit(int event, TH2 *histgram, double *coefficient_c, double *coefficient_ud, int *NDF){
  //void Fit(TH2 *histgram, double *coefficient_c, double *coefficient_ud, int *NDF){

  TF1 *fquad = new TF1("fquad", "[0]*x*x+x*[1]+[2]", 0, 4000);
  fquad->SetParameters(0, 0, 0);

  //parameter
  int rightbound;
  int leftbound;
  int NofHit;
  bool draw = false;
  bool fit = false;
  int count_d=0;
  int count_c=0;
  int count_u=0;

  ostringstream eventnum;
  string histname, histname_u, histname_c, histname_d, histname_a;
  string Monitor;
  eventnum.str();
  eventnum << event;
  histname_u = "Up Event_" + eventnum.str();
  histname_c = "Center Event_" + eventnum.str();
  histname_d = "Down Event_" + eventnum.str();
  histname_a = "All Event_" + eventnum.str();
  Monitor = "Monitor_" + eventnum.str();

  TH2D *uphist = new TH2D(histname_u.c_str(), histname_u.c_str(), 50, 0, 50, 95, 0, 95);
  TH2D *centerhist = new TH2D(histname_c.c_str(), histname_c.c_str(), 50, 0, 50, 95, 0, 95);
  TH2D *downhist = new TH2D(histname_d.c_str(), histname_d.c_str(), 50, 0, 50, 95, 0, 95);
  TH2D *allhist = new TH2D(histname_a.c_str(), histname_a.c_str(), 50, 0, 50, 95, 0, 95);

  /*
  vector <double> upx;
  vector <double> centerx;
  vector <double> downx;

  vector <double> upy;
  vector <double> centery;
  vector <double> downy;

  upx.clear();
  centerx.clear();
  downx.clear();

  upy.clear();
  centery.clear();
  downy.clear();
  */

  NofHit = histgram->GetEntries();
  
  for(int ix=0; ix<50; ix++){
    for(int iy=0; iy<95; iy++){
      double weight = histgram->GetBinContent(ix, iy);
      if(weight == 0){
	continue;
      }
      else if(iy<25){
	downhist->Fill(ix, iy, weight);
	allhist->Fill(ix, iy, weight);
	count_d++;
      }
      else if(iy>=25 && iy<=70){
	centerhist->Fill(ix, iy, weight);
	allhist->Fill(ix, iy, weight);
	count_c++;

      }
      else if(iy>70){
	uphist->Fill(ix, iy, weight);
	allhist->Fill(ix, iy, weight);
	count_u++;

      }
    }
  }

  int NofCenter = centerhist->GetEntries();

  if(NofCenter > 4){
    fit = true;
  }

  
  
  //  TCanvas* fitmonitor = new TCanvas(Monitor.c_str(), Monitor.c_str(), 1500, 800);
  //TCanvas* fitmonitor = new TCanvas("fitmonitor", "fitmonitor", 1500, 800);
  //fitmonitor->Divide(3, 2);
  //TCanvas* gfitmonitor = new TCanvas("gfitmonitor", "gfitmonitor", 1000, 600);

  /*
  if (draw){
  fitmonitor->cd(1);
  uphist->SetStats(0);
  uphist->Draw("colz");

  fitmonitor->cd(2);
  centerhist->SetStats(0);
  centerhist->Draw("colz");
  

  fitmonitor->cd(3);
  downhist->SetStats(0);
  downhist->Draw("colz");
  
  
  fitmonitor->cd(4);
  allhist->SetStats(0);
  allhist->Draw("colz");

  }
  */

  if(fit){

  /*
  uphist->Fit("fquad");
  *coefficient_ud = fquad->GetParameter(0);
  cout << *coefficient_ud << '\n';
  */
    //fitmonitor->cd(5);
    centerhist->Fit("fquad");
    *coefficient_c = fquad->GetParameter(0);
    *NDF = fquad->GetNDF();
    cout << *coefficient_c << '\n';
 
  /*
  downhist->Fit("fquad");
  *coefficient_ud = fquad->GetParameter(0);
  cout << *coefficient_ud << '\n';
  */

  }
   
  gStyle->SetPalette(1);
  
  /*
  for(int i_hit = 0; i_hit<NofHit; i_hit++){
    rightbound = 0;
    leftbound = 0;
    updown = 0;
    }
  */

  
  //fitmonitor->Modified();
  //fitmonitor->Update();
  //gSystem->ProcessEvents();
   
  
}
