#include <vector>
#include <fstream>
#include <cstdio>
#include <stdlib.h>
#include <TColor.h>

#include <TArc.h>
#include <TBox.h>
#include "TH2.h"
#include "TCanvas.h"
#include <TLine.h>
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

#include "DDimension.h"
#include "Dimension.h"

using namespace std;

Dimension *fdimension = new Dimension();

//function for drawing hit
void hit(int i_data, int i_start, int *track_flag, 
	 vector <double> *tracktime, 
	 double SignalTime,
	 double x, double y, double r){

  if(i_data==i_start){
    if(tracktime->size()==0){
      tracktime->push_back(SignalTime);
      *track_flag = 1;
    }
  }

  if(i_data > i_start){

    for(int i_size=1; i_size<=tracktime->size(); i_size++){
      if(SignalTime<tracktime->at(i_size-1)+10
	 && SignalTime>tracktime->at(i_size-1)-10){

	*track_flag = i_size;
	

	continue;
      }

    }//i_size loop    
  }

    if(*track_flag==0){
      tracktime->push_back(SignalTime);
      *track_flag = tracktime->size();

    }

    double color = *track_flag + 3;
    //double color = 1;

    //if(*track_flag==1){
    TArc *arc = new TArc(x,y,r);
    arc->SetFillColor(color);
    arc->SetLineColor(color);
    arc->Draw("SAME");
    //}
}

void bmxhit(double pln, double ch, double r)
{
  double x, y, z;
  int offsety = 0;
  int offsetz = -15;
  fdimension->get_pos_bm_FC(5, 1, pln-1, ch, &x, &y, &z, 0);

  //  cout  << "view= " << "1 " << "pln= " << pln << " ch= " << ch  << " z= " << z  << " x= " << x << '\n';            

  TArc *arc = new TArc(z+offsetz,x+offsety,r);
  arc->SetFillColor(2);
  arc->SetLineColor(2);
  arc->Draw("SAME");
}

void bmyhit(double pln, double ch, double r)
{
  double x, y, z;
  int offsety = 0;
  int offsetz = -20;
  fdimension->get_pos_bm_FC(5, 0, pln-1, ch, &x, &y, &z, 0);

  //  cout  << "view= " << "0 " << "pln= " << pln << " ch= " << ch  << " z= " << z  << " y= " << y << '\n';            

  TArc *arc = new TArc(z+offsetz,y+offsety,r);
  arc->SetFillColor(2);
  arc->SetLineColor(2);
  arc->Draw("SAME");
}



//function for drawing module
void DetModule(double x1, double y1, double x2, double y2){
  TBox *b1 = new TBox(x1, y1, x2, y2);
  Int_t trans_blue = TColor::GetColorTransparent(kBlue, 0.7);
  //Int_t trans_blue = TColor::GetColorTransparent(kBlack, 0.2);
  b1->SetLineColor(1);
  b1->SetFillColor(trans_blue);
  //b1->SetFillColor(kBlue);
  b1->SetLineWidth(3);
  b1->SetFillStyle(1001);
  b1->Draw("SAME");
};

void IronModule(double x1, double y1, double x2, double y2){
  TBox *b1 = new TBox(x1, y1, x2, y2);
  Int_t trans_orange = TColor::GetColorTransparent(kOrange, 0.5);
  //Int_t trans_orange = TColor::GetColorTransparent(kBlack, 0.6);
  b1->SetLineColor(trans_orange);
  b1->SetFillColor(trans_orange);
  b1->SetLineWidth(2);
  b1->SetFillStyle(1001);
  b1->Draw("SAME");
};

