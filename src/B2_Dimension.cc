#ifndef _B2_Dimension_C
#define _B2_Dimension_C

#include<iostream>
#include<sstream>
#include<fstream>
#include"B2_Dimension.hh"

using namespace std;

B2_Dimension::B2_Dimension(){
  f = TFile::Open("data/assembly_checklist.root","READ");
  t = (TTree*) f->Get("tree");
  double xy1,xy3;
  double xy0=0;
  int view,pln,ch;
  t -> SetBranchAddress("xy1",&xy1);
  t -> SetBranchAddress("xy3",&xy3);
  t -> SetBranchAddress("view",&view);
  t -> SetBranchAddress("pln",&pln);
  t -> SetBranchAddress("ch",&ch);
  for(int i=0;i<t->GetEntries();i++){
    t -> GetEntry(i);
    if(ch==0){
      xy0 = (xy1+xy3)/2./10. + 1.;//offset
    }
    if(ch<40){
      position_xy[view][pln][ch] = (xy1+xy3)/2./10.; //mm->cm
      position_xy[view][pln][ch] = position_xy[view][pln][ch] - xy0; //mm->cm
    }
    else{
      position_xy[view][pln][ch] = 0;
    }
  }
  f->Close();

  f = TFile::Open("data/position_module_z.root","READ");
  t = (TTree*) f->Get("tree");
  double z;
  double z0=0;
  t -> SetBranchAddress("z",&z);
  t -> SetBranchAddress("view",&view);
  t -> SetBranchAddress("pln",&pln);
  for(int i=0;i<t->GetEntries();i++){
    t -> GetEntry(i);
    if(i==0){
      z0 = z/10.;//offset
    }
    for(int ch_temp=0;ch_temp<80;ch_temp++){
      position_z [view][pln][ch_temp] = z/10.; //mm->cm
      position_z [view][pln][ch_temp] = position_z [view][pln][ch_temp] - z0;
    }
  }
  f->Close();
  std::cout << "call DIMENSION" << std::endl;
}

B2_Dimension::~B2_Dimension(){
}

bool B2_Dimension::get_pos(int mod, int pln, int ch, bool tpl, bool veto, double *posxy, double *posz){

   std::cout << "********************" << "\n"; 
  if(tpl){
    if(mod==6){
      *posxy = (ScintiWidth)*ch;
      *posz = (PlnThick+IronThick)*pln;
    }
    else if(mod==0){
      if(pln==0){
	*posxy = (ScintiWidth)*ch;;
	*posz = 0;
      }
      else{
	if(ch<8) *posxy = (ScintiWidth)*ch;
	else if(ch<24)*posxy = (ScintiWidth)*8+(ScibarWidth)*(ch-8);
	else *posxy = (ScintiWidth)*(ch-8);
	*posz = (PlnThick_PM)*(pln-1)+PlnThick_front_PM;
      }
    }
  }
  if(veto){
    *posxy=0;
    *posz = (ScintiWidth)*ch;
  }
  return true;
}

bool B2_Dimension::get_posXY(int mod, int view, int pln, int ch, double *posxy, double *posz){

  if(mod==6){
    *posxy   = (ScintiWidth)*ch;
    if( pln <= 10 ){
      if(view == 1) 
	*posz    = ( PlnThick + IronThick ) * pln ;
      else if(view == 0)
	*posz    = ( PlnThick + IronThick ) * pln + ScintiThick;
      return true;
    }
    else if(pln >= 11){
      this -> get_posVeto( mod, view, pln, ch, posxy, posz );
    }
  }
  else if(mod==0){
    if(pln==0){
    *posxy   = (ScintiWidth)*ch;
    if(view==0) *posz = 0;
    else *posz=PlnDist_PM;
    }
    else if( pln <= 17 ){
      if(ch<8) *posxy = (ScintiWidth)*ch;
      else if(ch<24)*posxy = (ScintiWidth)*8+(ScibarWidth)*(ch-8);
      else *posxy = (ScintiWidth)*(ch-8);
      if(view == 0) *posz = (PlnThick_PM)*(pln-1)+PlnThick_front_PM;
      else *posz = (PlnThick_PM)*(pln-1)+PlnThick_front_PM+PlnDist_PM;
      return true;
    }
    else if(pln >= 18){
      this -> get_posVeto( mod, view, pln, ch, posxy, posz );
    }
  }
  return true;
}

