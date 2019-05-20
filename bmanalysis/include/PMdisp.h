#ifndef __PMDRAW_HXX__
#define __PMDRAW_HXX__

#define dist 240  

#include <TBox.h>
#include "TH2.h"
#include "TCanvas.h"
#include <TLine.h>
#include <unistd.h>
#include <vector>

#include <fstream>
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
#include "TCanvas.h"
#include <TArc.h>

#include "DDimension.h"
#include "Dimension.h"

using namespace std;

Dimension *fdimension = new Dimension();

void xhit(int x,int y,double r){
  double X,Y,R;
  if(x==0)X=5;
  else X=46*x+9;
  if(x==0)Y=y*50+25;
  else{
  if(y<8)Y=y*50+25;
  else if(y<24)Y=412.5+(y-8)*25;
  else Y=(y-8)*50+25;
  }
  if(r<2)R=0;
  else R=sqrt(r-2)*3;
  TArc *arc=new TArc(X,Y,R);
  arc->SetFillColor(kRed);
  arc->SetLineColor(kRed);
  arc->Draw("SAME");

};


void yhit(int x,int y,double r){
  double X,Y,R;
  if(x==0)X=28;
  else X=46*x+32;
  if(x==0)Y=y*50+25;
  else{
  if(y<8)Y=y*50+25;
  else if(y<24)Y=412.5+(y-8)*25;
  else Y=(y-8)*50+25;
  }
  if(r<2)R=0;
  else R=sqrt(r-2)*3;
  TArc *arc=new TArc(X,Y,R);
  arc->SetFillColor(kRed);
  arc->SetLineColor(kRed);
  arc->Draw("SAME");
};


void vhit(int x,int y,double r){
  double X,Y,R;
  if(x==0)Y=-55;
  else Y=1255;
  //X=9.5+y*50;
  X=5+9.5+y*50;
  if(r<2)R=0;
  else R=sqrt(r-2)*3;
  TArc *arc=new TArc(X,Y,R);
  arc->SetFillColor(kRed);
  arc->SetLineColor(kRed);
  arc->Draw("SAME");
};




void xihit(int x,int y,double r){
  double X,Y,R;
  int offsety = -500;
  //X=105*x+791+dist;
  X=5+105*x+1074.5;
  Y=y*50+25 + offsety;
  if(r<2)R=0;
  else R=sqrt(r-2)*3;
  TArc *arc=new TArc(X,Y,R);
  arc->SetFillColor(kRed);
  arc->SetLineColor(kRed);
  arc->Draw("SAME");

};


void yihit(int x,int y,double r){
  double X,Y,R;
  //X=105*x+801+dist;
  X=5+105*x+1074.5+10;
  Y=y*50+25;
  if(r<2)R=0;
  else R=sqrt(r-2)*3;
  TArc *arc=new TArc(X,Y,R);
  arc->SetFillColor(kRed);
  arc->SetLineColor(kRed);
  arc->Draw("SAME");
};


void vihit(int x,int y,double r){
  double X,Y,R;
  if(x==0)Y=-55-20;
  else Y=1255+20;
  //X=9.5+y*50+1020;
  X=5+9.5+y*50+1075;
  if(r<2)R=0;
  else R=sqrt(r-2)*3;
  TArc *arc=new TArc(X,Y,R);
  arc->SetFillColor(kRed);
  arc->SetLineColor(kRed);
  arc->Draw("SAME");
};


void sci_ing(double x,double y,double x1,double y1){
  TBox *b1=new TBox(x,y,x1,y1);
  b1->SetLineColor(kGreen);
  b1->SetLineWidth(2);
  b1->SetFillStyle(0);
  b1->Draw("SAME");
};


void sci_par(double x,double y,double x1,double y1){
  TBox *b1=new TBox(x,y,x1,y1);
  b1->SetLineColor(kYellow);
  b1->SetLineWidth(2);
  b1->SetFillStyle(0);
  b1->Draw("SAME");
};

void sci_sci(double x,double y,double x1,double y1){
  TBox *b1=new TBox(x,y,x1,y1);
  b1->SetLineColor(kGreen+2);
  b1->SetLineWidth(2);
  b1->SetFillStyle(0);
  b1->Draw("SAME");
};

void sci_veto(double x,double y,double x1,double y1){
  TBox *b1=new TBox(x,y,x1,y1);
  b1->SetLineColor(kBlue);
  b1->SetLineWidth(2);
  b1->SetFillStyle(0);
  b1->Draw("SAME");
};


