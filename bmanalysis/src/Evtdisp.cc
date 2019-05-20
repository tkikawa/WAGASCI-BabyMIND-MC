#include <unistd.h>
#include <vector>

#include "BMdisp.h"
//#include "PMdisp.h"
#include "Fit.h"
#include "recon.h"
#include "function.h"

#include <TLatex.h>
#include <TGraphErrors.h>
#include <TImage.h>
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
#include <TCut.h>
#include <TText.h>

using namespace std;

int main(int argc, char *argv[])
{
  TApplication app("app", &argc, argv);   
  
  vector<int> *verch, *horch, *pln;

  TBranch *bverch, *bhorch, *bpln; 

  verch=0;
  horch=0;
  pln=0;

  bverch=0;
  bhorch=0;
  bpln=0;


  TFile *IngFileinput, *BmFileinput;

  //TString bmdir = "/Users/jinaren/Documents/T2K/BabyMIND/commissioning_run/event_estimation/macro/Myanalysis/Recon/";
  TString bmdir = "/Users/jinaren/Dropbox/bmanalysis/example/";

   TString bmfile = "Merge.root";

   TString inputbmFile = bmdir + bmfile;
   FileStat_t info;

   if (gSystem->GetPathInfo(inputbmFile.Data(), info)!=0) 
     {
       printf("File '%s' does not exist.\n", inputbmFile.Data());
       exit(0);
     } else {
     BmFileinput = new TFile(inputbmFile);
   }

   cout << "File:" << inputbmFile.Data() << " Analyzed ..." << '\n';

   TTree *bmtree = (TTree*)BmFileinput->Get("mtree");

   bmtree->SetBranchAddress("horch", &horch, &bhorch);
   bmtree->SetBranchAddress("verch", &verch, &bverch);
   bmtree->SetBranchAddress("pln", &pln, &bpln);

   vector<vector<int> >Bmhit;
   Bmhit.clear();

   vector<int>tmpbmhit;
   tmpbmhit.clear();


   for(int i_entry=0; i_entry<bmtree->GetEntries(); i_entry++)
     {
       tmpbmhit.clear();

       bmtree->GetEntry(i_entry);
       Long64_t tentry = bmtree->LoadTree(i_entry);
       bverch->GetEntry(i_entry);
       bhorch->GetEntry(i_entry);
       bpln->GetEntry(i_entry);
     
       for(int i_index=0; i_index<pln->size(); i_index++)
	 {
	   tmpbmhit.push_back(0);
	   tmpbmhit.push_back(pln->at(i_index));
	   tmpbmhit.push_back(horch->at(i_index));
	   
	   tmpbmhit.push_back(1);
	   tmpbmhit.push_back(pln->at(i_index));
	   tmpbmhit.push_back(verch->at(i_index));
	 }       
       Bmhit.push_back(tmpbmhit);
     }

   //=================== Baby MIND HIT ======================//

   for(int i=0; i<200; i++)
     {
       if(Bmhit.at(i).size()>=250)
	 {
	   cout << "IEVT= " << i << " " << Bmhit.at(i).size() << '\n';
	 }
     }

   while(1)
     {
       int eventid = 1;

       cout << "insert eventid" << '\n';
       cout << "insert -1 to quit" << '\n';
       cin >> eventid;

       if(eventid==-1) break;


       EvtDisp_bmonly(Bmhit, eventid);
       gSystem->ProcessEvents();
       monitor->Modified();
       monitor->Update();
      
       for(int j=0; j<Bmhit.at(eventid).size()/3; j++)
	 {
	   if(Bmhit.at(eventid).at(3*j)==0)
	     {
	       cout << "view= " << Bmhit.at(eventid).at(3*j) << " pln= " << Bmhit.at(eventid).at(3*j+1)
		    << " ch= " << Bmhit.at(eventid).at(3*j+2) << '\n';
	     }
	 }

       for(int j=0; j<Bmhit.at(eventid).size()/3; j++)
	 {
	   if(Bmhit.at(eventid).at(3*j)==1)
	     {
	       cout << "view= " << Bmhit.at(eventid).at(3*j) << " pln= " << Bmhit.at(eventid).at(3*j+1)
		    << " ch= " << Bmhit.at(eventid).at(3*j+2) << '\n';
	     }
	 }

     }

   app.Run();       
   BmFileinput->Close();

  return 0;
}
