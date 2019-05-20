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

using namespace std;

int positioncheck(double y)
{
  int check;
  if(y<27)
    {
      check = -1;
    }

  if(y>=27 && y<=67)
    {
      check = 0;
    }

  if(y>67)
    {
      check = +1;
    }

  return check;
}

void algorithm_A(vector<vector<double> >*ReconTrack, double &count_numu, double &count_numubar,
		 int minimumlimit)
{
  vector<vector<double> >dividetrackz;
  vector<vector<double> >dividetracky;
  vector<int> trackregion;

  dividetracky.clear();
  dividetrackz.clear();
  trackregion.clear();

  int last = ReconTrack->size();
  int count_track = 0;                                                                                        
  vector <double> temptracky;
  vector <double> temptrackz;                                                                                  
  temptracky.clear();
  temptrackz.clear();

  double hitcount_over = 0;
  double hitcount_under = 0;
  double count_over = 0;
  double count_under = 0;
  double count_numu_tmp = 0;
  double count_numubar_tmp = 0;
  count_numu = 0;
  count_numubar = 0;

  double weight;
  double distance = 0;
  double Ndata = ReconTrack->size();
  double D = ReconTrack->at(ReconTrack->size()-1).at(0) - ReconTrack->at(0).at(0);
                                                                                                              
  for(int i_datapoint=0; i_datapoint<last; i_datapoint++)                                                    
    {                                                     
      double datay = ReconTrack->at(i_datapoint).at(1);
      double dataz = ReconTrack->at(i_datapoint).at(0);
                                                                                                                         
      if(i_datapoint==0)                                                                                      
	{                                                                                                     
	  temptracky.push_back(datay);
	  temptrackz.push_back(dataz);                                                                         
	  trackregion.push_back(positioncheck(datay));                                                        
	}                                                                                                     
      if(i_datapoint!=0)                                                                                      
	{                                                                                                     
	  if(positioncheck(datay)==trackregion.at(count_track))                                               
	    {                                                                                                 
	      temptracky.push_back(datay);
	      temptrackz.push_back(dataz);                                                                     
	    }                                                                                                 
	  else if(positioncheck(datay)!=trackregion.at(count_track))                                          
	    {
	      if(temptracky.size()!=0)
		{
		  dividetracky.push_back(temptracky);
		  dividetrackz.push_back(temptrackz);                                                               
		  temptracky.clear();
		  temptrackz.clear();
		}             

	      count_track++;                                                                                  
	      temptracky.push_back(datay);
	      temptrackz.push_back(dataz);                                                                     
	      trackregion.push_back(positioncheck(datay));                                                    
                                                                                                      
	    }                                                                                               
	}                                                                    
                                                                                                             
    }// idatapoint loop

  
  if(temptracky.size()!=0)
    {
      dividetracky.push_back(temptracky);
      dividetrackz.push_back(temptrackz);
    }

  
  int NofTrack = trackregion.size();
  vector <vector<double> > currenttrack;
  vector <double> temptrack;


  for(int i=0; i<NofTrack; i++)
    {
      currenttrack.clear();
      hitcount_under = 0;
      hitcount_over = 0;

      if(dividetracky.at(i).size()<=minimumlimit)
	{
	  continue;
	}


      for(int i_data=0; i_data<dividetracky.at(i).size(); i_data++)
	{
	  temptrack.clear();
	  double z = dividetrackz.at(i).at(i_data);
	  double y = dividetracky.at(i).at(i_data);
	  temptrack.push_back(z);
	  temptrack.push_back(y);
	  currenttrack.push_back(temptrack);
	}

      
      int N_neighborhood = currenttrack.size()-1;
      
      for(int i_line=0; i_line<N_neighborhood; i_line++)
	{
	  double z1 = currenttrack.at(i_line).at(0); 
	  double z2 = currenttrack.at(i_line+1).at(0); 
	  double y1 = currenttrack.at(i_line).at(1); 
	  double y2 = currenttrack.at(i_line+1).at(1); 

	  double slope = (y1-y2)/(z1-z2);
	  double intercept = y1 - slope*z1;

	  int N_others = currenttrack.size();
	  
	  for(int j_line=0; j_line<N_others; j_line++)
	    {
	      if(j_line==i_line || j_line==i_line+1)
		{
		  continue;
		}

	      double tryz = currenttrack.at(j_line).at(0);
	      double tryy = currenttrack.at(j_line).at(1);

	      double judge = (tryy - (slope*tryz+intercept));

	      if(judge<0)
		{
		  hitcount_under += fabs(judge);
		}
	      if(judge>=0)
		{
		  hitcount_over += fabs(judge);
		}

	    }//j_line loop   

	}//iline loop



      if(trackregion.at(i) == 0)
	{
	  cout << "trackregion= " << trackregion.at(i) << '\n';
	  count_numu_tmp += hitcount_under;
	  count_numubar_tmp += hitcount_over;
	  //cout << "hitcount_under= " << hitcount_under << " hitcount_over= " << hitcount_over << '\n';
	}
      else
	{
	  cout << "trackregion= " << trackregion.at(i) << '\n';
	  count_numu_tmp += hitcount_over;
	  count_numubar_tmp += hitcount_under;

	  
	    {
	      //    cout << "hitcount_under= " << hitcount_under << " hitcount_over= " << hitcount_over << '\n';
	    }
	  
	}

      cout << "i= " << i << '\n';
      for(int k=0; k<currenttrack.size(); k++)
	{
	  cout << "z= " << currenttrack.at(k).at(0) <<  " y= " << currenttrack.at(k).at(1) << '\n';
	}

    }//NofTrack(i) loop

  count_numu_tmp = count_numu_tmp/(Ndata*D);
  count_numubar_tmp = count_numubar_tmp/(Ndata*D);

  count_numu = count_numu_tmp;
  count_numubar = count_numubar_tmp;


}