bool B2_Dimension::get_posVeto(int mod, int view, int pln, int ch, double *posxy, double *posz){

  if(mod==6){
    if(pln<=10||pln>=15) return false;
    
    *posz    = VetoStartZ     + ScintiWidth * ch;
    if(pln==11){//############ Right  VETO ################
      *posxy = VetoOffsetRight;
    }
    if(pln==12){//############ Left   VETO ################
      *posxy = VetoOffsetLeft;
    }
    if(pln==13){//############ Bottom VETO ################
      *posxy = VetoOffsetBottom;
    }
    if(pln==14){//############ Up VETO     ################
      *posxy = VetoOffsetUp;
    }
    return true;
  }
  else if(mod==0){
    if(pln<=17||pln>=22) return false;
    *posz  = VetoStartZ_PM + ScintiWidth*ch;
    if(pln==21){//Right VETO
      *posxy = VetoOffsetRight_PM;
      return true;
    }
    else if(pln==19){//Left VETO
      *posxy = VetoOffsetLeft_PM;
      return true;
    }
    else if(pln==18){//Bottom VETO
      *posxy = VetoOffsetBottom_PM;
      return true;
    }
    else if(pln==20){//Up VETO
      *posxy = VetoOffsetUp_PM;
      return true;
    }
  }

 return true;
}

double  Wi = 0.5;

bool B2_Dimension::get_expch(int mod, int pln, int *ch, bool tpl, bool veto, double a, double b)
{
  
  if(mod==6){
    if(tpl){
      double expz=pln*(PlnThick+IronThick);
      double expxy=expz*a+b;
      for(int numch=0;numch<48;numch++){
	double diffxy=expxy-numch*ScintiWidth;
	if(-Wi*ScintiWidth<=diffxy&&diffxy<Wi*ScintiWidth){
	  *ch=numch;
	  return true;
	}
      }
      return false;
    }
    if(veto){
      if(pln==0){//Right VETO
	double expz=(VetoOffsetRight-b)/a;
	for(int numch=0;numch<22;numch++){
	  double diffxy=expz-numch*ScintiWidth+VetoOffsetZY;
	  if(-Wi*ScintiWidth<=diffxy&&diffxy<Wi*ScintiWidth){
	    *ch=numch;
	    return true;
	  }
	}
      }
      else if(pln==1){//LEFT VETO
	double expz=(VetoOffsetLeft-b)/a;
	for(int numch=0;numch<22;numch++){
	  double diffxy=expz-numch*ScintiWidth+VetoOffsetZY;
	  if(-Wi*ScintiWidth<=diffxy&&diffxy<Wi*ScintiWidth){
	    *ch=numch;
	    return true;
	  }
	}
      }
      else if(pln==2){//Bottom VETO
	double expz=(VetoOffsetBottom-b)/a;
	for(int numch=0;numch<22;numch++){
	  double diffxy=expz-numch*ScintiWidth+VetoOffsetZX;
	  if(-Wi*ScintiWidth<=diffxy&&diffxy<Wi*ScintiWidth){
	    *ch=numch;
	    return true;
	  }
	}
      }
      else if(pln==3){//UP VETO
	double expz=(VetoOffsetUp-b)/a;
	for(int numch=0;numch<22;numch++){
	  double diffxy=expz-numch*ScintiWidth+VetoOffsetZX;
	  if(-Wi*ScintiWidth<=diffxy&&diffxy<Wi*ScintiWidth){
	    *ch=numch;
	    return true;
	  }
	}
      }
      return false;
    }
  }
  else if(mod==0){
    if(tpl){
      double expz=pln*(PlnThick+IronThick);
      double expxy=expz*a+b;
      for(int numch=0;numch<48;numch++){
	double diffxy=expxy-numch*ScintiWidth;
	if(-0.5*ScintiWidth<=diffxy&&diffxy<0.5*ScintiWidth){
	  *ch=numch;
	  return true;
	}
      }
      return false;
    }
    
    if(veto){
      if(pln==3){//Right VETO
	double expz=(VetoOffsetRight_PM-b)/a;
	for(int numch=0;numch<17;numch++){
	  double diffxy=expz-numch*ScintiWidth+VetoOffsetZY_PM;
	  if(-0.5*ScintiWidth<=diffxy&&diffxy<0.5*ScintiWidth){
	    *ch=numch;
	    return true;
	  }
	}
      }
      else if(pln==1){//LEFT VETO
	double expz=(VetoOffsetLeft_PM-b)/a;
	for(int numch=0;numch<17;numch++){
	  double diffxy=expz-numch*ScintiWidth+VetoOffsetZY_PM;
	  if(-0.5*ScintiWidth<=diffxy&&diffxy<0.5*ScintiWidth){
	    *ch=numch;
	    return true;
	  }
	}
      }
      else if(pln==0){//Bottom VETO
	double expz=(VetoOffsetBottom_PM-b)/a;
	for(int numch=0;numch<17;numch++){
	  double diffxy=expz-numch*ScintiWidth+VetoOffsetZX_PM;
	  if(-0.5*ScintiWidth<=diffxy&&diffxy<0.5*ScintiWidth){
	    *ch=numch;
	    return true;
	  }
	}
      }
      else if(pln==2){//UP VETO
	double expz=(VetoOffsetUp_PM-b)/a;
	for(int numch=0;numch<17;numch++){
	  double diffxy=expz-numch*ScintiWidth+VetoOffsetZX_PM;
	  if(-0.5*ScintiWidth<=diffxy&&diffxy<0.5*ScintiWidth){
	    *ch=numch;
	    return true;
	  }
	}
      }
      
      return false;
    }
  }
  return true;
}

