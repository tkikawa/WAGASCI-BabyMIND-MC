#include <iostream>
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
#include <TCut.h>

using namespace std;

bool coincidence_check(vector <double> *box, vector <double> *number)
{
  
  int N1 = number->size();
  int N2 = box->size();
  bool check;

  for(int i=0; i<N1; i++){
    check = false;
    for(int j=0; j<N2; j++){
      if(number->at(i)==box->at(j)){
	check = true;
      }
      if(j==N2-1){
	if(check==false){
	  goto finish;
	}
      }
      
    }
    
  }
  
 finish:

      if(check==false){
	return false;
      }

      if(check==true){
	return true;
      }
}
   

bool first_check(vector <double> *box, vector <double> *number){

  bool check = false;
  int N2 = box->size();
  for(int i=0; i<N2; i++){
    if(box->at(i)==1){
      check = true;
    }
  }

  return check;
  
}

bool coincidence_check_remove(vector <double> *box, vector <double> *number)
{
  
  int N1 = number->size();
  int N2 = box->size();
  bool check = true;

  for(int i=0; i<N1; i++){
    for(int j=0; j<N2; j++){
      if(number->at(i)==box->at(j)){
	check = false;
	goto finish;
      }
      
    }
    
  }
  
 finish:

      if(check==false){
	return false;
      }

      if(check==true){
	return true;
      }
}

bool TimingCheck(double time, int *Bunch){

  bool flag = false;
  int TW = 0;
  double bunch[9];
  *Bunch = 0;
  bunch[0] = 32170;
  bunch[1] = 32770;
  bunch[2] = 33340;
  bunch[3] = 33930;
  bunch[4] = 34520;
  bunch[5] = 35080;
  bunch[6] = 35670;
  bunch[7] = 36270;
  //bunch[8] = 100000;
  /*
  for(int i=0; i<9; i++){
    if(time < bunch[i]){
      TW = i;
      break;
    }    
  }
  */

  /*
  if(TW==0){
    if(time > bunch[TW] - 150){
      flag = true;
    }
  }
 
     
  if(TW==8){
    if(time < bunch[TW-1] + 150){
      flag = true;
    }
  }
    
  else{
    if(time < bunch[TW-1] + 150 || time > bunch[TW] - 150){
      flag = true;
    }
  }
  */

  for(int i=0; i<8; i++){
    if(time > bunch[i]-150 && time < bunch[i]+150){
      *Bunch = i+1;
      flag = true;
    }
  }
     
  
  return flag;
}

bool Centralregion(vector <double> *PosY, vector <double> *PosX, int index){
  if(PosY->at(index) < 500 && PosY->at(index) > -500 &&
     PosX->at(index) < 875 && PosX->at(index) > -875){
    return true;
  }
  else{
    return false;
  }
}
  
  