void iron(double x,double y,double x1,double y1){
  TBox *b1=new TBox(x,y,x1,y1);
  b1->SetFillColor(17);
  b1->SetLineWidth(2);
  b1->Draw("SAME");
};



void drawx(void){
  
  int k,j,i;
  int offsetz = 3000;
  int offsety = -500;

  for(k=0;k<11;k++){
    //for(j=0;j<24;j++)sci_ing(105*k+786+dist,j*50,105*k+796+dist,j*50+50);
    for(j=0;j<24;j++)sci_ing(105*k+1074.5,j*50+offsety,105*k+1084.5,j*50+50+offsety);
  }
  for(k=0;k<9;k++)iron(105*k+816+287,0+offsety,105*k+881+287,1200+offsety);

  //for(j=0;j<22;j++)sci_veto(-15.5+50*j+1020,-60-20,34.5+50*j+1020,-50-20);
  for(j=0;j<22;j++)sci_veto(-15.5+50*j+1075,-60-20+offsety,34.5+50*j+1075,-50-20+offsety);
  //for(j=0;j<22;j++)sci_veto(-15.5+50*j+1020,1250+20,34.5+50*j+1020,1260+20);
  for(j=0;j<22;j++)sci_veto(-15.5+50*j+1075,1250+20+offsety,34.5+50*j+1075,1260+20+offsety);


  
};


void drawy(void){
  
  int k,j,i;
  int offsetz = 3500;
  int offsety = 0;


  for(k=0;k<11;k++){
    //for(j=0;j<24;j++)sci_ing(105*k+796+dist,j*50,105*k+806+dist,j*50+50);
    for(j=0;j<24;j++)sci_ing(105*k+1084.5,j*50+offsety,105*k+1094.5,j*50+50+offsety);
  }
  for(k=0;k<9;k++)iron(105*k+816+287,0+offsety,105*k+881+287,1200+offsety);

  //for(j=0;j<22;j++)sci_veto(-15.5+50*j+1020,-60-20,34.5+50*j+1020,-50-20);
  for(j=0;j<22;j++)sci_veto(-15.5+50*j+1075,-60-20+offsety,34.5+50*j+1075,-50-20+offsety);
  //for(j=0;j<22;j++)sci_veto(-15.5+50*j+1020,1250+20,34.5+50*j+1020,1260+20);
  for(j=0;j<22;j++)sci_veto(-15.5+50*j+1075,1250+20+offsety,34.5+50*j+1075,1260+20+offsety);
};

/*
void tline(int mod,int xup,int xdown,float seppen,float katamuki){
  TLine *l1=new TLine(xup,seppen+xup*katamuki, xdown, seppen+xdown*katamuki);
  l1->SetLineWidth(5);
  l1->Draw("SAME");
};
*/


/*
void cline(int mod, int view, int pln, float up, float down, int value, int dist1){
  TLine *l1=new TLine(zposi(mod,view,pln),up, zposi(mod,view,pln+1+dist1), down);
  //l1->SetLineWidth(sqrt(sqrt(value)*8));
  l1->SetLineWidth(2);
  //l1->SetLineColor(28);
  l1->SetLineColor(kViolet-9+value);
  l1->Draw("SAME");
};
*/

void bmxhit(double pln, double ch, double r)
{
  double x, y, z;
  int offsety = 500;
  int offsetz = 3590;
  fdimension->get_pos_bm_FC(5, 1, pln, ch, &x, &y, &z, 0);
  
  //  cout  << "view= " << "1 " << "pln= " << pln << " ch= " << ch  << " z= " << z  << " x= " << x << '\n';

  TArc *arc = new TArc(z+offsetz,x+offsety,r);
  arc->SetFillColor(2);
  arc->SetLineColor(2);
  arc->Draw("SAME");
}

void bmyhit(double pln, double ch, double r)
{
  double x, y, z;
  int offsety = 600;
  int offsetz = 3590;
  fdimension->get_pos_bm_FC(5, 0, pln, ch, &x, &y, &z, 0);

  //  cout  << "view= " << "0 " << "pln= " << pln << " ch= " << ch  << " z= " << z  << " y= " << y << '\n';

  TArc *arc = new TArc(z+offsetz,y+offsety,r);
  arc->SetFillColor(2);
  arc->SetLineColor(2);
  arc->Draw("SAME");
}

