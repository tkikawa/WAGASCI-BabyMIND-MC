#include <unistd.h>
#include <vector>
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
#include <TMath.h>

#include "Dimension.h"

using namespace std;



void forcecut(double *a)
{
  int b;
  b = *a*1000;
  
  *a = b/1000.;  
}

int positioncheck(int ydata)
{
  if(ydata>=25 && ydata<=70)
    {
      return 0;
    }
  if(ydata<25)
    {
      return -1;
    }
  if(ydata>70)
    {
      return +1;
    }

}


void chargeselection(int i_charge, int *charge, int ydata)
{
  int posid = positioncheck(ydata);
  if(i_charge==0 && posid==0)
    {
      *charge = -1;
    }
  if(i_charge==0 && posid!=0)
    {
      *charge = +1;
    }
  if(i_charge==1 && posid==0)
    {
      *charge = +1;
    }
  if(i_charge==1 && posid!=0)
    {
      *charge = -1;
    }
}


void Delta(double z, double r, double a, double alpha,  double *othr, double *uthr, bool *next,
	   double charge, double *factor)
{ 
  double theta_i = TMath::ATan(a);
  
  //double theta_i = 0;
  double theta = TMath::ASin((z+r*TMath::Sin(theta_i))/r) - theta_i;
  double delta =  z*TMath::Tan(theta_i + charge*theta/2) ; // positive value
  double count = 0;
  int threshold = 10;
  double rest;
  bool plusflag;
  bool end = false;

  delta = delta + alpha;
  if(delta >=0) 
    {
      *factor = +1;
      plusflag = true;
    }
  if(delta < 0) *factor = -1;
  delta = fabs(delta);

  if(fabs(delta) > 300)
    {
      *next = true;
      count = 0;
      rest = 0;
      end = true;
    }

  *othr = delta/20.;

}

void Deltaoff(double z, double a, double alpha,  double *othr, double *uthr, bool *next,
	      double *factor)
{ 
  double delta =  z*a;
  double count = 0;
  int threshold = 10;
  double rest;
  bool plusflag;
  bool end;


  delta = delta + alpha;
  if(delta >=0) 
    {
      *factor = +1;
      plusflag = true;
    }
  if(delta < 0) *factor = -1;
  delta = fabs(delta);

  if(fabs(delta)>300)
    {
      *next = true;
      count = 0;
      rest = 0;
      end = true;
    }

  *othr = delta/20.;

}

double chisquare(vector<double> *y, vector<double> *Y, vector<int> recontrackpln)
{
  double value = 0;
  double tempy, tempY;
  int N = y->size();
  for(int i=0; i<N; i++)
    {
      int id = recontrackpln.at(i);
      tempy = y->at(i);
      tempY = Y->at(id-1);
      value += (tempy-tempY)*(tempy-tempY)/fabs(tempY);
    }

  return value/(N-1);
  
}

double dEdX(int i, double magnet[], double pmu)
{

  double dE;
  double a;
  double rho_iron = 7.874;
  double rho_sci = 1.038;

  double x[21] = {0, 1.025, 1.854, 3.437, 6.812, 10.91, 31.78, 43.82, 69.50, 109.9,
		  178.7, 247.2, 512.4, 640.2, 888.5, 1248, 1825, 2383, 4509, 5532, 
		  7524}; // CSDA range
  double y[21] = {0, 47.04, 56.16, 68.02, 85.09, 100.3, 152.7, 176.4, 221.8, 286.8, 
		  391.7,494.5, 899.5, 1101, 1502, 2103, 3104, 4104, 8105, 10110, 
		  14110};// MeV/c

  for(int i=0; i<21; i++)
    {
      if(pmu > y[i] && pmu < y[i+1])
        {
	  a = (y[i+1]-y[i])/(x[i+1] - x[i]);	  	  
          break;
        }
    }

  double drange = magnet[i]*3.0*rho_iron + 4.0*rho_sci; 
  dE = drange * a;

  return dE;

}