bool B2_Dimension::get_expchXY(int mod, int view, int pln, int *ch, double a, double b){
  int temp=-777;
  if(mod==6){
    if(pln>=11){//VETO plane
      int veto = pln - 11;
      bool flag = this->get_expch(mod, veto, &temp, 0, 1, a, b);
      *ch = temp;
      return flag;
    }
    else {//Tracking plane
      bool flag = this->get_expch(mod, pln   , &temp, 1, 0, a, b);
      if(temp>23)return false;
      if(temp<0)return false;
      *ch = temp;
      return flag;
    }
  }
  else if(mod==0){
    if(pln>=18){//VETO plane
      int veto = pln - 18;
      bool flag = this->get_expch(mod, veto, &temp, 0, 1, a, b);
      *ch = temp;
      return flag;
    }
    else {//Tracking plane
      bool flag = this->get_expch(mod, pln   , &temp, 1, 0, a, b);
      if(temp>31)return false;
      if(temp<0)return false;
      *ch = temp;
      return flag;
    }
  }
  else return false;
}


bool B2_Dimension::get_pos_wgs(int mod, int view, int pln, int ch, int grid, double *posx, double *posy, double *posz){

  double x=0,y=0,z=0;
  if(view==0){
    x = 0;
    if(grid==0){
      y = wgs_offsetxy      + wgs_scinti_width/2. + position_xy[view][pln][ch]; //offset + scinti width + loop
      z = wgs_firstoffset_z + wgs_scinti_thick/2. + position_z [view][pln][ch]; //offset + scinti width + loop
    }
    else if(grid==1){
      y = wgs_offsetxy_grid + wgs_cutwidth/2.     + wgs_cutgap * ch;	//offset + cut width + loop
      z = wgs_firstoffset_z + wgs_scinti_thick    + wgs_scinti_width/2. + position_z [0][pln][ch]; //offset + offset + scinti width + loop
    }
    else if(grid==2){
      y = wgs_offsetxy_grid + wgs_cutwidth/2.     + wgs_cutgap * ch;	//offset + cut width + loop
      z = wgs_firstoffset_z + wgs_scinti_thick    + wgs_scinti_width/2. + position_z [1][pln][ch]; //offset + offset + offset + scinti width + loop
    }
    else return false;
  }
  else if(view==1){
    y = 0;
    if(grid==0){
      x = wgs_offsetxy      + wgs_scinti_width/2. + position_xy[view][pln][ch];  //offset + scinti width + loop
      z = wgs_firstoffset_z + wgs_scinti_thick/2. + position_z [view][pln][ch];  //offset + offset + scinti width + loop
    }
    else if(grid==1){
      x = wgs_offsetxy_grid + wgs_cutwidth/2.     + wgs_cutgap * ch;	//offset + cut width + loop
      z = wgs_firstoffset_z + wgs_scinti_thick    + wgs_scinti_width/2. + position_z [0][pln][ch]; //offset + offset + offset + scinti width + loop
    }
    else if(grid==2){
      x = wgs_offsetxy_grid + wgs_cutwidth/2.     + wgs_cutgap * ch;	//offset + cut width + loop
      z = wgs_firstoffset_z + wgs_scinti_thick    + wgs_scinti_width/2. + position_z [1][pln][ch]; //offset + offset + offset + scinti width + loop
    }
    else return false;
  }
  else return false;
  
  *posx = x;
  *posy = y;
  *posz = z;
  return true;
}


bool B2_Dimension::get_grid_wgs(int mod, int view, int pln, int ch, int *grid, int *gridch){
  if(ch>=0 && ch<40){
    *grid=0;
    *gridch=ch;
  }
  else if(ch>=40 && ch < 60){
    *grid=1;
    *gridch=ch-40;
  }
  else if(ch>=60 && ch < 80){
    *grid=2;
    *gridch=ch-60;
  }
  else{ 
    std::cout << "B2_Dimension  chnum exceed 80 or less than 0" << "\n";
    return false;
  }
  
  return true;
}

bool B2_Dimension::get_pos_wgs(int mod, int view, int pln, int ch, double *posx, double *posy, double *posz){
  int grid,gridch;
  double x,y,z;
  this->get_grid_wgs(mod, view, pln, ch, &grid, &gridch);
  this->get_pos_wgs (mod, view, pln, gridch,  grid, &x, &y, &z);
  *posx=x;
  *posy=y;
  *posz=z;
  return true;
}