void drawsview(void){

  //drawing module

  const int NumberOfScintiModule = 18;
  const int NumberOfIronModule = 33;
  const int NumberOfHor = 95;

 ifstream fin("/Users/jinaren/Documents/T2K/BabyMIND/commissioning_run/event_estimation/macro/Myanalysis/Analysis/Scin_Mod_position.txt");
 double z[4][NumberOfScintiModule];
   for(int i=0; i<NumberOfScintiModule; i++){
     fin >> z[0][i] >> z[1][i] >> z[2][i] >> z[3][i];
   } 
   
   ifstream fin2("/Users/jinaren/Documents/T2K/BabyMIND/commissioning_run/event_estimation/macro/Myanalysis/Analysis/Iron_Mod_position.txt");
   double zz[2][NumberOfIronModule];
   for(int i=0; i<NumberOfIronModule; i++){
     fin2 >> zz[0][i] >> zz[1][i];
   } 

   ifstream fin3("/Users/jinaren/Documents/T2K/BabyMIND/commissioning_run/event_estimation/macro/Myanalysis/Analysis/v1_horizontal_from_center.txt");
   double hor[5][NumberOfHor];
   double uhorl[48], dhorl[48];
   double uhorr[47], dhorr[47];
   for(int i=0; i<NumberOfHor; i++){
     fin3 >> hor[0][i] >> hor[1][i] >> hor[2][i] >> hor[3][i] >> hor[4][i];
     if(i%2==0)
       {
	 dhorl[i/2] = hor[4][i]-15; 
	 uhorl[i/2] = hor[4][i]+15; 
       }
     if(i%2==1)
       {
	 dhorr[i/2] = hor[4][i]-15; 
	 uhorr[i/2] = hor[4][i]+15; 
       }
   }
   
   double Scin_thickness = 40; //cm
   double Iron_thickness = 40; //cm
 
  for(int i=0; i<18; i++){
    //DetModule(z[3][i]-Scin_thickness/2, -986.2, z[3][i]+Scin_thickness/2, 986.2); 

    for(int j=0; j<48; j++)
      {
	DetModule(z[3][i]-10, dhorl[j], z[3][i], uhorl[j]); 
      }

    for(int j=0; j<47; j++)
      {
	DetModule(z[3][i], dhorr[j], z[3][i]+10, uhorr[j]); 
      }

  }

  for(int i=0; i<33; i++){
    IronModule(zz[1][i]-Iron_thickness/2-130, -986.2, zz[1][i]+Iron_thickness/2-130, 986.2); 
  }

  //Draw magnetic direction
  double x = -300;
  double y = -700;
  double y1 = 0;
  double y2 = 700;
  double r = 100;

  TArc *marrow1 = new TArc(x, y, r);
  TLine *t1 = new TLine(x-r/sqrt(2), y+r/sqrt(2), x+r/sqrt(2), y-r/sqrt(2)); 
  TLine *t2 = new TLine(x-r/sqrt(2), y-r/sqrt(2), x+r/sqrt(2), y+r/sqrt(2));

  TArc *marrow2 = new TArc(x, y1, r);
  TArc *marrow3 = new TArc(x, y1, r/10);

  TArc *marrow4 = new TArc(x, y2, r);
  TLine *t3 = new TLine(x-r/sqrt(2), y2+r/sqrt(2), x+r/sqrt(2), y2-r/sqrt(2)); 
  TLine *t4 = new TLine(x-r/sqrt(2), y2-r/sqrt(2), x+r/sqrt(2), y2+r/sqrt(2));
  TLine *t5 = new TLine(-100, 500, 4900, 500);
  t5->SetLineStyle(2);

  TLine *t6 = new TLine(-100, -500, 4900, -500);
  t6->SetLineStyle(2);


  marrow1->Draw("SAME");
  t1->Draw("SAME");
  t2->Draw("SAME");

  marrow2->Draw("SAME");
  marrow3->Draw("SAME");

  marrow4->Draw("SAME");
  t3->Draw("SAME");
  t4->Draw("SAME");
  t5->Draw("SAME");
  t6->Draw("SAME");
  

  fin.close();

}

void drawtview(void){

  //drawing module

  const int NumberOfScintiModule = 18;
  const int NumberOfIronModule = 33;
  const int NumberOfVer = 32;
  

 ifstream fin("/Users/jinaren/Documents/T2K/BabyMIND/commissioning_run/event_estimation/macro/Myanalysis/Analysis/Scin_Mod_position.txt");
 double z[4][NumberOfScintiModule];
   for(int i=0; i<NumberOfScintiModule; i++){
     fin >> z[0][i] >> z[1][i] >> z[2][i] >> z[3][i];
   } 
   
   ifstream fin2("/Users/jinaren/Documents/T2K/BabyMIND/commissioning_run/event_estimation/macro/Myanalysis/Analysis/Iron_Mod_position.txt");
   double zz[2][NumberOfIronModule];
   for(int i=0; i<NumberOfIronModule; i++){
     fin2 >> zz[0][i] >> zz[1][i];
   }

   ifstream fin3("/Users/jinaren/Documents/T2K/BabyMIND/commissioning_run/event_estimation/macro/Myanalysis/Analysis/v1_vertical_from_center.txt");
   double ver[5][NumberOfVer];
   double uverl[8], dverl[8];
   double uverr[7], dverr[7];
   for(int i=0; i<NumberOfVer; i++){
     fin3 >> ver[0][i] >> ver[1][i] >> ver[2][i] >> ver[3][i] >> ver[4][i];
     if(i%4==0)
       {
	 dverl[i/4] = ver[4][i]-100; 
	 uverl[i/4] = ver[4][i]+100; 
       }
     if(i%4==2)
       {
	 dverr[i/4] = ver[4][i]-100; 
	 uverr[i/4] = ver[4][i]+100; 
       }
   } 
   
 
   double Scin_thickness = 40; //cm
   double Iron_thickness = 40; //cm
 
  for(int i=0; i<18; i++){
    //DetModule(z[3][i]-Scin_thickness/2, -1315.2, z[3][i]+Scin_thickness/2, 1315.2); 

    for(int j=0; j<8; j++)
      {
	DetModule(z[3][i]-20, dverl[j], z[3][i]-10, uverl[j]); 
      }

    for(int j=0; j<7; j++)
      {
	DetModule(z[3][i]+10, dverr[j], z[3][i]+20, uverr[j]); 
      }

  }

  for(int i=0; i<33; i++){
    IronModule(zz[1][i]-Iron_thickness/2-130, -1750, zz[1][i]+Iron_thickness/2-130, 1750); 
  }


  fin.close();
  
}