double dEdX_I(int i, double magnet[], double pmu, double initial)
{

  double dE;
  double a;
  double rho_iron = 7.874;
  double rho_sci = 1.038;

  double x[21] = {0, 1.025, 1.854, 3.437, 6.812, 10.91, 31.78, 43.82, 69.50, 109.9,
		  178.7, 247.2, 512.4, 640.2, 888.5, 1248, 1825, 2383, 4509, 5532,
		  7524}; // CSDA range
  double y[21] = {0, 47.04, 56.16, 68.02, 85.09, 100.3, 152.7, 176.4, 221.8, 286.8, 
		  391.7,494.5, 899.5, 1101, 1502, 2103, 3104, 4104, 8105, 10110, 
		  14110};// MeV/c

  for(int i=0; i<21; i++)
    {
      if(pmu > y[i] && pmu < y[i+1])
        {
	  a = (y[i+1]-y[i])/(x[i+1] - x[i]);	  	  
          break;
        }
    }

  double drange = magnet[i]*3.0*rho_iron + 4.0*rho_sci + initial; 
  dE = drange * a;

  return dE;

}

double momcal(double magnet[], vector<double> vecz, vector<double> vecy, vector<int> vecpln, double initialloss)
{
  int N = vecz.size();
  double recon = 0;
  double ironsum = 0;
  double ironrange, scirange;
  double range;
  double ratio;
  double magnetwidth = 30;
  double scintiwidth = 31;
  double rho_iron = 7.87;
  double rho_scinti = 1.03;

  for(int j=1; j<N; j++)
    {
      double tempz1, tempz2, tempy1, tempy2;
      double tempironrange, tempscirange;
      double dz, dy;
      double totalmagnet = 0; 
      int id = vecpln.at(j);
      tempz1 = vecz.at(j-1);
      tempz2 = vecz.at(j);
      tempy1 = vecy.at(j-1);
      tempy2 = vecy.at(j);
      dz = tempz2-tempz1;
      dy = tempy2-tempy1;

      if(id==1) totalmagnet = magnet[id-1];
      if(id!=1)
	{
	  for(int k=vecpln.at(j-1)+1; k<=id; k++)
	    {
	      totalmagnet += magnet[k-1];
	      if(j==1) totalmagnet += 3;
	    }
	}

      ratio = sqrt(dz*dz+dy*dy)/dz;
      tempironrange = ratio*totalmagnet*magnetwidth;
      tempscirange = ratio*scintiwidth;

      ironrange += tempironrange;
      scirange += tempscirange;
    }

  range = (ironrange*rho_iron + scirange*rho_scinti + scintiwidth*rho_scinti)/10 + initialloss;

  //cout << "range= " <<range << '\n';

  double x[21] = {0, 1.025, 1.854, 3.437, 6.812, 10.91, 31.78, 43.82, 69.50, 109.9,
                 178.7, 247.2, 512.4, 640.2, 888.5, 1248, 1825, 2383, 4509, 5532, 
		  7524};

  double x_kai[21];

  for(int i=0; i<21; i++)
    {
      x_kai[i] = x[i];
    }

  double y[21] = {0, 47.04, 56.16, 68.02, 85.09, 100.3, 152.7, 176.4, 221.8, 286.8, 
		  391.7,494.5, 899.5, 1101, 1502, 2103, 3104, 4104, 8105, 10110, 
		  14110};

  for(int i=0; i<20; i++)
    {
      double lefthand, righthand;
      double gapx, gapy;
      if(range > x_kai[i] && range < x_kai[i+1])
        {
          lefthand = fabs(range - x_kai[i]);
          righthand = fabs(range - x_kai[i+1]);
          gapx = x_kai[i+1]-x_kai[i];
          gapy = y[i+1]-y[i];
          recon = y[i] + gapy*(lefthand/gapx);
          break;
        }
    }

  return recon;

}

int positioncheck_m(double y)
{
  int check;
  if(y<-520)
    {
      check = -1;
    }

  if(y>=-520 && y<=520)
    {
      check = 0;
    }

  if(y>520)
    {
      check = +1;
    }

  return check;
}