bool B2_Dimension::get_pos_wgs_xy(int mod, int view, int pln, int ch, double *posxy, double *posz){
  int grid,gridch;
  double x,y,z;
  this->get_grid_wgs(mod, view, pln, ch, &grid, &gridch);
  this->get_pos_wgs (mod, view, pln, gridch,  grid, &x, &y, &z);
  if(view==0){
    *posxy=y;
  }
  else if(view==1){
    *posxy=x;
  }
  *posz =z;
  return true;
}



bool B2_Dimension::get_wgs_gridcell_id(int mod, int view, int pln, int ch, double posx, double posy, double posz, int* gridcell_id_x1, int* gridcell_id_x2, int* gridcell_id_y1, int* gridcell_id_y2){	
  int grid, gridch;
  double posx_ch,posy_ch,posz_ch;
  this->get_grid_wgs(mod, view, pln, ch, &grid, &gridch);
  if(view==0){
    if(grid==0){
      *gridcell_id_x1= (gridch+1)/2;
      for(int i=0;i<20;i++){
	this->get_pos_wgs(mod, 1, pln, i, 1, &posx_ch, &posy_ch, &posz_ch);
	if(posx_ch > posx){
	  *gridcell_id_y1 = i;
	  break;
	}
	if(i==19)*gridcell_id_y1 = 20;
      }
    }
    else if(grid==1){
      *gridcell_id_x1= gridch;
      *gridcell_id_x2= gridch+1;
      for(int i=0;i<20;i++){
	this->get_pos_wgs(mod, 1, pln, i, 1, &posx_ch, &posy_ch, &posz_ch);
	if(posx_ch > posx){
	  *gridcell_id_y1 = i;
	  break;
	}
	if(i==19)*gridcell_id_y1 = 20;
      }
    }
    else if(grid==2){
      *gridcell_id_x1= gridch+21;
      *gridcell_id_x2= gridch+21+1;
      for(int i=0;i<20;i++){
	this->get_pos_wgs(mod, 1, pln, i, 1, &posx_ch, &posy_ch, &posz_ch);
	if(posx_ch > posx){
	  *gridcell_id_y1 = i+21;
	  break;
	}
	if(i==19)*gridcell_id_y1 = 41;
      }
    }
  }
  
  else if(view==1){
    if(grid==0){
      *gridcell_id_y1= (gridch+1)/2 + 21;
      for(int i=0;i<20;i++){
	this->get_pos_wgs(mod, 0, pln, i, 1, &posx_ch, &posy_ch, &posz_ch);
	if(posy_ch > posy){
	  *gridcell_id_x1 = i+21;
	  break;
	}
	if(i==19)*gridcell_id_x1 = 41;
      }
    }
    else if(grid==1){
      *gridcell_id_y1= gridch;
      *gridcell_id_y2= gridch+1;
      for(int i=0;i<20;i++){
	this->get_pos_wgs(mod, 0, pln, i, 1, &posx_ch, &posy_ch, &posz_ch);
	if(posy_ch > posy){
	  *gridcell_id_x1 = i;
	  break;
	}
	if(i==19)*gridcell_id_x1 = 20;
      }
    }
    else if(grid==2){
      *gridcell_id_y1= gridch+21;
      *gridcell_id_y2= gridch+21+1;
      for(int i=0;i<20;i++){
	this->get_pos_wgs(mod, 0, pln, i, 1, &posx_ch, &posy_ch, &posz_ch);
	if(posy_ch > posy){
	  *gridcell_id_x1 = i+21;
	  break;
	}
	if(i==19)*gridcell_id_x1 = 41;
      }
    }
  }

  return true;  
}


bool B2_Dimension::get_pos_smrd(int mod, int pln, int ch, double *posx, double *posz){
  int dir;
  if(mod==3)dir=-1;
  else if(mod==4)dir=1;
  else return false;
  *posx = smrd_pln_gap*pln*dir;
  *posz = smrd_sci_width*ch;
  return true;
}


bool B2_Dimension::get_pos_bm(int mod, int view, int pln, int ch, double *posx, double *posy, double *posz){
  double detector_module_z[18] = {0, 25, 42.7, 53.7, 64.7, 88.9, 106.7,
				  138.2, 156.1, 186.8, 218.8, 236.7,
				  261.8, 279.5, 324.3, 369, 401.1, 411.1};

  double bmhgap=2.0414893617;
  double bmvgap=17.8;
  double bmsciz=0.75;

  if(view == 0){
    *posx = 0;
    *posy = bmhgap*ch;
    *posz = detector_module_z[pln]+bmsciz*(1+ch%2);
  }
  else{
    *posx = bmvgap*ch;
    *posy = 0;
    *posz = detector_module_z[pln]+bmsciz*(ch%2)*3;
  }

  return true;
}



#endif