void DetModule(double x1, double y1, double x2, double y2){
  TBox *b1 = new TBox(x1, y1, x2, y2);
  Int_t trans_blue = TColor::GetColorTransparent(kBlue, 0.5);
  //Int_t trans_blue = TColor::GetColorTransparent(kBlack, 0.2);                                    
  b1->SetLineColor(trans_blue);
  b1->SetFillColor(trans_blue);
  b1->SetLineWidth(2);
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

  int offsetz = 3600;
  int offsety = 600;

  //drawing module                                                                                  

  const int NumberOfScintiModule = 18;
  const int NumberOfIronModule = 33;

  ifstream fin("/Users/jinaren/Documents/T2K/BabyMIND/commissioning_run/event_estimation/macro/Myana\
lysis/Analysis/Scin_Mod_position.txt");
  double z[4][NumberOfScintiModule];
  for(int i=0; i<NumberOfScintiModule; i++){
    fin >> z[0][i] >> z[1][i] >> z[2][i] >> z[3][i];
  }

  ifstream fin2("/Users/jinaren/Documents/T2K/BabyMIND/commissioning_run/event_estimation/macro/My\
analysis/Analysis/Iron_Mod_position.txt");
  double zz[2][NumberOfIronModule];
  for(int i=0; i<NumberOfIronModule; i++){
    fin2 >> zz[0][i] >> zz[1][i];
  }


  double Scin_thickness = 40; //cm                                                                 
  double Iron_thickness = 40; //cm                                                                 

  for(int i=0; i<18; i++){
    DetModule(z[3][i]-Scin_thickness/2+offsetz, -986.2+offsety, z[3][i]+Scin_thickness/2+offsetz, 986.2+offsety);
  }

  for(int i=0; i<33; i++){
    IronModule(zz[1][i]-Iron_thickness/2-130+offsetz, -986.2+offsety, zz[1][i]+Iron_thickness/2-130+offsetz, 986.2+offsety);
  }

  //Draw magnetic direction                                                                         
  double x = -300;
  double y = -700;
  double y1 = 0;
  double y2 = 700;
  double r = 100;

  /*
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
  */


  fin.close();

}

void drawtview(void){

  //drawing module  

  int offsetz = 3600;
  int offsety = 500;

  const int NumberOfScintiModule = 18;
  const int NumberOfIronModule = 33;

  ifstream fin("/Users/jinaren/Documents/T2K/BabyMIND/commissioning_run/event_estimation/macro/Myana\
lysis/Analysis/Scin_Mod_position.txt");
  double z[4][NumberOfScintiModule];
  for(int i=0; i<NumberOfScintiModule; i++){
    fin >> z[0][i] >> z[1][i] >> z[2][i] >> z[3][i];
  }

  ifstream fin2("/Users/jinaren/Documents/T2K/BabyMIND/commissioning_run/event_estimation/macro/My\
analysis/Analysis/Iron_Mod_position.txt");
  double zz[2][NumberOfIronModule];
  for(int i=0; i<NumberOfIronModule; i++){
    fin2 >> zz[0][i] >> zz[1][i];
  }


  double Scin_thickness = 40; //cm                                                                 
  double Iron_thickness = 40; //cm                                                                 

  for(int i=0; i<18; i++){
    DetModule(z[3][i]-Scin_thickness/2+offsetz, -1420.5+offsety, z[3][i]+Scin_thickness/2+offsetz, 1420.5+offsety);
  }

  for(int i=0; i<33; i++){
    IronModule(zz[1][i]-Iron_thickness/2-130+offsetz, -1750+offsety, zz[1][i]+Iron_thickness/2-130+offsetz, 1750+offsety);
  }


  fin.close();


}



TCanvas *c1;

