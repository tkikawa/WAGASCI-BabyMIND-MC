#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
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

int Minimum(vector <double> *Chi, double Total){

        double temp_Chi = Chi->at(0);
	double temp_ID = 0;

	
	for(int i=0; i<Total; i++){
	  for(int j=i+1; j<Total; j++){
	    if(Chi->at(j) < temp_Chi){
	      temp_Chi = Chi->at(j);
	      temp_ID = j;
	      i = j;
	    }
	  }
	}



	return temp_ID;
	
}

void Multitrack(vector <double> *Chi, double Total, vector <double> *Candidate){
	
	for(int i=0; i<Total; i++){
	    if(Chi->at(i) < 6){
	      Candidate->push_back(i);
	  }
	}

}



void Recon(TH2 *hist){
 
  //**********Hit Point imformation ***********//
  //ix, iy, 
  //
  //*******************************************//

  //**********Cluster imformation ***********//
  //ix, hitx, hity,... 
  //
  //*****************************************//
    

  const int Nhit = hist->GetEntries();
  //cout << Nhit << '\n';
  
  vector <double> hit[1000];
  vector <double> cellx[100];
  vector <double> celly[100];
  vector <double> cell_tempx;
  vector <double> cell_tempy;
  vector <double> cell_tempChi;
  vector <double> cell_tempID;
  vector <double> cluster[1000];
  vector <double> clusterx[1000];
  vector <double> clustery[1000];
  vector <double> Candidate;
  vector <int> Number;
  vector <int> Number_sum;
  
  for(int i=0; i<1000; i++){
    hit[i].clear();
    cellx[i].clear();
    celly[i].clear();
    cell_tempx.clear();
    cell_tempy.clear();
    cell_tempChi.clear();
    cell_tempID.clear();
    cluster[i].clear();
    clusterx[i].clear();
    clustery[i].clear();
    Candidate.clear();
    Number.clear();
    Number_sum.clear();
  }

  TCanvas *c1 = new TCanvas("c1", "c1", 1000, 1000);
  c1->Divide(1,2);
  TCanvas *c2 = new TCanvas("c2", "c2", 1000, 1000);
  c2->Divide(1,2);
  TH2D *hit_hist = new TH2D("hit_hist", "hit_hist", 50, 0, 50, 95, 0, 95);
  TH2D *cluster_hist = new TH2D("cluster_hist", "cluster_hist", 50, 0, 50, 95, 0, 95);
 
  int count_temp = 0;
  int count_true = 0;
  int count_cluster = 0;
  bool cluster_candidate;

  for(int ix=0; ix<50; ix++){
    count_temp = count_true;
    
    for(int iy=0; iy<95; iy++){
      
      if(hist->GetBinContent(ix, iy)!=0){
	hit[count_true].push_back(ix);
	hit[count_true].push_back(iy);
	count_true++;
	hit_hist->Fill(ix, iy);
	
      }//condition for hist->GetBinContent
    }//iy loop

     //clustering
    if(count_true > count_temp){
      int start = count_temp;
      int stop = count_true;
      for(int i=start; i<stop; i++){
	cluster_candidate = false;
	for(int j=i+1; j<stop; j++){
	  if(abs(hit[i].at(1) - hit[j].at(1))<=2){
	    // cout << hit[i].at(1) - hit[j].at(1) << '\n';

	    if(cluster[count_cluster].size()==0){
	      cluster[count_cluster].push_back(hit[i].at(0));
	      cluster[count_cluster].push_back(hit[i].at(1));
	    }

	    cluster[count_cluster].push_back(hit[j].at(0));
	    cluster[count_cluster].push_back(hit[j].at(1));
	    cluster_candidate = true;
	    break;
	  }
	 
	}//j loop
	
	if(!cluster_candidate && cluster[count_cluster].size()==0){
	  cluster[count_cluster].push_back(hit[i].at(0));
	  cluster[count_cluster].push_back(hit[i].at(1));
	  count_cluster++;
	  //cout << "ix= " << ix <<  "!cluster" << '\n';
	}

	if(!cluster_candidate && cluster[count_cluster].size()!=0){	  
	  count_cluster++;
	  //cout << "ix= " << ix <<  "!!cluster" << '\n';
	}
	 
      }//i loop
     
      if(cluster_candidate){
	count_cluster++;
	//cout << "ix= "  << ix << "!!!cluster" << '\n';
      }

    }//condition for count_true
     
  }//ix loop

  const int NofCluster = count_cluster;
  int count_num = 0;
  double tempx = cluster[0].at(0);
  Number_sum.push_back(0);
  for(int i_cluster=0; i_cluster<NofCluster; i_cluster++){

    double tempy = 0;
    double temp;
    double temp_sum = 0;
        
    
      if(cluster[i_cluster].at(0)!=tempx){
    
	if(Number.size()!=0){
	  for(int i_sum=0; i_sum<count_num; i_sum++){
	    temp_sum +=Number.at(i_sum);
	  }
	  Number_sum.push_back(temp_sum);
	temp = i_cluster-temp_sum;
	
	Number.push_back(temp);
	tempx = cluster[i_cluster].at(0);
      }
	
      if(Number.size()==0){
	Number.push_back(i_cluster);
	tempx = cluster[i_cluster].at(0);
      }

      count_num++;
    }

    int Nofelement = cluster[i_cluster].size()/2;
    for(int i_element=0; i_element<Nofelement; i_element++){
      tempy += cluster[i_cluster].at(2*i_element+1);
    }
    clusterx[i_cluster].push_back(tempx);
    clusterx[i_cluster].push_back(count_num);
    clustery[i_cluster].push_back(tempy/(double)Nofelement);
    
    cout << clusterx[i_cluster].at(0) << " " << count_num  << '\n';
    cout << "size " << Number.size() << '\n'; 
    cluster_hist->Fill(clusterx[i_cluster].at(0), clustery[i_cluster].at(0));
  }

  double temp_Sum = 0;

  for(int i_sum=0; i_sum<count_num; i_sum++){
    temp_Sum += Number.at(i_sum);
  }
  Number_sum.push_back(temp_Sum);
 double temp = NofCluster-1-temp_Sum;
	
  Number.push_back(temp);
  tempx = cluster[NofCluster-1].at(0);

  count_num++;


  cout << "NOF " << count_num << '\n'; 
  // cout << count_true << '\n';

  // cout << cluster[0].size() << '\n';
 
  // cout << cluster[1].at(13) << '\n';
  
  c1->cd(1);
  hit_hist->SetMarkerStyle(4);
  hit_hist->SetMarkerSize(1);
  hit_hist->Draw();

  c1->cd(2);
  cluster_hist->SetMarkerStyle(4);
  cluster_hist->SetMarkerSize(1);
  cluster_hist->Draw();

  int count_cell = 0;
  int number = 0;

  /*
  cout << Number.at(0) << " " << Number.at(1) << " " << Number.at(2)  <<'\n';
  cout << Number.at(3) << " " << Number.at(4) << " " << Number.at(5)  <<'\n';
  cout << Number.at(6) << " " << Number.at(7) << " " << Number.at(8)  <<'\n';
  cout << Number.at(9) << " " << Number.at(10) << " " << Number.at(11)  <<'\n';
  cout << Number.at(12) << '\n';
  cout << Number_sum.at(0) << " " << Number_sum.at(1) << " " << Number_sum.at(2)  <<'\n';
  cout << Number_sum.at(3) << " " << Number_sum.at(4) << " " << Number_sum.at(5)  <<'\n';
  cout << Number_sum.at(6) << " " << Number_sum.at(7) << " " << Number_sum.at(8)  <<'\n';
  */

  TF1 *linear = new TF1("linear", "[0]*x+[1]", 0, 50);
  TH2D *fit_hist = new TH2D("fit_hist", "fit_hist", 50, 0, 50, 95, 0, 95);
  TH2D *fit_result = new TH2D("fit_result", "fit_result", 50, 0, 50, 95, 0, 95);

  int Count = 0;
  int Count_cell = 0;
  bool multi = false;
  
  for(int i=0; i<count_num-2; i++){ 
    if(i==0){
      for(int j=0; j<Number.at(i); j++){
	double x = clusterx[j].at(0);
	double y = clustery[j].at(0);
	cellx[j].push_back(x);
	celly[j].push_back(y);
      } //loop for j
    }
      int count_k = 0;
      cell_tempx.clear();
      cell_tempy.clear();
      Candidate.clear();

      double search_k, search_l;
      int Limit_l;
      
      
      if(i>=0 && i<=5){
	search_l = Number.at(i+4);
	Limit_l = Number_sum.at(i+3);
      }
      if(i>=6){
	search_l = Number.at(i+2);
	Limit_l = Number_sum.at(i+2);
      }
      
      for(int k=0; k<Number.at(i+1); k++){
	
	/*	if(k<Number.at(i+1)){
	  Limit_l = Number_sum.at(i+2);
	  search_l = Number.at(i+2);
	}
	if(k>=Number.at(i+1) && k<Number.at(i+2)){
	  Limit_l = Number_sum.at(i+3);
	  search_l = Number.at(i+3);
	}
	if(k>=Number.at(i+2) && k<Number.at(i+3)){
	  Limit_l = Number_sum.at(i+4);
	  search_l = Number.at(i+4);
	  }*/
	    
	  for(int l=0; l<search_l; l++){
	    fit_hist->Reset();
	    linear->SetParameters(0, 0);

	    double temp_x1 = cellx[0].at(Count_cell);
	    double temp_y1 = celly[0].at(Count_cell);
	    double temp_x2 = clusterx[k+Number_sum.at(i+1)].at(0);
	    double temp_y2 = clustery[k+Number_sum.at(i+1)].at(0);
	    //double temp_x3 = clusterx[l+Number_sum.at(i+2)].at(0);
	    //double temp_y3 = clustery[l+Number_sum.at(i+2)].at(0);
	    double temp_x3 = clusterx[l+Limit_l].at(0);
	    double temp_y3 = clustery[l+Limit_l].at(0);
	  
	    fit_hist->Fill(temp_x1, temp_y1);
	    fit_hist->Fill(temp_x2, temp_y2);
	    fit_hist->Fill(temp_x3, temp_y3);
	    
	    fit_hist->Fit(linear, "0", "", 0, 50);
	    double Chi;
	    Chi = linear->GetChisquare();
	    double NDF;
	    NDF = linear->GetNDF();
	    
	    if(Chi<6){
	      cout << i << " " << k << " " << l << '\n';
	      cell_tempx.push_back(temp_x2);
	      cell_tempy.push_back(temp_y2);
	      cell_tempChi.push_back(Chi);
	      count_k++;
	    }

	    //usleep(1000000);
	    Count++;
	    cout << fit_hist->GetEntries() << " " << Chi << '\n';
	    cout << temp_x1 << " " << Chi << '\n';

	  } //loop for l
      } //loop for k
      
	if(count_k>=1){
         	  
	  //int candidate = Minimum(&cell_tempChi, count_k);
	  Multitrack(&cell_tempChi, count_k, &Candidate);
	  int candidate;
	  if(Candidate.size() >= 2){
	    candidate = Candidate.at(1);
	    multi = true;
	  }

	  if(Candidate.size() == 1){
	    candidate = Candidate.at(0);
	  }
	  
	  cellx[0].push_back(cell_tempx.at(candidate));
	  celly[0].push_back(cell_tempy.at(candidate));



	  Count_cell++;
	}

 
	 cout << "i " << i << '\n';
	 cout << "count_num" << count_num << '\n';
  } //loop for i

 

  int N = cellx[0].size();

  for(int i_hist=0; i_hist<N; i_hist++){
    fit_result->Fill(cellx[0].at(i_hist), celly[0].at(i_hist));
  }

  cout << cluster_hist->GetEntries() << '\n';

  c2->cd(1);
  fit_result->Draw();
  fit_result->SetMarkerStyle(4);
  fit_result->SetMarkerSize(1);
  
  cout << Count << '\n';
  
  
  
       


}
