#include <unistd.h>
#include <vector>

//#include "BMdisp.h"
#include "PMdisp.h"
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
  vector<vector<int> > Inghit, Bmhit;
  
  vector<int> *bmpln, *bmchx, *bmchy, *bmspill, *bmid;
  vector<int> *ingpln, *ingch, *ingview, *ingspill, *ingid;

  TBranch *b_bmpln, *b_bmchx,*b_bmchy, *b_bmspill, *b_bmid;
  TBranch *b_ingpln, *b_ingch, *b_ingview, *b_ingspill, *b_ingid;

  bmpln=0;
  bmchx=0;
  bmchy=0;
  bmspill=0;
  bmid=0;

  ingview=0;
  ingpln=0;
  ingch=0;
  ingspill=0;
  ingid=0;

  TFile *IngFileinput, *BmFileinput;

   TString bmdir = "/Users/jinaren/Documents/T2K/BabyMIND/commissioning_run/event_estimation/macro/Myanalysis/Event/";
   TString ingdir = "/Users/jinaren/Documents/T2K/BabyMIND/commissioning_run/event_estimation/macro/Myanalysis/INGRID_DATA_bac/process/";
   
   TString bmfile = "Event_5_May_2.root";
   TString ingfile = "ingrid_32159_10_process.root";

   TString inputingFile = ingdir + ingfile;
   TString inputbmFile = bmdir + bmfile;
   FileStat_t info;
   if (gSystem->GetPathInfo(inputingFile.Data(), info)!=0) 
     {
       printf("File '%s' does not exist.\n", inputingFile.Data());
       exit(0);
     } else {
     IngFileinput = new TFile(inputingFile);
   }

   if (gSystem->GetPathInfo(inputbmFile.Data(), info)!=0) 
     {
       printf("File '%s' does not exist.\n", inputbmFile.Data());
       exit(0);
     } else {
     BmFileinput = new TFile(inputbmFile);
   }

   cout << "File:" << inputingFile.Data() << " Analyzed ..." << '\n';
   cout << "File:" << inputbmFile.Data() << " Analyzed ..." << '\n';

   TTree *ingtree = (TTree*)IngFileinput->Get("tree");
   TTree *bmtree = (TTree*)BmFileinput->Get("events");

   ingtree->SetBranchAddress("view", &ingview, &b_ingview);
   ingtree->SetBranchAddress("ch", &ingch, &b_ingch);
   ingtree->SetBranchAddress("pln", &ingpln, &b_ingpln);
   ingtree->SetBranchAddress("spill", &ingspill, &b_ingspill);
   ingtree->SetBranchAddress("id", &ingid, &b_ingid);

   bmtree->SetBranchAddress("_horch", &bmchy, &b_bmchy);
   bmtree->SetBranchAddress("_verch", &bmchx, &b_bmchx);
   bmtree->SetBranchAddress("_pln", &bmpln, &b_bmpln);
   bmtree->SetBranchAddress("_SpillTag", &bmspill, &b_bmspill);
   bmtree->SetBranchAddress("_GTrigTime", &bmid, &b_bmid);

   for(int i_entry=0; i_entry<ingtree->GetEntries(); i_entry++)
     {
       ingtree->GetEntry(i_entry);
       Long64_t tentry = ingtree->LoadTree(i_entry);
       b_ingview->GetEntry(i_entry);
       b_ingch->GetEntry(i_entry);
       b_ingpln->GetEntry(i_entry);
       b_ingspill->GetEntry(i_entry);
       b_ingid->GetEntry(i_entry);
     }

   for(int i_entry=0; i_entry<bmtree->GetEntries(); i_entry++)
     {
       bmtree->GetEntry(i_entry);
       Long64_t tentry = bmtree->LoadTree(i_entry);
       b_bmchx->GetEntry(i_entry);
       b_bmchy->GetEntry(i_entry);
       b_bmpln->GetEntry(i_entry);
       b_bmspill->GetEntry(i_entry);
       b_bmid->GetEntry(i_entry);
     }

   //=================== INGRID HIT ======================//

   cout << "INGRID..." << '\n';
   //Copy ingspill
   vector<int> copyingspill;
   copyingspill.clear();
   for(int i=0; i<ingspill->size(); i++) 
     {
       copyingspill.push_back(ingspill->at(i));
     }

   auto last = unique(copyingspill.begin(), copyingspill.end());
   copyingspill.erase(last, copyingspill.end());
   //for(int i=0; i<ingspill->size(); i++) cout << ingspill->at(i) << '\n';
   const int NofIngEvent = copyingspill.size();


   //Copy ingid
   vector<int> copyingid;
   copyingid.clear();
   for(int i=0; i<ingid->size(); i++) 
     {
       copyingid.push_back(ingid->at(i));
     }

   last = unique(copyingid.begin(), copyingid.end());
   copyingid.erase(last, copyingid.end());

   cout << "spill size= " << copyingspill.size() << '\n';
   cout << "id size= " << copyingid.size() << '\n';
   cout << "first spill= " << copyingspill.at(0) << '\n'; 
   
   int ingindex[2];
   vector<int>tmpinghit;
   for(int i=0; i<copyingid.size(); i++)
     //for(int i=0; i<1; i++)
     {
       tmpinghit.clear();
       auto ingbounds = std::equal_range(ingid->begin(), ingid->end(), copyingid.at(i));
       ingindex[0] = ingbounds.first - ingid->begin();
       ingindex[1] = ingbounds.second - ingid->begin();
       //cout << "First= " << ingindex[0] << " Second= " << ingindex[1] << '\n';
       for(int i_index=ingindex[0]; i_index<ingindex[1]; i_index++)
	 {
	   tmpinghit.push_back(ingview->at(i_index));
	   tmpinghit.push_back(ingpln->at(i_index));
	   tmpinghit.push_back(ingch->at(i_index));
	 }
       Inghit.push_back(tmpinghit);
     }

   //=================== Baby MIND HIT ======================//

   cout << '\n';
   cout << "BabyMIND..." << '\n';

   //Copy ingspill
   vector<int> copybmspill;
   copybmspill.clear();
   for(int i=0; i<bmspill->size(); i++) 
     {
       copybmspill.push_back(bmspill->at(i));
     }

   last = unique(copybmspill.begin(), copybmspill.end());
   copybmspill.erase(last, copybmspill.end());
   //for(int i=0; i<bmspill->size(); i++) cout << bmspill->at(i) << '\n';
   const int NofBmEvent = copybmspill.size();

   //Copy bmid
   vector<int> copybmid;
   copybmid.clear();
   for(int i=0; i<bmid->size(); i++) 
     {
       copybmid.push_back(bmid->at(i));
     }

   last = unique(copybmid.begin(), copybmid.end());
   copybmid.erase(last, copybmid.end());


   cout << "spill size= " << copybmspill.size() << '\n';
   cout << "id size= " << copybmid.size() << '\n';
   cout << "first spill= " << copybmspill.at(0) << '\n';  
   cout << "last spill= " << copybmspill.at(copybmspill.size()-1) << '\n';     
   cout << '\n';

   
   int bmindex[2];
   vector<int>tmpbmhit;
   for(int i=0; i<copybmspill.size(); i++)
     //for(int i=0; i<1; i++)
     {
       tmpbmhit.clear();
       auto bmbounds = std::equal_range(bmid->begin(), bmid->end(), copybmid.at(i));
       bmindex[0] = bmbounds.first - bmid->begin();
       bmindex[1] = bmbounds.second - bmid->begin();
       //cout << "First= " << bmindex[0] << " Second= " << bmindex[1] << '\n';
       for(int i_index=bmindex[0]; i_index<bmindex[1]; i_index++)
	 {
	   for(int iview=0; iview<2; iview++)
	     {
	       if(iview==0)
		 {
		   tmpbmhit.push_back(iview);
		   tmpbmhit.push_back(bmpln->at(i_index));
		   tmpbmhit.push_back(bmchy->at(i_index));
		 }
	       if(iview==1)
		 {
		   tmpbmhit.push_back(iview);
		   tmpbmhit.push_back(bmpln->at(i_index));
		   tmpbmhit.push_back(bmchx->at(i_index));
		 }
	     }
	 }
       Bmhit.push_back(tmpbmhit);
     }


   //================== Find Matching Spill =================//

   vector<int> matchingspill;
   vector<int> matchingid;
   vector<int> matchbmid;
   matchingspill.clear();
   matchingid.clear();
   matchbmid.clear();

   for(int i=0; i<copyingspill.size(); i++)
     {
       int tryspill = copyingspill.at(i);
       for(int j=0; j<copybmspill.size(); j++)
	 {
	   int refspill = copybmspill.at(j);
	   int dif = tryspill-refspill;
	   if(dif==0) 
	     {
	       matchingspill.push_back(tryspill);
	       matchingid.push_back(i);
	       matchbmid.push_back(j);
	       cout << "ing= " << tryspill << " bm= " << refspill << '\n';
	       cout << "i= " << i << " j= " << j << '\n';
	       continue;
	     }

	   //if(dif<0) break;
	 }
     }

   cout << "matchingspill.size()= " << matchingspill.size() << '\n';
 

   //TCanvas *c1;

   /*
       int eventid;
       cout << "(ING) choose event from 0 to " << Inghit.size()  << '\n';
       cout << "(BM) choose event from 0 to " << Bmhit.size()  << '\n';
       cout << "quit : -1" << '\n';
       cin >> eventid;
   */
   TString Imgfilename;
   TString ImageDir = "/Users/jinaren/Documents/T2K/BabyMIND/commissioning_run/event_estimation/macro/Myanalysis/INGRID_DATA/image/matching/";
   //TString ImageDir = "/Users/jinaren/Documents/T2K/BabyMIND/commissioning_run/event_estimation/macro/Myanalysis/INGRID_DATA/image/32159_10_INGRID/";
   //TString ImageDir = "/Users/jinaren/Documents/T2K/BabyMIND/commissioning_run/event_estimation/macro/Myanalysis/INGRID_DATA/image/5_May_2_BabyMIND/";
   
   //for(int i=0; i<matchingid.size(); i++)
   for(int i=9; i<10; i++)
   //for(int i=0; i<copyingspill.size(); i++)
   //for(int i=0; i<1; i++)
     {
       int eventingid = matchingid.at(i);
       int eventbmid = matchbmid.at(i);
       //int eventingid = i;
       //int eventbmid = 1;
       cout << matchingspill.at(i) << '\n';
       EvtDisp(Inghit, Bmhit, eventingid, eventbmid);
       Imgfilename.Form("EventDisp_%d", matchingspill.at(i));
       //Imgfilename.Form("EventDisp_%d", copyingspill.at(i));
       //Imgfilename.Form("EventDisp_%d", copybmspill.at(i));
       //c1->SaveAs(ImageDir + Imgfilename + ".pdf");
       gSystem->ProcessEvents();
       c1->Modified();
       c1->Update();
       sleep(3);
     }
   app.Run();

       
       IngFileinput->Close();


  return 0;
}