//bool Firstdraw=true;
void EvtDisp_bmonly(vector<vector<int> >Bmhit, int bmid){
  //if(Firstdraw){
  //c1 = new TCanvas("c1","c1",2050*0.8/2,400*1.03*2.5); 
  c1 = new TCanvas("c1","c1",2000, 500);    
  TH1F *h=new TH1F("","Top view",2050*10,0,8000);
  //TH1F *h=new TH1F("","Side view",820*10,0,820);
  h->SetMinimum(-1000);
  h->SetMaximum(+2000);
  h->GetXaxis()->SetLabelSize(0);
  h->GetYaxis()->SetLabelSize(0);
  h->SetStats(0);
  h->SetNdivisions(0);
  //TH1F *v=new TH1F("","Top view",2050*10,0,1875);
  
  TH1F *v=new TH1F("","Side view",2050*10,0,8000);
  //TH1F *v=new TH1F("","Top view",820*10,0,820);
  v->SetMinimum(-1000);
  v->SetMaximum(+2000);
  v->GetXaxis()->SetLabelSize(0);
  v->GetYaxis()->SetLabelSize(0);
  v->SetStats(0);
  v->SetNdivisions(0);
  
  c1->Divide(2,1);
  c1->cd(1);
  h->Draw();
  drawtview();

  c1->cd(2);
  v->Draw();
  drawsview();    
  //=============== Draw Hits ===============// 
  
  double r = 40;
  //id = 11;
 
  if(Bmhit.size()>0)
    {
      for(int i=0; i<Bmhit.at(bmid).size()/3; i++)
	{
	  int view = Bmhit.at(bmid).at(3*i+0);
	  int pln = Bmhit.at(bmid).at(3*i+1);
	  int ch = Bmhit.at(bmid).at(3*i+2);

	  if(view==0)
	    {
	      c1->cd(2);
	      bmyhit(pln, ch, r);
	    }
	  
	  if(view==1)
	    {
	      c1->cd(1);
	      bmxhit(pln, ch, r);
	    }
	}
    }

  c1->Update();    
  c1->Modified();
    
}

void EvtDisp(vector<vector<int> > Inghit, vector<vector<int> >Bmhit, int ingid, int bmid){
  //if(Firstdraw){
    c1 = new TCanvas("c1","c1",2050*0.8/2,400*1.03*2.5);    
    //Firstdraw=false;
    //}
  //*****Draw module*****
  //TH1F *h=new TH1F("","Side view",2050*10,0,1875);
  TH1F *h=new TH1F("","Top view",2050*10,0,8000);
  //TH1F *h=new TH1F("","Side view",820*10,0,820);
  h->SetMinimum(-1000);
  h->SetMaximum(+2000);
  h->GetXaxis()->SetLabelSize(0);
  h->GetYaxis()->SetLabelSize(0);
  h->SetStats(0);
  h->SetNdivisions(0);
  //TH1F *v=new TH1F("","Top view",2050*10,0,1875);
  
  TH1F *v=new TH1F("","Side view",2050*10,0,8000);
  //TH1F *v=new TH1F("","Top view",820*10,0,820);
  v->SetMinimum(-1000);
  v->SetMaximum(+2000);
  v->GetXaxis()->SetLabelSize(0);
  v->GetYaxis()->SetLabelSize(0);
  v->SetStats(0);
  v->SetNdivisions(0);
  
  c1->Divide(1,2);
  c1->cd(1);
  h->Draw();
  drawx();
  drawtview();

  c1->cd(2);
  v->Draw();
  drawsview();
  drawy();

    

  //=============== Draw Hits ===============// 
  
  double r = 40;
  //id = 11;

  if(Inghit.size()>0)
    {
      for(int i=0; i<Inghit.at(ingid).size()/3; i++)
	{
	  int view = Inghit.at(ingid).at(3*i+0);
	  int pln = Inghit.at(ingid).at(3*i+1);
	  int ch = Inghit.at(ingid).at(3*i+2);
	  
	  if(pln<11)
	    {
	      if(view==0)
		{
		  c1->cd(2);
		  yihit(pln, ch, r);
		}
	      
	      if(view==1)
		{
		  c1->cd(1);
		  xihit(pln, ch, r);
		}
	    }
	}
    }
 
  if(Bmhit.size()>0)
    {
      for(int i=0; i<Bmhit.at(bmid).size()/3; i++)
	{
	  int view = Bmhit.at(bmid).at(3*i+0);
	  int pln = Bmhit.at(bmid).at(3*i+1);
	  int ch = Bmhit.at(bmid).at(3*i+2);

	  if(view==0)
	    {
	      c1->cd(2);
	      bmyhit(pln, ch, r);
	    }
	  
	  if(view==1)
	    {
	      c1->cd(1);
	      bmxhit(pln, ch, r);
	    }
	}
    }

  c1->Update();    
  c1->Modified();

    
}


/*  
  printf("Type \'s\' to save the event display.\n");
  printf("Type \'q\' to quit.\n");
  
  char ans[8];
  fgets(ans,8,stdin);
  
  if( *ans == 's') c1->Print("~/Evtdisp.pdf");
  if( *ans == 'q') exit(0);
*/
 
  



#endif
