#ifndef _Dimension_C
#define _Dimension_C

#include<iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include "DDimension.h"

using namespace std;

Dimension::Dimension()
{
  ifstream finh("/Users/jinaren/Documents/T2K/BabyMIND/commissioning_run/event_estimation/macro/Myanalysis/Analysis/v1_horizontal_from_center.txt");
  ifstream finv("/Users/jinaren/Documents/T2K/BabyMIND/commissioning_run/event_estimation/macro/Myanalysis/Analysis/v1_vertical_from_center.txt");
  ifstream finz("/Users/jinaren/Documents/T2K/BabyMIND/commissioning_run/event_estimation/macro/Myanalysis/Analysis/Scin_Mod_position.txt");
  ifstream finz_add("/Users/jinaren/Documents/T2K/BabyMIND/commissioning_run/event_estimation/macro/Myanalysis/Analysis/Scin_Mod_position.txt");
  ifstream finc("/Users/jinaren/Documents/T2K/BabyMIND/commissioning_run/event_estimation/macro/Myanalysis/Analysis/correction.txt");

  double temp1, temp2, temp3, temp4, temp5;

  for(int i=0; i<19; i++)
    {
      finc >> temp1 >> temp2;
      correction[i] = temp2;
    }

  for(int i=0; i<95; i++)
    {
      finh >> temp1 >> temp2 >> temp3 >> temp4 >> temp5;
      dataposy[i] = temp5;
    }

  for(int i=0; i<32; i++)
    {
      finv >> temp1 >> temp2 >> temp3 >> temp4 >> temp5;
      if(i%2==0)
        {
          dataposx[i/2] = temp5;
        }
    }

  dataposz[0] = -20;
  for(int i=0; i<18; i++)
    {
      finz >> temp1 >> temp2 >> temp3 >> temp4;
      dataposz[i] = temp4;
    }

  dataposz_add[0] = -20;
  for(int i=0; i<20; i++)
    {
      if(i==1) dataposz_add[i+1] = 95;
      else if(i==2) dataposz_add[i+1] = 166;
      else if(i!=1 && i!=2)
        {
          finz_add >> temp1 >> temp2 >> temp3 >> temp4;
          dataposz_add[i+1] = temp4;
        }
    }

  std::cout << "call DIMENSION" << std::endl;

  finh.close();
  finv.close();
  finz.close();
  finz_add.close();

}

Dimension::~Dimension(){
}

bool Dimension::get_pos_bm_FC(int mod, int view, int pln, double ch, double *posx, double *posy, double *posz, int smode)
{
  //std::cout << "********************" << "\n";
  double cm = 10;
  double vsciwidth = 210.6;
  double hsciwidth = 31.3;
  double epsilon = 0.1;
  double bmsciz = 0.75;
  double bmhgap = 2.04; 
  double bmvgap = 21.06;
  double zoffset = 0;
  double topoffset = 0;

  double bmhgap_add[3]={bmhgap, 20.0*cm, 2.5*cm};
  double bmvgap_add[3]={bmvgap, 20.0*cm, 2.5*cm};
  double bmsciz_add[3]={bmsciz, 0.35*cm, 0.3*cm};

  double SciBMHX_add, SciBMHY_add, SciBMHZ_add;
  double SciBMVX_add, SciBMVY_add, SciBMVZ_add;

  if(smode==1)
    {
      SciBMHX_add = 300.*cm/2;
      SciBMHY_add = 20.0*cm/2;
      SciBMHZ_add = 0.35*cm/2;

      SciBMVX_add = 20.*cm/2;
      SciBMVY_add = 200.*cm/2;
      SciBMVZ_add = 0.35*cm/2;
    }

  if(smode==2)
    {
      SciBMHX_add = 300.*cm/2;
      SciBMHY_add = 2.5*cm/2;
      SciBMHZ_add = 0.35*cm/2;

      SciBMVX_add = 2.5*cm/2;
      SciBMVY_add = 200.*cm/2;
      SciBMVZ_add = 0.35*cm/2;
    }

  if(smode==0)
    {

  if(view==0)
    {
      int minch = ch;
      if(fabs(minch-ch)<epsilon)
        {
          *posz = dataposz[pln] + (minch%2+1)*10 +10;
          *posy = dataposy[minch] + correction[pln];
        }
      if(fabs(minch-ch)>=epsilon)
        {
          *posz = dataposz[pln] + 20;
          *posy = (dataposy[minch] + dataposy[minch+1])/2 + correction[pln];
        }
    }

  if(view==1)
    {
      int minch = ch;
      if(fabs(minch-ch)<epsilon)
        {
          *posz = dataposz[pln] + (minch%2)*30 +5;
          *posx = dataposx[minch];
        }
      if(fabs(minch-ch)>=epsilon)
        {
          *posz = dataposz[pln] + 20;
          *posx = (dataposx[minch] + dataposx[minch+1])/2;
        }
    }
    }//smode==0

  if(smode!=0)
    {
  if(view == 0)
    {
      if(pln!=2 && pln!=3)
	{
	  int minch = ch;
	  if(fabs(minch-ch)<epsilon)
	    {
	      *posz = dataposz_add[pln] + (minch%2+1)*10 +10 + zoffset;
	      *posy = dataposy[minch] + correction[pln];
	    }
	  if(fabs(minch-ch)>=epsilon)
	    {
	      *posz = dataposz_add[pln] + 20 + zoffset;
	      *posy = (dataposy[minch] + dataposy[minch+1])/2 + correction[pln];
	    }
	}

      if(pln==2 || pln==3)
	{
	  *posx = 0;
	  *posy = bmhgap_add[smode]*ch - SciBMVY_add+SciBMHY_add;
	  *posz = dataposz_add[pln];
	}
    }

  if(view==1)
    {
      if(pln!=2 && pln!=3)
	{
	  int minch = ch;
	  if(fabs(minch-ch)<epsilon)
	    {
	      *posz = dataposz_add[pln] + (minch%2)*30 + 5 + zoffset;
	      *posx = dataposx[minch];
	    }
	  if(fabs(minch-ch)>=epsilon)
	    {
              *posz = dataposz_add[pln] + 20 + zoffset;
              *posx = (dataposx[minch] + dataposx[minch+1])/2;
	    }
	}

      if(pln==2 || pln==3)
	{
	  *posx = bmvgap_add[smode]*ch - SciBMHX_add+SciBMVX_add;
	  *posy = 0;
	  *posz = dataposz_add[pln] + zoffset;
	}

    }
    }//smode!=0


  return true;
}

#endif
