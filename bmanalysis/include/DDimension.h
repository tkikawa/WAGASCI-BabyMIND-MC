#ifndef _Dimension_H
#define _Dimension_H

#include<iostream>
#include<sstream>
#include<fstream>

using namespace std;

class Dimension{
 private:
  double dataposx[16], dataposy[95], dataposz[19];
  double dataposz_add[21];
  double correction[19];
 public:
  Dimension();
  ~Dimension();

  bool get_pos_bm_FC(int mod, int view, int pln, double ch, double *posx, double *posy, double *posz, int smode);
  //bool get_pos_bm_FC_add(int mod, int view, int pln, double ch, double *posx, double *posy, double *posz,int smode);

};
#endif