TCanvas *monitor;
bool firstdraw=true;
 
//main program
 void BMdisp(int i_start, int i_stop, vector<double> positionz,
	    vector <double> positiony, vector <double> positionx,
	     vector <double> RSignalLeadTime, int Bunch)
{
  if(firstdraw)
    {
      monitor = new TCanvas("monitor", "monitor", 2000, 1600);
      monitor->Divide(1,2);
      firstdraw=0;
    }
  
  TH2F *h = new TH2F("","Side View", 5000, -100, 4900, 2172, -1086, 1086);
  //h->SetMinimum(0);
  //h->SetMaximum(1200);
  h->GetXaxis()->SetLabelSize(0);
  h->GetYaxis()->SetLabelSize(0);
  h->SetStats(0);
  h->GetXaxis()->SetNdivisions(0);
  h->GetYaxis()->SetNdivisions(0);

  TH2F *v = new TH2F("","Top View", 5000, -100, 4900, 3500, -1750, 1750 );
  //v->SetMinimum(0);
  //v->SetMaximum(1200);
  v->GetXaxis()->SetLabelSize(0);
  v->GetYaxis()->SetLabelSize(0);
  v->SetStats(0);
  v->GetXaxis()->SetNdivisions(0);
  v->GetYaxis()->SetNdivisions(0);

 
  monitor->cd(1);
  v->Draw();
  drawtview();

  monitor->cd(2);
  //monitor->cd();
  h->Draw();
  drawsview();

    
  //drawing hit

    vector <double> tracktime;
    tracktime.clear();


    
    for(int data = i_start; data < i_stop; data++){

      //if(Bunch>=4 && Bunch <=8 || Bunch<=2){
      //	continue;
      //}      

      
      int track_flag = 0;
      int R = 20;
      
      monitor->cd(1);
      hit(data, i_start, &track_flag,
	  &tracktime, RSignalLeadTime.at(data),
	  positionz.at(data), positionx.at(data), R);
     
            
      monitor->cd(2);
      //monitor->cd();
      
      hit(data, i_start, &track_flag,
	  &tracktime, RSignalLeadTime.at(data),
	  positionz.at(data), positiony.at(data), R);
      

    }
    

  //monitor
  //monitor->SaveAs(Directory_image + simagename.c_str() + ".pdf");
  monitor->Update();
  monitor->Modified();

  /*  
  cout << "Type \'s\' to save the event display" << '\n';
  cout << "Type \'q\' to quit" << '\n';
 
  char ans[8];
  fgets(ans, 8, stdin);

  if( *ans == 's' ) monitor->Print("/Users/jinaren/Documents/T2K/BabyMIND/J-PARC_physics_run/image/image.pdf");
  */
  //monitor->Clear();

  
};

void EvtDisp_bmonly(vector<vector<int> >Bmhit, int bmid){

  if(firstdraw)
    {
      monitor = new TCanvas("monitor", "monitor", 2000, 1600);
      monitor->Divide(1,2);
      firstdraw=0;
    }


  TH2F *h = new TH2F("","Side View", 5000, -100, 4900, 2172, -1086, 1086);
  //h->SetMinimum(0);
  //h->SetMaximum(1200);
  h->GetXaxis()->SetLabelSize(0);
  h->GetYaxis()->SetLabelSize(0);
  h->SetStats(0);
  h->GetXaxis()->SetNdivisions(0);
  h->GetYaxis()->SetNdivisions(0);

  TH2F *v = new TH2F("","Top View", 5000, -100, 4900, 3500, -1750, 1750 );
  //v->SetMinimum(0);
  //v->SetMaximum(1200);
  v->GetXaxis()->SetLabelSize(0);
  v->GetYaxis()->SetLabelSize(0);
  v->SetStats(0);
  v->GetXaxis()->SetNdivisions(0);
  v->GetYaxis()->SetNdivisions(0);

  monitor->cd(1);
  h->Draw();
  drawsview();

  monitor->cd(2);
  v->Draw();
  drawtview();
  //=============== Draw Hits ===============// 

  double r = 20;

  
  if(Bmhit.size()>0)
    {
      for(int i=0; i<Bmhit.at(bmid).size()/3; i++)
        {
          int view = Bmhit.at(bmid).at(3*i+0);
          int pln = Bmhit.at(bmid).at(3*i+1);
          int ch = Bmhit.at(bmid).at(3*i+2);

          if(view==0)
            {
              monitor->cd(1);
              bmyhit(pln, ch, r);
            }

          if(view==1)
            {
              monitor->cd(2);
              bmxhit(pln, ch, r);
            }
        }
    }
  

  monitor->Update();
  monitor->Modified();

}