void msuitei(vector<vector<double> >*ReconTrack , double *expectedmomentum_r, double *expectedmomentum_c, double refa, int *cid, int smode,TH2D *reReconTrack, TGraph *Fittedplot, bool curvature, double chivalue[], double initialloss)
{

  Dimension *fdimension = new Dimension();

  double detector_module_z[19] = {0, 0, 25, 42.7, 53.7, 64.7, 88.9, 106.7,
                                  138.2, 156.1, 186.8, 218.8, 236.7,
                                  261.8, 279.5, 324.3, 369, 401.1, 411.1};

  double iron_module_z[33] = {7, 14.2, 21,49.7, 60.7, 71.7,
			      95.9, 102.9, 113.7, 121, 145.2, 151.9,
			      163.1, 170, 193.8, 200.8, 207.8, 214.8,
			      243.7, 250.6, 257.8, 286.5, 293.7, 300.4,
			      307.4, 331.3, 338.3, 345.4, 352.1, 376,
			      383.3, 390.2, 397.1};

  for(int i=0; i<19; i++)
    {
      detector_module_z[i] = detector_module_z[i]*10;
    }

  for(int i=0; i<33; i++)
    {
      iron_module_z[i] = iron_module_z[i]*10;
    }

  TFile *histfile = new TFile("oneshot.root", "recreate");

  int last = ReconTrack->size();
  int count_track = 0;
  vector <double> temptracky;
  vector <double> temptrackch;
  vector <double> temptrackz;
  vector <double> recontrackZ;
  vector <double> recontrackY;
  vector <int> recontrackpln;
  vector <double> recontrackch;
  temptracky.clear();
  temptrackz.clear();
  recontrackY.clear();
  recontrackZ.clear();
  recontrackpln.clear();
  recontrackch.clear();

  for(int i_datapoint=0; i_datapoint<last; i_datapoint++)
    {
      int pln = ReconTrack->at(i_datapoint).at(0);
      double ch  = ReconTrack->at(i_datapoint).at(1);
      double datax, datay, dataz;
      
      fdimension->get_pos_bm_FC(5, 0, pln-1, ch, &datax, &datay, &dataz, smode);
      cout << "z= "  << dataz << " y= " << datay << " pln= " << pln << " ch= " << ch  << '\n';
      recontrackZ.push_back(dataz);
      recontrackY.push_back(datay);
      recontrackpln.push_back(pln);
      recontrackch.push_back(ch);
      
      reReconTrack->Fill(dataz, ch);

    }// idatapoint loop

  ostringstream scanvas;
  string canvasname;
  string histname1, histname2;
  double X1[17];
  double X2[16];
  double Y1[17];
  double Y2[16];
  double z_forgra[17], z_forgra1[17];
  double y_forgra[17], y_forgra1[17];
  double y_recon[17], z_recon[17];
  TH2D *rerecontrack = new TH2D("rerecontrack", "rerecontrack", 200, -50, 4100, 100, -970, 970);
  vector<vector<double> >ReRecon;
  ReRecon.clear();

  TH2D *hist0 = new TH2D("hist0", "hist0", 200, 0, 4100, 95, 0, 95);
  TH2D *hist_recon = new TH2D("hist_recon", "hist_recon", 200, 0, 4100, 95, 0, 95);

  hist0->SetTitle("chosen track;z (mm); y (ch)");
  hist0->GetXaxis()->SetLabelSize(0.05);
  hist0->GetYaxis()->SetLabelSize(0.05);
  hist0->GetXaxis()->SetTitleSize(0.05);
  hist0->GetYaxis()->SetTitleSize(0.05);
  //hist0->GetYaxis()->SetLabelOffset(0.05);
  hist0->SetStats(0);

  double pointz;
  double pointy;
  double gap[17];
  double cm = 1;

  //Track re-reconstruction

  //Define gap length
  gap[0] = 0;
  gap[1] = (detector_module_z[3]-detector_module_z[2])*cm;
  gap[2] = 0;
  gap[3] = 0;
  gap[4] = (detector_module_z[6]-iron_module_z[5])*cm;
  gap[5] = 0;
  gap[6] = (detector_module_z[8]-iron_module_z[9])*cm;
  gap[7] = 0;
  gap[8] = (detector_module_z[10]-iron_module_z[13])*cm;
  gap[9] = 0;
  gap[10] = (detector_module_z[12]-detector_module_z[11])*cm;
  gap[11] = 0;
  gap[12] = (detector_module_z[14]-detector_module_z[13])*cm;
  gap[13] = (detector_module_z[15]-iron_module_z[24])*cm;
  gap[14] = 0;
  gap[15] = (iron_module_z[29]-detector_module_z[16])*cm;
  gap[16] = 0;
  
  cout << "last= " << last << '\n';

  vector<double> Ydata;
  vector<double> Rdata;
  vector<double> Pdata;
  vector<double> Zdata;
  vector<double> Adata;
  vector<double> ydata;
  vector<double> trueydata;

  double deltaE[17];
  double magnet[17]={3, 0, 1, 1, 1, 2, 2, 2, 2, 4, 0, 3, 0, 4, 4, 4, 0};
  double scinti = 1;
  double rho = 7.87; //cm/g
  double mip = 2.0; // MeV
  double iron = 3.1; //cm
  double layer = 4.0;
  double plastic = 1.03; //cm/g


  double a, Y, R, P;
  double opta[2], opty[2], optr[2], optp[2];
  double minchi[2] = {pow(10, 20), pow(10, 20)};

  double  B_default = 1.5;
  double B[17];
  double dE;
  double epsilon = pow(10, -5);

  B[16] = 0;
  for(int i=0; i<16; i++)
    {
      B[i] = (magnet[i]*30)/(magnet[i]*30 + gap[i] + 40 +  (magnet[i]-1)*10)*B_default;
      //cout << "B[" << i  << "]" << B[i] << '\n';
    }

  bool magnetflag[17] = {true, false, true, true, true, true, true, true, true, true,
			 false, true, false, true, true, true, false};

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //Momentum Estimation by range

  double rangemomentum;
  for(int i=0; i<recontrackpln.size(); i++)
    {
      //cout << "z= " << recontrackpln.at(i) << '\n';
    }
  double centerR = momcal(magnet, recontrackZ, recontrackY, recontrackpln, initialloss)/(0.3*B[0]);
  rangemomentum = momcal(magnet, recontrackZ, recontrackY, recontrackpln, initialloss);
  double maxP, minP;
  maxP = rangemomentum + 300;
  minP = rangemomentum - 300;

  //cout << "CENTER R= " << centerR << '\n';
  // cout << "range momentum= " <<  momcal(magnet, recontrackZ, recontrackY) << '\n';
  //cout << "range momentum = " <<  rangemomentum << '\n';
  //cout << "Size of Track = "  << recontrackZ.size() << '\n';


  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


  //######################### Compressed Track ###########################//
  //rerecontrack
  //vector ReRecon (z, y)
  //######################################################################//

  
  //######################### Momentum Estimation -Ichikawa System- ########################//
  //R: Radius, delta: Correction term, Y: Estimated y position
  //R(1) -> delta(1) -> Y(1)
  //|
  //R(2) -> delta(2) -> Y(2)
  //|
  //R(3) -> delta(3) -> Y(3)
  //
  //Parameter : a, Y(1), R(1)
  //######################################################################//

  TH2D *chimap;
 
  if(curvature)
    {

      Ydata.clear();
      Rdata.clear();
      Pdata.clear();
      Zdata.clear();
      Adata.clear();
      ydata.clear();
      trueydata.clear();

      int N = recontrackZ.size();
      int lastpln = recontrackpln.at(N-1);
      cout << "lastpln= " << lastpln << '\n';
      
      for(int i=0; i<N; i++)
	{
	  //Zdata.push_back(recontrackZ.at(i));
	  ydata.push_back(recontrackch.at(i));
	  y_recon[i] = recontrackch.at(i);
	}

      for(int i=0; i<lastpln; i++)
	{
	  double tempx, tempy, tempz;
	  fdimension->get_pos_bm_FC(5, 0, i+1, 0, &tempx, &tempy, &tempz, smode);
	  Zdata.push_back(tempz);
	  z_recon[i] = tempz;
	  //cout << tempz << '\n';
	}

      

  /*
  for(int i=0; i<17; i++)
    {
      deltaE[i] = mip*(rho*magnet[i]*iron+plastic*scinti*layer);
    }
  */

  double fposx, fposz, fposy1, fposy2;
  fdimension->get_pos_bm_FC(5, 0, 0, ydata.at(0), &fposx, &fposy1, &fposz, smode);
  fdimension->get_pos_bm_FC(5, 0, 0, ydata.at(1), &fposx, &fposy2, &fposz, smode);

  double centera = (fposy2-fposy1)/(Zdata.at(1)-Zdata.at(0));
  cout << "center a (1)= " << centera << '\n';
  forcecut(&centera);
  cout << "center a (2)= " << centera << '\n';
  double centerY = ydata.at(0);
  cout << "CENTER Y = " << centerY << '\n';

  //========= Binnning ============

  bool bias = false;
  
  double da = 0.01;
  int abin = 10;
  double amin = -abin*da/2. + centera;
  double amax = abin*da/2. + centera;

  double dY = 0.5;  
  int Ybin = 20;
  double Ymin = -Ybin*dY/2. + centerY;
  double Ymax = Ybin*dY/2. + centerY;

  //double Ymin = -Ybin*dY/2. + 50.0;
  //double Rmin = -Rbin*dR/2. + centerR;

  double dR, Rmin, Rmax;
  int Rbin = 200;
 
  if(bias)
    {  
      if(last==16 || last==17)
	{
	  dR = 50;
	  Rmin = 1400/(0.3*B[0]);
	  Rmax = Rbin*dR + Rmin;
	}
      else
	{
	  Rmin = minP/(0.3*B[0]);
	  Rmax = maxP/(0.3*B[0]);
	  dR = (Rmax-Rmin)/Rbin;
	}
    }

  if(!bias)
    {
      Rmin = 1000;
      Rmax = 8000;
      dR = (Rmax-Rmin)/Rbin;
    }

  
  //////////////////////////////////

  chimap = new TH2D("chimap", "chimap", Ybin, Ymin, Ymax, Rbin-1, Rmin*0.3*B[0], Rmax*0.3*B[0]);
  chimap->SetTitle("Chimap;Y (ch);P (MeV/c)");
  chimap->GetXaxis()->SetLabelSize(0.05);
  chimap->GetYaxis()->SetLabelSize(0.045);
  chimap->GetXaxis()->SetTitleSize(0.05);
  chimap->GetYaxis()->SetTitleSize(0.045);
  chimap->GetYaxis()->SetTitleOffset(1.18);
  chimap->SetStats(0);

  int charge = 0;
  double chistock[2] = {0, 0};
  double factor;

  for(int i_charge=0; i_charge<=1; i_charge++)
    {
      for(int i_a=0; i_a<abin; i_a++)
	{
	  for(int i_Y=0; i_Y<Ybin; i_Y++)
	    {
	      for(int i_R=0; i_R<Rbin; i_R++)
		{
		  Ydata.clear();
		  Rdata.clear();
		  Pdata.clear();
		  bool Rflag = false;
		  bool next = false;
		  double alpha = 0;
		  
		  a = da * i_a + amin;
		  Y = dY * i_Y + Ymin;
		  R = dR * i_R + Rmin;
		  P = 0.3*B[0]*R;
		  
		  Rdata.push_back(R);
		  Pdata.push_back(P);
		  Ydata.push_back(Y);

	      for(int i=0; i<lastpln-1; i++)
		{	
		  double tempr, tempy, tempz, tempp, tempa, othr, uthr, 
		    tempposy1, tempposy2, tempposz, tempposx;
		  chargeselection(i_charge, &charge, Ydata.at(i));
		  if(i!=0)
		    {
		      dE = dEdX(i, magnet, Pdata.at(i));
		    }
		  if(i==0)
		    {
		      dE = dEdX_I(i, magnet, Pdata.at(i), initialloss);
		    }
		  //cout << "i= "  << i  <<  " R= " << R << '\n';  
		  
		  if(magnetflag[i])
		    {
		      if(i!=0) tempz = Zdata.at(i+1) - Zdata.at(i);
		      if(i==0) tempz = Zdata.at(1) - Zdata.at(0);
		      
		      if(i!=0) 
			{
			  //cout << "check" << '\n';
			  fdimension->get_pos_bm_FC(5, 0, 0, Ydata.at(i-1), 
						    &tempposx, &tempposy1, 
						    &tempposz, smode);
			  fdimension->get_pos_bm_FC(5, 0, 0, Ydata.at(i), 
						    &tempposx, &tempposy2, 
						    &tempposz, smode);
			  //cout << "check" << '\n';
			  tempa = (tempposy2 - tempposy1)/(Zdata.at(i)-Zdata.at(i-1));
			}

		      if(i==0) tempa = a;
		
		      tempr = Pdata.at(i)/(0.3*B[i]);

		      //cout << "check2" << '\n';
		      //cout << "tempz= " << tempz << " tempr= " << tempr << " tempa= " << tempa << '\n'; 
		      Delta(tempz, tempr, tempa, alpha, &othr, &uthr, &next, 
			    charge, &factor);
		      //cout << "next= " << next << " alpha= " << alpha << '\n';
		      if(next) break;
		      //cout << "check2" << '\n';
		      tempy = Ydata.at(i) + othr*factor;
		      alpha = uthr;

		      tempp = Pdata.at(i) - dE;
		      
		      Rdata.push_back(tempr);
		      Ydata.push_back(tempy);
		      Adata.push_back(tempa);
		      Pdata.push_back(tempp);			
		    }

		  if(!magnetflag[i])
		    {
		      if(i!=0) tempz = Zdata.at(i+1) - Zdata.at(i);
		      if(i==0) tempz = Zdata.at(1) - Zdata.at(0);
		      
		      if(i!=0) 
			{
			  //cout << "check1" << '\n';
			  fdimension->get_pos_bm_FC(5, 0, 0, Ydata.at(i-1), 
						    &tempposx, &tempposy1, 
						    &tempposz, smode);
			  fdimension->get_pos_bm_FC(5, 0, 0, Ydata.at(i), 
						    &tempposx, &tempposy2, 
						    &tempposz, smode);
			  tempa = (tempposy2 - tempposy1)/(Zdata.at(i)-Zdata.at(i-1));
			  //cout << "check1" << '\n';
			}

		      if(i==0) tempa = a;

		      Deltaoff(tempz, tempa, alpha, &othr, &uthr, &next, &factor);
		      if(next) break;
		      tempy = Ydata.at(i) + othr*factor;
		      alpha = uthr;
		      tempp = Pdata.at(i) - dE;

		      Rdata.push_back(tempr);
		      Ydata.push_back(tempy);
		      Pdata.push_back(tempp);
		      Adata.push_back(tempa);		      
			
		    }
		      
		      if(tempp < 0)
			{
			  Rflag = true;
			  break;
			}
		}//i loop

	      //cout << "next= " << next << '\n';

	  	
	      if(!Rflag && !next)
		{
		  double chi2 = chisquare(&ydata, &Ydata, recontrackpln);	  
		  //cout << "chi2= " << chi2 << '\n';
		  if(chi2 < minchi[i_charge])
		    {
		      opta[i_charge] = a; 
		      opty[i_charge] = Y;
		      optr[i_charge] = R;
		      optp[i_charge] = 0.3*B[0]*R;
		      minchi[i_charge] = chi2;
		      trueydata.clear();
		      for(int i=0; i<lastpln; i++)
			{
			  trueydata.push_back(Ydata.at(i));
			}
		    }
		  
		  if(chi2<100 && i_charge==1 && fabs(a-0.072)<0.001)
		    {
		      chimap->Fill(Y, (double)R*0.3*B[0], chi2);
		    }
		  
		}
		}//R loop	 
	    }//Y loop
	}//a loop

      chistock[i_charge] = minchi[i_charge];
      cout << "Charge = " << i_charge << '\n';
      cout << "ydata size= " << ydata.size() << '\n';
      cout << "Zdata size= " << Zdata.size() << '\n';
      cout << "trueydata size= " << trueydata.size() << '\n';
      int plncount = 0;

      if(trueydata.size()<=1)
	{
	  continue;
	}

      for(int i=1; i<trueydata.size()+1; i++)
    {
      if(i==recontrackpln.at(plncount))
	{
      cout << "i= " << i  << " Z= " << Zdata.at(i-1) << " truey= " << trueydata.at(i-1) << " datay= " << ydata.at(plncount) << '\n';
      plncount++;
	}
      else
	{
      cout << "i= " << i  << " Z= " << Zdata.at(i-1) << " truey= " << trueydata.at(i-1) << " datay= " << " --- " << '\n';
	}
    }
   cout << '\n';

   if(i_charge==0)
     {
       for(int i=0; i<trueydata.size(); i++)
	 {
	   hist_recon->Fill(Zdata.at(i), trueydata.at(i));
	   z_forgra[i] = Zdata.at(i);
	   y_forgra[i] = trueydata.at(i);
	 }
     }

   if(i_charge==1)
     {
       for(int i=0; i<trueydata.size(); i++)
	 {
	   z_forgra1[i] = Zdata.at(i);
	   y_forgra1[i] = trueydata.at(i);
	 }
     }

    }// charge loop

  chivalue[0] = chistock[0];
  chivalue[1] = chistock[1];

  
  cout << "################ Chisqure #################" << '\n';
  cout << '\n';
  cout << "charge = -1 : " << chistock[0] << " charge = +1 : " << chistock[1] << '\n'; 
  cout << "!!!!!!!!!   " << "a= " << opta[0] << "  Y= " << opty[0] << " R= " << optr[0]  << " P= " << optp[0]  <<  "   !!!!!!!!!!" << '\n';
  cout << "!!!!!!!!!   " << "a= " << opta[1] << "  Y= " << opty[1] << " R= " << optr[1]  << " P= " << optp[1]  <<  "   !!!!!!!!!!" << '\n';
  cout << '\n';
  cout << "###########################################" << '\n';

  double Chicut = 1.0;
 
  if(chistock[0] <= chistock[1] && chistock[0] < Chicut) 
    {
      *cid = -1;
      *expectedmomentum_c = 0.3*B[0]*optr[0];
    }
  if(chistock[0] > chistock[1] && chistock[1] < Chicut) 
    {
      *cid = +1;
      *expectedmomentum_c = 0.3*B[0]*optr[1];
    }
  

    }//curvature


  *expectedmomentum_r = rangemomentum;

  const int Ngra = recontrackpln.size();

  cout << "Recon Track" << '\n';
  for(int i=0; i<Ngra; i++)
    {
      cout << "z= " << z_recon[i] << " y= " << y_recon[i] << '\n'; 
    }

  if(chivalue[0]<=chivalue[1])
    {
      Fittedplot = new TGraph(Ngra, z_forgra, y_forgra);
      Fittedplot->SetName("fittedplot");
    }

  if(chivalue[0]>chivalue[1])
    {
      Fittedplot = new TGraph(Ngra, z_forgra1, y_forgra1);     
      Fittedplot->SetName("fittedplot");
    }

  TGraph *fittedplot_nega = new TGraph(Ngra, z_forgra, y_forgra);
  fittedplot_nega->SetName("fittedplot_nega");
  fittedplot_nega->SetMarkerStyle(4);

  TGraph *fittedplot_posi = new TGraph(Ngra, z_forgra1, y_forgra1);
  fittedplot_posi->SetName("fittedplot_posi");
  fittedplot_posi->SetMarkerStyle(4);

  TGraph *reconplot = new TGraph(Ngra, z_recon, y_recon);
  reconplot->SetName("reconplot");
  reconplot->SetMarkerStyle(4);

  cout << "check" << '\n';


  histfile->cd();
  //rerecontrack->Write();
  //if(curvature) chimap->Write();
  //hist0->Write();

  
  //hist_recon->Write();
  fittedplot_nega->Write();
  fittedplot_posi->Write();
  reconplot->Write();
  

  //fittedplot1->Write();

 finish:

  histfile->Close();

  cout << "Fit finish" << '\n';
 
}
